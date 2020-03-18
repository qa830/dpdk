/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(C) 2020 Marvell International Ltd.
 */

#include <inttypes.h>
#include <fnmatch.h>
#include <sys/queue.h>
#include <regex.h>

#include <rte_common.h>
#include <rte_errno.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_string_fns.h>

#include "eal_trace.h"

RTE_DEFINE_PER_LCORE(volatile int, trace_point_sz);
RTE_DEFINE_PER_LCORE(void *, trace_mem);
RTE_DEFINE_PER_LCORE(char, ctf_field[TRACE_CTF_FIELD_SIZE]);
RTE_DEFINE_PER_LCORE(int, ctf_count);

static struct trace_point_head tp_list = STAILQ_HEAD_INITIALIZER(tp_list);
static struct trace trace;

struct trace*
trace_obj_get(void)
{
	return &trace;
}

struct trace_point_head *
trace_list_head_get(void)
{
	return &tp_list;
}

int
eal_trace_init(void)
{
	/* Trace memory should start with 8B aligned for natural alignment */
	RTE_BUILD_BUG_ON((offsetof(struct __rte_trace_header, mem) % 8) != 0);

	/* One of the Trace registration failed */
	if (trace.register_errno) {
		rte_errno = trace.register_errno;
		goto fail;
	}

	if (rte_trace_global_is_disabled())
		return 0;

	rte_spinlock_init(&trace.lock);

	/* Is duplicate trace name registered */
	if (trace_has_duplicate_entry())
		goto fail;

	/* Generate UUID ver 4 with total size of events and number of events */
	trace_uuid_generate();

	/* Generate CTF TDSL metadata */
	if (trace_metadata_create())
		goto fail;

	/* Create trace directory */
	if (trace_mkdir())
		goto free_meta;

	/* Save current epoch timestamp for future use */
	if (trace_epoch_time_save())
		goto free_meta;

	rte_trace_global_mode_set(trace.mode);

	return 0;

free_meta:
	trace_metadata_destroy();
fail:
	trace_err("failed to initialize trace [%s]", rte_strerror(rte_errno));
	return -rte_errno;
}

void
eal_trace_fini(void)
{
	if (rte_trace_global_is_disabled())
		return;
	trace_mem_per_thread_free();
	trace_metadata_destroy();
}

bool
rte_trace_global_is_enabled(void)
{
	return trace.global_status;
}

bool
rte_trace_global_is_disabled(void)
{
	return !trace.global_status;
}

void
rte_trace_global_level_set(uint32_t level)
{
	struct trace_point *tp;

	trace.level = level;

	if (rte_trace_global_is_disabled())
		return;

	STAILQ_FOREACH(tp, &tp_list, next) {
		if (level >= rte_trace_level_get(tp->handle))
			rte_trace_enable(tp->handle);
		else
			rte_trace_disable(tp->handle);
	}
}

uint32_t
rte_trace_global_level_get(void)
{
	return trace.level;
}

void
rte_trace_global_mode_set(enum rte_trace_mode_e mode)
{
	struct trace_point *tp;

	if (rte_trace_global_is_disabled())
		return;

	STAILQ_FOREACH(tp, &tp_list, next)
		rte_trace_mode_set(tp->handle, mode);

	trace.mode = mode;
}

enum
rte_trace_mode_e rte_trace_global_mode_get(void)
{
	return trace.mode;
}

bool
rte_trace_is_id_invalid(rte_trace_t t)
{
	if (trace_id_get(t) >= trace.nb_trace_points)
		return true;

	return false;
}

bool
rte_trace_is_enabled(rte_trace_t trace)
{
	uint64_t val;

	if (rte_trace_is_id_invalid(trace))
		return false;

	val = __atomic_load_n(trace, __ATOMIC_ACQUIRE);
	return !!(val & __RTE_TRACE_FIELD_ENABLE_MASK);
}

bool
rte_trace_is_disabled(rte_trace_t trace)
{
	uint64_t val;

	if (rte_trace_is_id_invalid(trace))
		return true;

	val = __atomic_load_n(trace, __ATOMIC_ACQUIRE);
	return !!!(val & __RTE_TRACE_FIELD_ENABLE_MASK);
}

int
rte_trace_enable(rte_trace_t trace)
{
	if (rte_trace_is_id_invalid(trace))
		return -ERANGE;

	if (rte_trace_level_get(trace) > rte_trace_global_level_get())
		return -EACCES;

	__atomic_or_fetch(trace, __RTE_TRACE_FIELD_ENABLE_MASK,
			  __ATOMIC_RELEASE);
	return 0;
}

