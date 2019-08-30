/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright (C) 2019 Marvell International Ltd.
 */

#ifndef _OTX2_CRYPTODEV_H_
#define _OTX2_CRYPTODEV_H_

#include "cpt_common.h"

#include "otx2_dev.h"

/* Marvell OCTEON TX2 Crypto PMD device name */
#define CRYPTODEV_NAME_OCTEONTX2_PMD	crypto_octeontx2

#define OTX2_CPT_MAX_LFS		64
#define OTX2_CPT_MAX_QUEUES_PER_VF	64

/**
 * Device private data
 */
struct otx2_cpt_vf {
	struct otx2_dev otx2_dev;
	/**< Base class */
	uint16_t max_queues;
	/**< Max queues supported */
};

/*
 * Crypto device driver ID
 */
uint8_t otx2_cryptodev_driver_id;

#endif /* _OTX2_CRYPTODEV_H_ */
