/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2020 Intel Corporation.
 */

#ifndef _AESNI_MB_MFN_RAWDEV_TEST_VECTORS_H_
#define _AESNI_MB_MFN_RAWDEV_TEST_VECTORS_H_

#include <stdbool.h>

/*
 * DOCSIS test data and cases
 * - encrypt direction: CRC-Crypto
 * - decrypt direction: Crypto-CRC
 */
struct docsis_test_data {
	struct {
		uint8_t data[16];
		unsigned int len;
	} key;

	struct {
		uint8_t data[16] __rte_aligned(16);
		unsigned int len;
	} cipher_iv;

	struct {
		uint8_t data[1024];
		unsigned int len;
		unsigned int cipher_offset;
		unsigned int crc_offset;
		bool no_cipher;
		bool no_crc;
	} plaintext;

	struct {
		uint8_t data[1024];
		unsigned int len;
		unsigned int cipher_offset;
		unsigned int crc_offset;
		bool no_cipher;
		bool no_crc;
	} ciphertext;
};

struct docsis_test_data docsis_test_case_1 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 24,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x7A, 0xF0,
			/* CRC */
			0x61, 0xF8, 0x63, 0x42
		},
		.len = 24,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	}
};

struct docsis_test_data docsis_test_case_2 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 25,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x7A, 0xF0, 0xDF,
			/* CRC */
			0xFE, 0x12, 0x99, 0xE5
		},
		.len = 25,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	}
};

struct docsis_test_data docsis_test_case_3 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 34,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0xD6, 0xE2, 0x70, 0x5C,
			0xE6, 0x4D, 0xCC, 0x8C, 0x47, 0xB7, 0x09, 0xD6,
			/* CRC */
			0x54, 0x85, 0xF8, 0x32
		},
		.len = 34,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	}
};

struct docsis_test_data docsis_test_case_4 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 35,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x92, 0x6A, 0xC2, 0xDC,
			0xEE, 0x3B, 0x31, 0xEC, 0x03, 0xDE, 0x95, 0x33,
			0x5E,
			/* CRC */
			0xFE, 0x47, 0x3E, 0x22
		},
		.len = 35,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	}
};

struct docsis_test_data docsis_test_case_5 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 82,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x77, 0x74, 0x56, 0x05,
			0xD1, 0x14, 0xA2, 0x8D, 0x2C, 0x9A, 0x11, 0xFC,
			0x7D, 0xB0, 0xE7, 0x18, 0xCE, 0x75, 0x7C, 0x89,
			0x14, 0x56, 0xE2, 0xF2, 0xB7, 0x47, 0x08, 0x27,
			0xF7, 0x08, 0x7A, 0x13, 0x90, 0x81, 0x75, 0xB0,
			0xC7, 0x91, 0x04, 0x83, 0xAD, 0x11, 0x46, 0x46,
			0xF8, 0x54, 0x87, 0xA0, 0x42, 0xF3, 0x71, 0xA9,
			0x8A, 0xCD, 0x59, 0x77, 0x67, 0x11, 0x1A, 0x87,
			/* CRC */
			0xAB, 0xED, 0x2C, 0x26
		},
		.len = 82,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	}
};

struct docsis_test_data docsis_test_case_6 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 83,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x77, 0x74, 0x56, 0x05,
			0xD1, 0x14, 0xA2, 0x8D, 0x2C, 0x9A, 0x11, 0xFC,
			0x7D, 0xB0, 0xE7, 0x18, 0xCE, 0x75, 0x7C, 0x89,
			0x14, 0x56, 0xE2, 0xF2, 0xB7, 0x47, 0x08, 0x27,
			0xF7, 0x08, 0x7A, 0x13, 0x90, 0x81, 0x75, 0xB0,
			0xC7, 0x91, 0x04, 0x83, 0xAD, 0x11, 0x46, 0x46,
			0xF8, 0x54, 0x87, 0xA0, 0xA4, 0x0C, 0xC2, 0xF0,
			0x81, 0x49, 0xA8, 0xA6, 0x6C, 0x48, 0xEB, 0x1F,
			0x4B,
			/* CRC */
			0x2F, 0xD4, 0x48, 0x18
		},
		.len = 83,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	}
};