int
rte_trace_disable(rte_trace_t trace)
{
	if (rte_trace_is_id_invalid(trace))
		return -ERANGE;

	__atomic_and_fetch(trace, ~__RTE_TRACE_FIELD_ENABLE_MASK,
			   __ATOMIC_RELEASE);
	return 0;
}

uint32_t
rte_trace_level_get(rte_trace_t trace)
{
	uint64_t val;

	if (rte_trace_is_id_invalid(trace))
		return 0;

	val = __atomic_load_n(trace, __ATOMIC_ACQUIRE);
	val &= __RTE_TRACE_FIELD_LEVEL_MASK;
	val = val >> __RTE_TRACE_FIELD_LEVEL_SHIFT;

	return val;
}

int
rte_trace_level_set(rte_trace_t trace, uint32_t level)
{
	uint64_t val;

	if (rte_trace_is_id_invalid(trace) || level > RTE_LOG_DEBUG)
		return -EINVAL;

	val = __atomic_load_n(trace, __ATOMIC_ACQUIRE);
	val &= ~__RTE_TRACE_FIELD_LEVEL_MASK;
	val |= (uint64_t)level << __RTE_TRACE_FIELD_LEVEL_SHIFT;
	__atomic_store_n(trace, val, __ATOMIC_RELEASE);

	if (level <= rte_trace_global_level_get())
		rte_trace_enable(trace);
	else
		rte_trace_disable(trace);

	return 0;
}

int
rte_trace_mode_get(rte_trace_t trace)
{
	uint64_t val;

	if (rte_trace_is_id_invalid(trace))
		return -EINVAL;

	val = __atomic_load_n(trace, __ATOMIC_ACQUIRE);

	return !!(val & __RTE_TRACE_FIELD_ENABLE_DISCARD);
}

int
rte_trace_mode_set(rte_trace_t trace, enum rte_trace_mode_e mode)
{
	if (rte_trace_is_id_invalid(trace) || mode >  RTE_TRACE_MODE_DISCARD)
		return -EINVAL;

	if (mode == RTE_TRACE_MODE_OVERWRITE)
		__atomic_and_fetch(trace, ~__RTE_TRACE_FIELD_ENABLE_DISCARD,
				   __ATOMIC_RELEASE);
	else
		__atomic_or_fetch(trace, __RTE_TRACE_FIELD_ENABLE_DISCARD,
				   __ATOMIC_RELEASE);

	return 0;
}

int
rte_trace_pattern(const char *pattern, bool enable, bool *found)
{
	struct trace_point *tp;
	int rc;

	if (found)
		*found = false;

	STAILQ_FOREACH(tp, &tp_list, next) {
		if (fnmatch(pattern, tp->name, 0) == 0) {
			if (found)
				*found = true;
			if (enable)
				rc = rte_trace_enable(tp->handle);
			else
				rc = rte_trace_disable(tp->handle);
		}
		if (rc < 0)
			return rc;
	}
	return 0;
}

int
rte_trace_regexp(const char *regex, bool enable, bool *found)
{
	struct trace_point *tp;
	regex_t r;
	int rc;

	if (regcomp(&r, regex, 0) != 0)
		return -EINVAL;

	if (found)
		*found = false;

	STAILQ_FOREACH(tp, &tp_list, next) {
		if (regexec(&r, tp->name, 0, NULL, 0) == 0) {
			if (found)
				*found = true;
			if (enable)
				rc = rte_trace_enable(tp->handle);
			else
				rc = rte_trace_disable(tp->handle);
		}
		if (rc < 0)
			return rc;
	}
	regfree(&r);
	return 0;
}

rte_trace_t
rte_trace_from_name(const char *name)
{
	struct trace_point *tp;

	if (name == NULL)
		return NULL;

	STAILQ_FOREACH(tp, &tp_list, next)
		if (strncmp(tp->name, name, TRACE_POINT_NAME_SIZE) == 0)
			return tp->handle;

	return NULL;
}

