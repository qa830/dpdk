/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2019 Intel Corporation
 */

#include <rte_eal_memconfig.h>
#include <rte_errno.h>
#include <rte_hash.h>
#include <rte_jhash.h>
#include <rte_malloc.h>
#include <rte_random.h>
#include <rte_rwlock.h>
#include <rte_tailq.h>

#include "rte_ipsec_sad.h"

/*
 * Rules are stored in three hash tables depending on key_type.
 * Each rule will also be stored in SPI_ONLY table.
 * for each data entry within this table last two bits are reserved to
 * indicate presence of entries with the same SPI in DIP and DIP+SIP tables.
 */

#define IPSEC_SAD_NAMESIZE	64
#define SAD_PREFIX		"SAD_"
/* "SAD_<name>" */
#define SAD_FORMAT		SAD_PREFIX "%s"

#define DEFAULT_HASH_FUNC	rte_jhash

struct hash_cnt {
	uint32_t cnt_2;
	uint32_t cnt_3;
};

struct rte_ipsec_sad {
	char name[IPSEC_SAD_NAMESIZE];
	struct rte_hash	*hash[RTE_IPSEC_SAD_KEY_TYPE_MASK];
	__extension__ struct hash_cnt cnt_arr[];
};

TAILQ_HEAD(rte_ipsec_sad_list, rte_tailq_entry);
static struct rte_tailq_elem rte_ipsec_sad_tailq = {
	.name = "RTE_IPSEC_SAD",
};
EAL_REGISTER_TAILQ(rte_ipsec_sad_tailq)

#define SET_BIT(ptr, bit)	(void *)((uintptr_t)(ptr) | (uintptr_t)(bit))
#define CLEAR_BIT(ptr, bit)	(void *)((uintptr_t)(ptr) & ~(uintptr_t)(bit))
#define GET_BIT(ptr, bit)	(void *)((uintptr_t)(ptr) & (uintptr_t)(bit))

/*
 * @internal helper function
 * Add a rule of type SPI_DIP or SPI_DIP_SIP.
 * Inserts a rule into an appropriate hash table,
 * updates the value for a given SPI in SPI_ONLY hash table
 * reflecting presence of more specific rule type in two LSBs.
 * Updates a counter that reflects the number of rules whith the same SPI.
 */
static inline int
add_specific(struct rte_ipsec_sad *sad, void *key,
		int key_type, void *sa)
{
	void *tmp_val;
	int ret, notexist;

	ret = rte_hash_lookup(sad->hash[key_type], key);
	notexist = (ret == -ENOENT);
	ret = rte_hash_add_key_data(sad->hash[key_type], key, sa);
	if (ret != 0)
		return ret;
	ret = rte_hash_lookup_data(sad->hash[RTE_IPSEC_SAD_SPI_ONLY],
		key, &tmp_val);
	if (ret < 0)
		tmp_val = NULL;
	tmp_val = SET_BIT(tmp_val, key_type);
	ret = rte_hash_add_key_data(sad->hash[RTE_IPSEC_SAD_SPI_ONLY],
		key, tmp_val);
	if (ret != 0)
		return ret;
	ret = rte_hash_lookup(sad->hash[RTE_IPSEC_SAD_SPI_ONLY], key);
	if (key_type == RTE_IPSEC_SAD_SPI_DIP)
		sad->cnt_arr[ret].cnt_2 += notexist;
	else
		sad->cnt_arr[ret].cnt_3 += notexist;

	return 0;
}