struct docsis_test_data docsis_test_case_7 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 83,
		.cipher_offset = 40,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0x3B, 0x9F, 0x72, 0x4C, 0xB5, 0x72,
			0x3E, 0x56, 0x54, 0x49, 0x13, 0x53, 0xC4, 0xAA,
			0xCD, 0xEA, 0x6A, 0x88, 0x99, 0x07, 0x86, 0xF4,
			0xCF, 0x03, 0x4E, 0xDF, 0x65, 0x61, 0x47, 0x5B,
			0x2F, 0x81, 0x09, 0x12, 0x9A, 0xC2, 0x24, 0x8C,
			0x09,
			/* CRC */
			0x11, 0xB4, 0x06, 0x33
		},
		.len = 83,
		.cipher_offset = 40,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = false
	}
};

struct docsis_test_data docsis_test_case_8 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 24,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = true
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x7A, 0xF0,
			/* CRC */
			0x8A, 0x0F, 0x74, 0xE8
		},
		.len = 24,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = true
	}
};

struct docsis_test_data docsis_test_case_9 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 83,
		.cipher_offset = 40,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = true
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0x3B, 0x9F, 0x72, 0x4C, 0xB5, 0x72,
			0x3E, 0x56, 0x54, 0x49, 0x13, 0x53, 0xC4, 0xAA,
			0xCD, 0xEA, 0x6A, 0x88, 0x99, 0x07, 0x86, 0xF4,
			0xCF, 0x03, 0x4E, 0xDF, 0x65, 0x61, 0x47, 0x5B,
			0x2F, 0x81, 0x09, 0x12, 0x9A, 0xC2, 0x24, 0x8C,
			0x09,
			/* CRC */
			0x5D, 0x2B, 0x12, 0xF4
		},
		.len = 83,
		.cipher_offset = 40,
		.crc_offset = 6,
		.no_cipher = false,
		.no_crc = true
	}
};

struct docsis_test_data docsis_test_case_10 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 24,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = true,
		.no_crc = false
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00,
			/* CRC */
			0x14, 0x08, 0xE8, 0x55
		},
		.len = 24,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = true,
		.no_crc = false
	}
};

struct docsis_test_data docsis_test_case_11 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 83,
		.cipher_offset = 40,
		.crc_offset = 6,
		.no_cipher = true,
		.no_crc = false
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA,
			/* CRC */
			0xB3, 0x60, 0xEB, 0x38
		},
		.len = 83,
		.cipher_offset = 40,
		.crc_offset = 6,
		.no_cipher = true,
		.no_crc = false
	}
};

struct docsis_test_data docsis_test_case_12 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 24,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = true,
		.no_crc = true
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 24,
		.cipher_offset = 18,
		.crc_offset = 6,
		.no_cipher = true,
		.no_crc = true
	}
};

struct docsis_test_data docsis_test_case_13 = {
	.key = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD,
			0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
			0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 83,
		.cipher_offset = 40,
		.crc_offset = 6,
		.no_cipher = true,
		.no_crc = true
	},
	.ciphertext = {
		.data = {
			/* DOCSIS header */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x05,
			0x04, 0x03, 0x02, 0x01, 0x08, 0x00, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
			0xAA,
			/* CRC */
			0xFF, 0xFF, 0xFF, 0xFF
		},
		.len = 83,
		.cipher_offset = 40,
		.crc_offset = 6,
		.no_cipher = true,
		.no_crc = true
	}
};

/*
 * GPON test data and cases
 * - encrypt direction: CRC-Crypto-BIP
 * - decrypt direction: BIP-Crypto-CRC
 */
struct gpon_test_data {
	struct {
		uint8_t data[16];
		unsigned int len;
	} key;

	struct {
		uint8_t data[16] __rte_aligned(16);
		unsigned int len;
	} cipher_iv;

	struct {
		uint8_t data[1024];
		unsigned int len;
		unsigned int cipher_offset;
		unsigned int crc_offset;
		unsigned int bip_offset;
		unsigned int padding_len;
		bool no_cipher;
	} plaintext;

	struct {
		uint8_t data[1024];
		unsigned int len;
		unsigned int cipher_offset;
		unsigned int crc_offset;
		unsigned int bip_offset;
		unsigned int padding_len;
		bool no_cipher;
	} ciphertext;

	struct {
	uint8_t data[8];
		unsigned int len;
	} output;
};