static void
trace_point_dump(FILE *f, struct trace_point *tp)
{
	rte_trace_t handle = tp->handle;
	uint32_t level;

	level = rte_trace_level_get(handle);
	fprintf(f, "\tid %d, %s, size is %d, level is %s, mode is %s, %s\n",
		trace_id_get(handle), tp->name,
		(uint16_t)(*handle & __RTE_TRACE_FIELD_SIZE_MASK),
		eal_loglevel_to_string(level),
		trace_mode_to_string(rte_trace_mode_get(handle)),
		rte_trace_is_enabled(handle) ? "enabled" : "disabled");
}

static void
trace_lcore_mem_dump(FILE *f)
{
	struct trace *trace = trace_obj_get();
	struct __rte_trace_header *header;
	uint32_t count;

	if (trace->nb_trace_mem_list == 0)
		return;

	rte_spinlock_lock(&trace->lock);
	fprintf(f, "nb_trace_mem_list = %d\n", trace->nb_trace_mem_list);
	fprintf(f, "\nTrace mem info\n--------------\n");
	for (count = 0; count < trace->nb_trace_mem_list; count++) {
		header = trace->lcore_meta[count].mem;
		fprintf(f, "\tid %d, mem=%p, area=%s, lcore_id=%d, name=%s\n",
		count, header,
		trace_area_to_string(trace->lcore_meta[count].area),
		header->stream_header.lcore_id,
		header->stream_header.thread_name);
	}
	rte_spinlock_unlock(&trace->lock);
}

void
rte_trace_dump(FILE *f)
{
	struct trace_point_head *tp_list = trace_list_head_get();
	struct trace *trace = trace_obj_get();
	struct trace_point *tp;

	fprintf(f, "\nGlobal info\n-----------\n");
	fprintf(f, "status = %s\n",
		rte_trace_global_is_enabled() ? "enabled" : "disabled");
	fprintf(f, "log level = %s\n",
		eal_loglevel_to_string(rte_trace_global_level_get()));
	fprintf(f, "mode = %s\n",
		trace_mode_to_string(rte_trace_global_mode_get()));
	fprintf(f, "dir = %s\n", trace->dir);
	fprintf(f, "buffer len = %d\n", trace->buff_len);
	fprintf(f, "number of trace points = %d\n", trace->nb_trace_points);

	trace_lcore_mem_dump(f);
	fprintf(f, "\nTrace point info\n----------------\n");
	STAILQ_FOREACH(tp, tp_list, next)
		trace_point_dump(f, tp);
}

static inline size_t
list_sz(uint32_t index)
{
	return sizeof(struct thread_mem_meta) * (index + 1);
}

void
__rte_trace_mem_per_thread_alloc(void)
{
	struct trace *trace = trace_obj_get();
	struct __rte_trace_header *header;
	uint32_t count;

	if (rte_trace_global_is_disabled())
		return;

	if (RTE_PER_LCORE(trace_mem))
		return;

	rte_spinlock_lock(&trace->lock);

	count = trace->nb_trace_mem_list;

	/* Allocate room for storing the thread trace mem meta */
	trace->lcore_meta = realloc(trace->lcore_meta, list_sz(count));

	/* Provide dummy space for fastpath to consume */
	if (trace->lcore_meta == NULL) {
		trace_crit("trace mem meta memory realloc failed");
		header = NULL; goto fail;
	}

	/* First attempt from huge page */
	header = eal_malloc_no_trace(NULL, trace_mem_sz(trace->buff_len), 8);
	if (header) {
		trace->lcore_meta[count].area = TRACE_AREA_HUGEPAGE;
		goto found;
	}

	/* Second attempt from heap */
	header = malloc(trace_mem_sz(trace->buff_len));
	if (header == NULL) {
		trace_crit("trace mem malloc attempt failed");
		header = NULL; goto fail;

	}

	/* Second attempt from heap is success */
	trace->lcore_meta[count].area = TRACE_AREA_HEAP;

	/* Initialize the trace header */
found:
	header->offset = 0;
	header->len = trace->buff_len;
	header->stream_header.magic = TRACE_CTF_MAGIC;
	rte_uuid_copy(header->stream_header.uuid, trace->uuid);
	header->stream_header.lcore_id = rte_lcore_id();

	/* Store the thread name */
	char *name = header->stream_header.thread_name;
	memset(name, 0, __RTE_TRACE_EMIT_STRING_LEN_MAX);
	rte_thread_getname(pthread_self(), name,
			   __RTE_TRACE_EMIT_STRING_LEN_MAX);

	trace->lcore_meta[count].mem = header;
	trace->nb_trace_mem_list++;
fail:
	RTE_PER_LCORE(trace_mem) = header;
	rte_spinlock_unlock(&trace->lock);
}