int
rte_ipsec_sad_add(struct rte_ipsec_sad *sad, union rte_ipsec_sad_key *key,
		int key_type, void *sa)
{
	void *tmp_val;
	int ret;

	if ((sad == NULL) || (key == NULL) || (sa == NULL) ||
			/* sa must be 4 byte aligned */
			(GET_BIT(sa, RTE_IPSEC_SAD_KEY_TYPE_MASK) != 0))
		return -EINVAL;

	/*
	 * Rules are stored in three hash tables depending on key_type.
	 * All rules will also have an entry in SPI_ONLY table, with entry
	 * value's two LSB's also indicating presence of rule with this SPI
	 * in other tables.
	 */
	switch (key_type) {
	case(RTE_IPSEC_SAD_SPI_ONLY):
		ret = rte_hash_lookup_data(sad->hash[key_type],
			key, &tmp_val);
		if (ret >= 0)
			tmp_val = SET_BIT(sa, GET_BIT(tmp_val,
				RTE_IPSEC_SAD_KEY_TYPE_MASK));
		else
			tmp_val = sa;
		ret = rte_hash_add_key_data(sad->hash[key_type],
			key, tmp_val);
		return ret;
	case(RTE_IPSEC_SAD_SPI_DIP):
	case(RTE_IPSEC_SAD_SPI_DIP_SIP):
		return add_specific(sad, key, key_type, sa);
	default:
		return -EINVAL;
	}
}

/*
 * @internal helper function
 * Delete a rule of type SPI_DIP or SPI_DIP_SIP.
 * Deletes an entry from an appropriate hash table and decrements
 * an entry counter for given SPI.
 * If entry to remove is the last one with given SPI within the table,
 * then it will also update related entry in SPI_ONLY table.
 * Removes an entry from SPI_ONLY hash table if there no rule left
 * for this SPI in any table.
 */
static inline int
del_specific(struct rte_ipsec_sad *sad, void *key, int key_type)
{
	void *tmp_val;
	int ret;
	uint32_t *cnt;

	ret = rte_hash_del_key(sad->hash[key_type], key);
	if (ret < 0)
		return ret;
	ret = rte_hash_lookup_data(sad->hash[RTE_IPSEC_SAD_SPI_ONLY],
		key, &tmp_val);
	if (ret < 0)
		return ret;
	cnt = (key_type == RTE_IPSEC_SAD_SPI_DIP) ? &sad->cnt_arr[ret].cnt_2 :
			&sad->cnt_arr[ret].cnt_3;
	if (--(*cnt) != 0)
		return 0;

	tmp_val = CLEAR_BIT(tmp_val, key_type);
	if (tmp_val == NULL)
		ret = rte_hash_del_key(sad->hash[RTE_IPSEC_SAD_SPI_ONLY], key);
	else
		ret = rte_hash_add_key_data(sad->hash[RTE_IPSEC_SAD_SPI_ONLY],
			key, tmp_val);
	if (ret < 0)
		return ret;
	return 0;
}

int
rte_ipsec_sad_del(struct rte_ipsec_sad *sad, union rte_ipsec_sad_key *key,
		int key_type)
{
	void *tmp_val;
	int ret;

	if ((sad == NULL) || (key == NULL))
		return -EINVAL;
	switch (key_type) {
	case(RTE_IPSEC_SAD_SPI_ONLY):
		ret = rte_hash_lookup_data(sad->hash[key_type],
			key, &tmp_val);
		if (ret < 0)
			return ret;
		if (GET_BIT(tmp_val, RTE_IPSEC_SAD_KEY_TYPE_MASK) == 0) {
			RTE_ASSERT((cnt_2 == 0) && (cnt_3 == 0));
			ret = rte_hash_del_key(sad->hash[key_type],
				key);
		} else {
			tmp_val = GET_BIT(tmp_val,
				RTE_IPSEC_SAD_KEY_TYPE_MASK);
			ret = rte_hash_add_key_data(sad->hash[key_type],
				key, tmp_val);
		}
		return ret;
	case(RTE_IPSEC_SAD_SPI_DIP):
	case(RTE_IPSEC_SAD_SPI_DIP_SIP):
		return del_specific(sad, key, key_type);
	default:
		return -EINVAL;
	}
}