struct gpon_test_data gpon_test_case_1 = {
	.key = {
		.data = {
			0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
			0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* XGEM header */
			0x00, 0x20, 0x27, 0x11, 0x00, 0x00, 0x21, 0x23,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04,
			/* CRC */
			0x05, 0x06, 0x01, 0x01
		},
		.len = 16,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 0,
		.no_cipher = false
	},
	.ciphertext = {
		.data = {
			/* XGEM header */
			0x00, 0x20, 0x27, 0x11, 0x00, 0x00, 0x21, 0x23,
			/* Ethernet frame */
			0xC7, 0x62, 0x82, 0xCA,
			/* CRC */
			0x3E, 0x92, 0xC8, 0x5A
		},
		.len = 16,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 0,
		.no_cipher = false
	},
	.output = {
		/* Expected BIP */
		.data = {0xF9, 0xD0, 0x4C, 0xA2},
		.len  = 4
	}
};

struct gpon_test_data gpon_test_case_2 = {
	.key = {
		.data = {
			0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
			0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* XGEM header */
			0x00, 0x40, 0x27, 0x11, 0x00, 0x00, 0x29, 0x3C,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01,
			/* CRC */
			0x81, 0x00, 0x00, 0x01
		},
		.len = 24,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 0,
		.no_cipher = false
	},
	.ciphertext = {
		.data = {
			/* XGEM header */
			0x00, 0x40, 0x27, 0x11, 0x00, 0x00, 0x29, 0x3C,
			/* Ethernet frame */
			0xC7, 0x62, 0x82, 0xCA, 0xF6, 0x6F, 0xF5, 0xED,
			0xB7, 0x90, 0x1E, 0x02,
			/* CRC */
			0xEA, 0x38, 0xA1, 0x78
		},
		.len = 24,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 0,
		.no_cipher = false
	},
	.output = {
		/* Expected BIP */
		.data = {0x6C, 0xE5, 0xC6, 0x70},
		.len  = 4
	}
};

struct gpon_test_data gpon_test_case_3 = {
	.key = {
		.data = {
			0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
			0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* XGEM header */
			0x01, 0x00, 0x27, 0x11, 0x00, 0x00, 0x33, 0x0B,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x81, 0x00, 0x00, 0x01,
			0x08, 0x00, 0x45, 0x00, 0x00, 0x6A, 0xB0, 0x7E,
			0x00, 0x00, 0x04, 0x06, 0x83, 0xBD, 0xC0, 0xA8,
			0x00, 0x01, 0xC0, 0xA8, 0x01, 0x01, 0x04, 0xD2,
			0x16, 0x2E, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34,
			0x56, 0x90, 0x50, 0x10, 0x20, 0x00, 0xA6, 0x33,
			0x00, 0x00, 0x30, 0x31,
			/* CRC */
			0x32, 0x33, 0x34, 0x35
		},
		.len = 72,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 0,
		.no_cipher = false
	},
	.ciphertext = {
		.data = {
			/* XGEM header */
			0x01, 0x00, 0x27, 0x11, 0x00, 0x00, 0x33, 0x0B,
			/* Ethernet frame */
			0xC7, 0x62, 0x82, 0xCA, 0xF6, 0x6F, 0xF5, 0xED,
			0xB7, 0x90, 0x1E, 0x02, 0x6B, 0x2C, 0x08, 0x7D,
			0x3C, 0x90, 0xE8, 0x2C, 0x44, 0x30, 0x03, 0x29,
			0x5F, 0x88, 0xA9, 0xD6, 0x1E, 0xF9, 0xD1, 0xF1,
			0xD6, 0x16, 0x8C, 0x72, 0xA4, 0xCD, 0xD2, 0x8F,
			0x63, 0x26, 0xC9, 0x66, 0xB0, 0x65, 0x24, 0x9B,
			0x60, 0x5B, 0x18, 0x60, 0xBD, 0xD5, 0x06, 0x13,
			0x40, 0xC9, 0x60, 0x64,
			/* CRC */
			0x36, 0x5F, 0x86, 0x8C
		},
		.len = 72,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 0,
		.no_cipher = false
	},
	.output = {
		/* Expected BIP */
		.data = {0xDF, 0xE0, 0xAD, 0xFB},
		.len  = 4
	}
};