void
trace_mem_per_thread_free(void)
{
	struct trace *trace = trace_obj_get();
	uint32_t count;
	void *mem;

	if (rte_trace_global_is_disabled())
		return;

	rte_spinlock_lock(&trace->lock);
	for (count = 0; count < trace->nb_trace_mem_list; count++) {
		mem = trace->lcore_meta[count].mem;
		if (trace->lcore_meta[count].area == TRACE_AREA_HUGEPAGE)
			eal_free_no_trace(mem);
		else if (trace->lcore_meta[count].area == TRACE_AREA_HEAP)
			free(mem);
	}
	rte_spinlock_unlock(&trace->lock);
}

void
__rte_trace_emit_ctf_field(size_t sz, const char *in, const char *datatype)
{
	char *field = RTE_PER_LCORE(ctf_field);
	int count = RTE_PER_LCORE(ctf_count);
	int rc;

	RTE_PER_LCORE(trace_point_sz) += sz;
	rc = snprintf(RTE_PTR_ADD(field, count),
		      RTE_MAX(0, TRACE_CTF_FIELD_SIZE - 1 - count),
		      "%s %s;", datatype, in);
	if (rc <= 0) {
		RTE_PER_LCORE(trace_point_sz) = 0;
		trace_crit("CTF field is too long");
		return;
	}
	RTE_PER_LCORE(ctf_count) += rc;
}

int
__rte_trace_point_register(rte_trace_t handle, const char *name, uint32_t level,
			 void (*fn)(void))
{
	char *field = RTE_PER_LCORE(ctf_field);
	struct trace_point *tp;
	uint16_t sz;

	/* Sanity checks of arguments */
	if (name == NULL || fn == NULL || handle == NULL) {
		trace_err("invalid arguments");
		rte_errno = EINVAL; goto fail;
	}

	/* Sanity check of level */
	if (level > RTE_LOG_DEBUG || level > UINT8_MAX) {
		trace_err("invalid log level=%d", level);
		rte_errno = EINVAL; goto fail;

	}

	/* Check the size of the trace point object */
	RTE_PER_LCORE(trace_point_sz) = 0;
	RTE_PER_LCORE(ctf_count) = 0;
	fn();
	if (RTE_PER_LCORE(trace_point_sz) == 0) {
		trace_err("missing rte_trace_emit_header() in register fn");
		rte_errno = EBADF; goto fail;
	}

	/* Is size overflowed */
	if (RTE_PER_LCORE(trace_point_sz) > UINT16_MAX) {
		trace_err("trace point size overflowed");
		rte_errno = ENOSPC; goto fail;
	}

	/* Are we running out of space to store trace points? */
	if (trace.nb_trace_points > UINT16_MAX) {
		trace_err("trace point exceeds the max count");
		rte_errno = ENOSPC; goto fail;
	}

	/* Get the size of the trace point */
	sz = RTE_PER_LCORE(trace_point_sz);
	tp = calloc(1, sizeof(struct trace_point));
	if (tp == NULL) {
		trace_err("fail to allocate trace point memory");
		rte_errno = ENOMEM; goto fail;
	}

	/* Initialize the trace point */
	if (rte_strscpy(tp->name, name, TRACE_POINT_NAME_SIZE) < 0) {
		trace_err("name is too long");
		rte_errno = E2BIG;
		goto free;
	}

	/* Copy the field data for future use */
	if (rte_strscpy(tp->ctf_field, field, TRACE_CTF_FIELD_SIZE) < 0) {
		trace_err("CTF field size is too long");
		rte_errno = E2BIG;
		goto free;
	}

	/* Clear field memory for the next event */
	memset(field, 0, TRACE_CTF_FIELD_SIZE);

	/* Form the trace handle */
	*handle = sz;
	*handle |= trace.nb_trace_points << __RTE_TRACE_FIELD_ID_SHIFT;
	*handle |= (uint64_t)level << __RTE_TRACE_FIELD_LEVEL_SHIFT;

	trace.nb_trace_points++;
	tp->handle = handle;

	/* Add the trace point at tail */
	STAILQ_INSERT_TAIL(&tp_list, tp, next);
	__atomic_thread_fence(__ATOMIC_RELEASE);

	/* All Good !!! */
	return 0;
free:
	free(tp);
fail:
	if (trace.register_errno == 0)
		trace.register_errno = rte_errno;

	return -rte_errno;
}