struct rte_ipsec_sad *
rte_ipsec_sad_create(const char *name, struct rte_ipsec_sad_conf *conf)
{
	char hash_name[RTE_HASH_NAMESIZE];
	struct rte_tailq_entry *te;
	struct rte_ipsec_sad_list *sad_list;
	struct rte_ipsec_sad *sad, *tmp_sad = NULL;
	struct rte_hash_parameters hash_params = {0};
	int ret;
	uint32_t sa_sum;

	RTE_BUILD_BUG_ON(RTE_IPSEC_SAD_KEY_TYPE_MASK != 3);

	if ((name == NULL) || (conf == NULL) ||
			(conf->max_sa[RTE_IPSEC_SAD_SPI_ONLY] == 0) ||
			(conf->max_sa[RTE_IPSEC_SAD_SPI_DIP] == 0) ||
			(conf->max_sa[RTE_IPSEC_SAD_SPI_DIP_SIP] == 0) ||
			/* check that either IPv4 or IPv6 type flags
			 * are configured
			 */
			((!!(conf->flags & RTE_IPSEC_SAD_FLAG_IPV4) ^
			!!(conf->flags & RTE_IPSEC_SAD_FLAG_IPV6)) == 0)) {
		rte_errno = EINVAL;
		return NULL;
	}

	/** Init SAD*/
	sa_sum = conf->max_sa[RTE_IPSEC_SAD_SPI_ONLY] +
		conf->max_sa[RTE_IPSEC_SAD_SPI_DIP] +
		conf->max_sa[RTE_IPSEC_SAD_SPI_DIP_SIP];
	sad = rte_zmalloc_socket(NULL, sizeof(*sad) +
		(sizeof(struct hash_cnt) * sa_sum),
		RTE_CACHE_LINE_SIZE, conf->socket_id);
	if (sad == NULL) {
		rte_errno = ENOMEM;
		return NULL;
	}

	ret = snprintf(sad->name, sizeof(sad->name), SAD_FORMAT, name);
	if (ret < 0 || ret >= (int)sizeof(sad->name)) {
		rte_errno = ENAMETOOLONG;
		return NULL;
	}

	hash_params.hash_func = DEFAULT_HASH_FUNC;
	hash_params.hash_func_init_val = rte_rand();
	hash_params.socket_id = conf->socket_id;
	hash_params.name = hash_name;
	if (conf->flags & RTE_IPSEC_SAD_FLAG_RW_CONCURRENCY)
		hash_params.extra_flag = RTE_HASH_EXTRA_FLAGS_RW_CONCURRENCY;

	/** Init hash[RTE_IPSEC_SAD_SPI_ONLY] for SPI only */
	ret = snprintf(hash_name, sizeof(hash_name),
		"sad_%p_1", sad);
	if (ret < 0 || ret >= (int)sizeof(hash_name)) {
		rte_errno = ENAMETOOLONG;
		return NULL;
	}
	hash_params.key_len = sizeof(((struct rte_ipsec_sadv4_key *)0)->spi);
	hash_params.entries = sa_sum;
	sad->hash[RTE_IPSEC_SAD_SPI_ONLY] = rte_hash_create(&hash_params);
	if (sad->hash[RTE_IPSEC_SAD_SPI_ONLY] == NULL) {
		rte_ipsec_sad_free(sad);
		return NULL;
	}

	/** Init hash_2 for SPI + DIP */
	ret = snprintf(hash_name, sizeof(hash_name),
		"sad_%p_2", sad);
	if (ret < 0 || ret >= (int)sizeof(hash_name)) {
		rte_errno = ENAMETOOLONG;
		rte_ipsec_sad_free(sad);
		return NULL;
	}
	if (conf->flags & RTE_IPSEC_SAD_FLAG_IPV4)
		hash_params.key_len +=
			sizeof(((struct rte_ipsec_sadv4_key *)0)->dip);
	else
		hash_params.key_len +=
			sizeof(((struct rte_ipsec_sadv6_key *)0)->dip);
	hash_params.entries = conf->max_sa[RTE_IPSEC_SAD_SPI_DIP];
	sad->hash[RTE_IPSEC_SAD_SPI_DIP] = rte_hash_create(&hash_params);
	if (sad->hash[RTE_IPSEC_SAD_SPI_DIP] == NULL) {
		rte_ipsec_sad_free(sad);
		return NULL;
	}

	/** Init hash_3 for SPI + DIP + SIP */
	ret = snprintf(hash_name, sizeof(hash_name),
		"sad_%p_3", name);
	if (ret < 0 || ret >= (int)sizeof(hash_name)) {
		rte_errno = ENAMETOOLONG;
		rte_ipsec_sad_free(sad);
		return NULL;
	}
	if (conf->flags & RTE_IPSEC_SAD_FLAG_IPV4)
		hash_params.key_len +=
			sizeof(((struct rte_ipsec_sadv4_key *)0)->sip);
	else
		hash_params.key_len +=
			sizeof(((struct rte_ipsec_sadv6_key *)0)->sip);
	hash_params.entries = conf->max_sa[RTE_IPSEC_SAD_SPI_DIP_SIP];
	sad->hash[RTE_IPSEC_SAD_SPI_DIP_SIP] = rte_hash_create(&hash_params);
	if (sad->hash[RTE_IPSEC_SAD_SPI_DIP_SIP] == NULL) {
		rte_ipsec_sad_free(sad);
		return NULL;
	}

	sad_list = RTE_TAILQ_CAST(rte_ipsec_sad_tailq.head,
			rte_ipsec_sad_list);
	rte_mcfg_tailq_write_lock();
	/* guarantee there's no existing */
	TAILQ_FOREACH(te, sad_list, next) {
		tmp_sad = (struct rte_ipsec_sad *)te->data;
		if (strncmp(name, tmp_sad->name, IPSEC_SAD_NAMESIZE) == 0)
			break;
	}
	if (te != NULL) {
		rte_mcfg_tailq_write_unlock();
		rte_errno = EEXIST;
		rte_ipsec_sad_free(sad);
		return NULL;
	}

	/* allocate tailq entry */
	te = rte_zmalloc("IPSEC_SAD_TAILQ_ENTRY", sizeof(*te), 0);
	if (te == NULL) {
		rte_mcfg_tailq_write_unlock();
		rte_errno = ENOMEM;
		rte_ipsec_sad_free(sad);
		return NULL;
	}

	te->data = (void *)sad;
	TAILQ_INSERT_TAIL(sad_list, te, next);
	rte_mcfg_tailq_write_unlock();
	return sad;
}

