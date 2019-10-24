/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2017 Cavium, Inc
 * Copyright(c) 2019 Arm Limited
 */

#ifndef _RTE_PAUSE_H_
#define _RTE_PAUSE_H_

/**
 * @file
 *
 * CPU pause operation.
 *
 */

#include <stdint.h>
#include <rte_common.h>
#include <rte_atomic.h>
#include <rte_compat.h>
#include <assert.h>

/**
 * Pause CPU execution for a short while
 *
 * This call is intended for tight loops which poll a shared resource or wait
 * for an event. A short pause within the loop may reduce the power consumption.
 */
static inline void rte_pause(void);

static inline void rte_sevl(void);
static inline void rte_wfe(void);
/**
 * @warning
 * @b EXPERIMENTAL: this API may change, or be removed, without prior notice
 *
 * Atomic load from addr, it returns the 16-bit content of *addr.
 *
 * @param addr
 *  A pointer to the memory location.
 * @param memorder
 *  The valid memory order variants are __ATOMIC_ACQUIRE and __ATOMIC_RELAXED.
 *  These map to C++11 memory orders with the same names, see the C++11 standard
 *  the GCC wiki on atomic synchronization for detailed definitions.
 */
static __rte_always_inline uint16_t
__atomic_load_ex_16(volatile uint16_t *addr, int memorder);

/**
 * @warning
 * @b EXPERIMENTAL: this API may change, or be removed, without prior notice
 *
 * Atomic load from addr, it returns the 32-bit content of *addr.
 *
 * @param addr
 *  A pointer to the memory location.
 * @param memorder
 *  The valid memory order variants are __ATOMIC_ACQUIRE and __ATOMIC_RELAXED.
 *  These map to C++11 memory orders with the same names, see the C++11 standard
 *  the GCC wiki on atomic synchronization for detailed definitions.
 */
static __rte_always_inline uint32_t
__atomic_load_ex_32(volatile uint32_t *addr, int memorder);

/**
 * @warning
 * @b EXPERIMENTAL: this API may change, or be removed, without prior notice
 *
 * Atomic load from addr, it returns the 64-bit content of *addr.
 *
 * @param addr
 *  A pointer to the memory location.
 * @param memorder
 *  The valid memory order variants are __ATOMIC_ACQUIRE and __ATOMIC_RELAXED.
 *  These map to C++11 memory orders with the same names, see the C++11 standard
 *  the GCC wiki on atomic synchronization for detailed definitions.
 */
static __rte_always_inline uint64_t
__atomic_load_ex_64(volatile uint64_t *addr, int memorder);

/**
 * @warning
 * @b EXPERIMENTAL: this API may change, or be removed, without prior notice
 *
 * Wait for *addr to be updated with a 16-bit expected value, with a relaxed
 * memory ordering model meaning the loads around this API can be reordered.
 *
 * @param addr
 *  A pointer to the memory location.
 * @param expected
 *  A 16-bit expected value to be in the memory location.
 * @param memorder
 *  Two different memory orders that can be specified:
 *  __ATOMIC_ACQUIRE and __ATOMIC_RELAXED. These map to
 *  C++11 memory orders with the same names, see the C++11 standard or
 *  the GCC wiki on atomic synchronization for detailed definition.
 */
__rte_experimental
static __rte_always_inline void
rte_wait_until_equal_16(volatile uint16_t *addr, uint16_t expected,
int memorder);

/**
 * @warning
 * @b EXPERIMENTAL: this API may change, or be removed, without prior notice
 *
 * Wait for *addr to be updated with a 32-bit expected value, with a relaxed
 * memory ordering model meaning the loads around this API can be reordered.
 *
 * @param addr
 *  A pointer to the memory location.
 * @param expected
 *  A 32-bit expected value to be in the memory location.
 * @param memorder
 *  Two different memory orders that can be specified:
 *  __ATOMIC_ACQUIRE and __ATOMIC_RELAXED. These map to
 *  C++11 memory orders with the same names, see the C++11 standard or
 *  the GCC wiki on atomic synchronization for detailed definition.
 */