struct gpon_test_data gpon_test_case_4 = {
	.key = {
		.data = {
			0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
			0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* XGEM header */
			0x00, 0x39, 0x03, 0xFD, 0x00, 0x00, 0xB3, 0x6A,
			/* Ethernet frame */
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x10, 0x11,
			/* CRC */
			0x20, 0x21, 0x22, 0x23,
			/* Padding */
			0x55, 0x55
		},
		.len = 24,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 2,
		.no_cipher = false
	},
	.ciphertext = {
		.data = {
			/* XGEM header */
			0x00, 0x39, 0x03, 0xFD, 0x00, 0x00, 0xB3, 0x6A,
			/* Ethernet frame */
			0x73, 0xE0, 0x5D, 0x5D, 0x32, 0x9C, 0x3B, 0xFA,
			0x6B, 0x66,
			/* CRC */
			0xF6, 0x8E, 0x5B, 0xD5,
			/* Padding */
			0xAB, 0xCD
		},
		.len = 24,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 2,
		.no_cipher = false
	},
	.output = {
		/* Expected BIP */
		.data = {0x71, 0xF6, 0x8B, 0x73},
		.len  = 4
	}
};

struct gpon_test_data gpon_test_case_5 = {
	.key = {
		.data = {
			0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
			0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* XGEM header */
			0x00, 0x05, 0x03, 0xFD, 0x00, 0x00, 0xB9, 0xB4,
			/* Ethernet frame */
			0x08,
			/* Padding */
			0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55
		},
		.len = 16,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 7,
		.no_cipher = false
	},
	.ciphertext = {
		.data = {
			/* XGEM header */
			0x00, 0x05, 0x03, 0xFD, 0x00, 0x00, 0xB9, 0xB4,
			/* Ethernet frame */
			0x73,
			/* Padding */
			0xBC, 0x02, 0x03, 0x6B, 0xC4, 0x60, 0xA0
		},
		.len = 16,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 7,
		.no_cipher = false
	},
	.output = {
		/* Expected BIP */
		.data = {0x18, 0x7D, 0xD8, 0xEA},
		.len  = 4
	}
};

struct gpon_test_data gpon_test_case_6 = {
	.key = {
		.data = {
			0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
			0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
		},
		.len = 16
	},
	.cipher_iv = {
		.data = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			/* XGEM header */
			0x01, 0x00, 0x27, 0x11, 0x00, 0x00, 0x33, 0x0B,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x81, 0x00, 0x00, 0x01,
			0x08, 0x00, 0x45, 0x00, 0x00, 0x6A, 0xB0, 0x7E,
			0x00, 0x00, 0x04, 0x06, 0x83, 0xBD, 0xC0, 0xA8,
			0x00, 0x01, 0xC0, 0xA8, 0x01, 0x01, 0x04, 0xD2,
			0x16, 0x2E, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34,
			0x56, 0x90, 0x50, 0x10, 0x20, 0x00, 0xA6, 0x33,
			0x00, 0x00, 0x30, 0x31,
			/* CRC */
			0x32, 0x33, 0x34, 0x35
		},
		.len = 72,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 0,
		.no_cipher = true
	},
	.ciphertext = {
		.data = {
			/* XGEM header */
			0x01, 0x00, 0x27, 0x11, 0x00, 0x00, 0x33, 0x0B,
			/* Ethernet frame */
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x81, 0x00, 0x00, 0x01,
			0x08, 0x00, 0x45, 0x00, 0x00, 0x6a, 0xb0, 0x7e,
			0x00, 0x00, 0x04, 0x06, 0x83, 0xbd, 0xc0, 0xa8,
			0x00, 0x01, 0xc0, 0xa8, 0x01, 0x01, 0x04, 0xd2,
			0x16, 0x2e, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34,
			0x56, 0x90, 0x50, 0x10, 0x20, 0x00, 0xa6, 0x33,
			0x00, 0x00, 0x30, 0x31,
			/* CRC */
			0x53, 0xC1, 0xE6, 0x0C
		},
		.len = 72,
		.cipher_offset = 8,
		.crc_offset = 8,
		.bip_offset = 0,
		.padding_len = 0,
		.no_cipher = true
	},
	.output = {
		/* Expected BIP */
		.data = {0x6A, 0xD5, 0xC2, 0xAB},
		.len  = 4
	}
};

#endif /* _AESNI_MB_MFN_RAWDEV_TEST_VECTORS_H_ */