struct rte_ipsec_sad *
rte_ipsec_sad_find_existing(const char *name)
{
	struct rte_ipsec_sad *sad = NULL;
	struct rte_tailq_entry *te;
	struct rte_ipsec_sad_list *sad_list;


	sad_list = RTE_TAILQ_CAST(rte_ipsec_sad_tailq.head,
		rte_ipsec_sad_list);

	rte_mcfg_tailq_read_lock();
	TAILQ_FOREACH(te, sad_list, next) {
		sad = (struct rte_ipsec_sad *) te->data;
		if (strncmp(name, sad->name, IPSEC_SAD_NAMESIZE) == 0)
			break;
	}
	rte_mcfg_tailq_read_unlock();

	if (te == NULL) {
		rte_errno = ENOENT;
		return NULL;
	}

	return sad;
}

void
rte_ipsec_sad_free(struct rte_ipsec_sad *sad)
{
	struct rte_tailq_entry *te;
	struct rte_ipsec_sad_list *sad_list;

	if (sad == NULL)
		return;

	sad_list = RTE_TAILQ_CAST(rte_ipsec_sad_tailq.head,
			rte_ipsec_sad_list);
	rte_mcfg_tailq_write_lock();
	TAILQ_FOREACH(te, sad_list, next) {
		if (te->data == (void *)sad)
			break;
	}
	if (te != NULL)
		TAILQ_REMOVE(sad_list, te, next);

	rte_mcfg_tailq_write_unlock();

	rte_hash_free(sad->hash[RTE_IPSEC_SAD_SPI_ONLY]);
	rte_hash_free(sad->hash[RTE_IPSEC_SAD_SPI_DIP]);
	rte_hash_free(sad->hash[RTE_IPSEC_SAD_SPI_DIP_SIP]);
	rte_free(sad);
	if (te != NULL)
		rte_free(te);
}