__rte_experimental
static __rte_always_inline void
rte_wait_until_equal_32(volatile uint32_t *addr, uint32_t expected,
int memorder);

/**
 * @warning
 * @b EXPERIMENTAL: this API may change, or be removed, without prior notice
 *
 * Wait for *addr to be updated with a 64-bit expected value, with a relaxed
 * memory ordering model meaning the loads around this API can be reordered.
 *
 * @param addr
 *  A pointer to the memory location.
 * @param expected
 *  A 64-bit expected value to be in the memory location.
 * @param memorder
 *  Two different memory orders that can be specified:
 *  __ATOMIC_ACQUIRE and __ATOMIC_RELAXED. These map to
 *  C++11 memory orders with the same names, see the C++11 standard or
 *  the GCC wiki on atomic synchronization for detailed definition.
 */
__rte_experimental
static __rte_always_inline void
rte_wait_until_equal_64(volatile uint64_t *addr, uint64_t expected,
int memorder);

#ifdef RTE_ARM_USE_WFE
#define RTE_WAIT_UNTIL_EQUAL_ARCH_DEFINED
#endif

#ifndef RTE_WAIT_UNTIL_EQUAL_ARCH_DEFINED
static inline void rte_sevl(void)
{
}

static inline void rte_wfe(void)
{
	rte_pause();
}

/**
 * @warning
 * @b EXPERIMENTAL: this API may change, or be removed, without prior notice
 *
 * Atomic load from addr, it returns the 16-bit content of *addr.
 *
 * @param addr
 *  A pointer to the memory location.
 * @param memorder
 *  The valid memory order variants are __ATOMIC_ACQUIRE and __ATOMIC_RELAXED.
 *  These map to C++11 memory orders with the same names, see the C++11 standard
 *  the GCC wiki on atomic synchronization for detailed definitions.
 */
static __rte_always_inline uint16_t
__atomic_load_ex_16(volatile uint16_t *addr, int memorder)
{
	uint16_t tmp;
	assert((memorder == __ATOMIC_ACQUIRE)
			|| (memorder == __ATOMIC_RELAXED));
	tmp = __atomic_load_n(addr, memorder);
	return tmp;
}

static __rte_always_inline uint32_t
__atomic_load_ex_32(volatile uint32_t *addr, int memorder)
{
	uint32_t tmp;
	assert((memorder == __ATOMIC_ACQUIRE)
			|| (memorder == __ATOMIC_RELAXED));
	tmp = __atomic_load_n(addr, memorder);
	return tmp;
}

static __rte_always_inline uint64_t
__atomic_load_ex_64(volatile uint64_t *addr, int memorder)
{
	uint64_t tmp;
	assert((memorder == __ATOMIC_ACQUIRE)
			|| (memorder == __ATOMIC_RELAXED));
	tmp = __atomic_load_n(addr, memorder);
	return tmp;
}

static __rte_always_inline void
rte_wait_until_equal_16(volatile uint16_t *addr, uint16_t expected,
int memorder)
{
	if (__atomic_load_n(addr, memorder) != expected) {
		rte_sevl();
		do {
			rte_wfe();
		} while (__atomic_load_ex_16(addr, memorder) != expected);
	}
}

static __rte_always_inline void
rte_wait_until_equal_32(volatile uint32_t *addr, uint32_t expected,
int memorder)
{
	if (__atomic_load_ex_32(addr, memorder) != expected) {
		rte_sevl();
		do {
			rte_wfe();
		} while (__atomic_load_ex_32(addr, memorder) != expected);
	}
}

static __rte_always_inline void
rte_wait_until_equal_64(volatile uint64_t *addr, uint64_t expected,
int memorder)
{
	if (__atomic_load_ex_64(addr, memorder) != expected) {
		rte_sevl();
		do {
			rte_wfe();
		} while (__atomic_load_ex_64(addr, memorder) != expected);
	}
}
#endif

#endif /* _RTE_PAUSE_H_ */
