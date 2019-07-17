#! /bin/sh -e
# SPDX-License-Identifier: BSD-3-Clause
# Copyright 2015 6WIND S.A.
# Copyright 2019 Mellanox Technologies, Ltd

# Run a quick testpmd forwarding with null PMD without hugepage

build=${1:-build}
coremask=${2:-3} # default using cores 0 and 1

testpmd=$build/app/dpdk-testpmd
[ -f $testpmd ] || testpmd=$build/app/testpmd
if [ ! -f $testpmd ] ; then
	echo 'ERROR: testpmd cannot be found' >&2
	exit 1
fi

unset libs
if ldd $testpmd | grep -q librte_ ; then
	libs='-d librte_mempool_ring.so -d librte_pmd_null.so'
fi

(sleep 1 && echo stop) |
$testpmd -c $coremask --no-huge -m 150 \
	$libs --vdev net_null1 --vdev net_null2 -- \
	--no-mlockall --total-num-mbufs=2048 -ia