static int
__ipsec_sad_lookup(const struct rte_ipsec_sad *sad,
		const union rte_ipsec_sad_key *keys[], uint32_t n, void *sa[])
{
	const void *keys_2[RTE_HASH_LOOKUP_BULK_MAX];
	const void *keys_3[RTE_HASH_LOOKUP_BULK_MAX];
	void *vals_2[RTE_HASH_LOOKUP_BULK_MAX] = {NULL};
	void *vals_3[RTE_HASH_LOOKUP_BULK_MAX] = {NULL};
	uint32_t idx_2[RTE_HASH_LOOKUP_BULK_MAX];
	uint32_t idx_3[RTE_HASH_LOOKUP_BULK_MAX];
	uint64_t mask_1, mask_2, mask_3;
	uint64_t map, map_spec;
	uint32_t n_2 = 0;
	uint32_t n_3 = 0;
	uint32_t i;
	int n_pkts = 0;

	for (i = 0; i < n; i++)
		sa[i] = NULL;

	/*
	 * Lookup keys in SPI only hash table first.
	 */
	rte_hash_lookup_bulk_data(sad->hash[RTE_IPSEC_SAD_SPI_ONLY],
		(const void **)keys, n, &mask_1, sa);
	for (map = mask_1; map; map &= (map - 1)) {
		i = rte_bsf64(map);
		/*
		 * if returned value indicates presence of a rule in other
		 * tables save a key for further lookup.
		 */
		if ((uintptr_t)sa[i] & RTE_IPSEC_SAD_SPI_DIP_SIP) {
			idx_3[n_3] = i;
			keys_3[n_3++] = keys[i];
		}
		if ((uintptr_t)sa[i] & RTE_IPSEC_SAD_SPI_DIP) {
			idx_2[n_2] = i;
			keys_2[n_2++] = keys[i];
		}
		sa[i] = CLEAR_BIT(sa[i], RTE_IPSEC_SAD_KEY_TYPE_MASK);
	}

	if (n_2 != 0) {
		rte_hash_lookup_bulk_data(sad->hash[RTE_IPSEC_SAD_SPI_DIP],
			keys_2, n_2, &mask_2, vals_2);
		for (map_spec = mask_2; map_spec; map_spec &= (map_spec - 1)) {
			i = rte_bsf64(map_spec);
			sa[idx_2[i]] = vals_2[i];
		}
	}
	if (n_3 != 0) {
		rte_hash_lookup_bulk_data(sad->hash[RTE_IPSEC_SAD_SPI_DIP_SIP],
			keys_3, n_3, &mask_3, vals_3);
		for (map_spec = mask_3; map_spec; map_spec &= (map_spec - 1)) {
			i = rte_bsf64(map_spec);
			sa[idx_3[i]] = vals_3[i];
		}
	}
	for (i = 0; i < n; i++)
		n_pkts += (sa[i] != NULL);

	return n_pkts;
}

int
rte_ipsec_sad_lookup(const struct rte_ipsec_sad *sad,
		const union rte_ipsec_sad_key *keys[], uint32_t n, void *sa[])
{
	uint32_t num, i = 0;
	int n_pkts = 0;

	if (unlikely((sad == NULL) || (keys == NULL) || (sa == NULL)))
		return -EINVAL;

	do {
		num = RTE_MIN(n - i, (uint32_t)RTE_HASH_LOOKUP_BULK_MAX);
		n_pkts += __ipsec_sad_lookup(sad,
			&keys[i], num, &sa[i]);
		i += num;
	} while (i != n);

	return n_pkts;
}
