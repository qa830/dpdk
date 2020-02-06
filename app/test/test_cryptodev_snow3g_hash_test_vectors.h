/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2016-2020 Intel Corporation
 */

#ifndef TEST_CRYPTODEV_SNOW3G_HASH_TEST_VECTORS_H_
#define TEST_CRYPTODEV_SNOW3G_HASH_TEST_VECTORS_H_

struct snow3g_hash_test_data {
	struct {
		uint8_t data[64];
		unsigned len;
	} key;

	struct {
		uint8_t data[64];
		unsigned len;
	} auth_iv;

	struct {
		uint8_t data[2056];
		unsigned len; /* length must be in Bits */
	} plaintext;

	struct {
		unsigned len;
	} validAuthLenInBits;

	struct {
		uint8_t data[64];
		unsigned len;
	} digest;
};

struct snow3g_hash_test_data snow3g_hash_test_case_1 = {
	.key = {
		.data = {
			0x20, 0x9E, 0x9A, 0xA2, 0x98, 0x89, 0xE9, 0x2C,
			0x47, 0xAA, 0xD6, 0x14, 0x94, 0x3D, 0x79, 0xC3
		},
	.len = 16
	},
	.auth_iv = {
		.data = {
			0x7E, 0xEC, 0x06, 0xB1, 0x91, 0xCD, 0xCC, 0x89,
			0xB8, 0x58, 0xE9, 0x60, 0x4A, 0xA7, 0xE7, 0x71
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			0x03, 0x18, 0x60, 0x69, 0xAD, 0xEF, 0xDF, 0x39,
			0x22, 0x1D, 0x1B, 0xC5, 0xB6, 0x3B, 0xB9, 0x95,
			0x55, 0x34, 0x78, 0xBB, 0xA1, 0x5B, 0x20, 0xD6,
			0x71, 0x17, 0x19, 0xD1, 0x2C, 0x00, 0x9B, 0x30,
			0x18, 0xFB, 0x99, 0xC6, 0xEB, 0x79, 0xFF, 0x0D,
			0x97, 0x1A, 0xD2, 0x4D, 0x56, 0x8B, 0xE3, 0xAB
		},
		.len = 384
	},
	.validAuthLenInBits = {
		.len = 384
		},
	.digest = {
		.data = {0x79, 0x9D, 0xA2, 0xF3},
		.len  = 4
	}
};

struct snow3g_hash_test_data snow3g_hash_test_case_2 = {
	.key = {
		.data = {
			0xAC, 0xF7, 0x4F, 0x93, 0x52, 0x4E, 0x11, 0x10,
			0x9B, 0xB8, 0xBB, 0xE0, 0xEE, 0xE8, 0x1E, 0x10
		},
	.len = 16
	},
	.auth_iv = {
		.data = {
			0xD3, 0xF2, 0xA1, 0xFF, 0x6D, 0x22, 0x3D, 0xD6,
			0xF1, 0x3B, 0x4B, 0x5A, 0xD8, 0xB9, 0x12, 0x54
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			0xC6, 0xE1, 0x92, 0x41, 0x3C, 0x9A, 0xBB, 0x24,
			0x6F, 0x3F, 0xF4, 0x42, 0x0F, 0x26, 0xE9, 0x17,
			0x8E, 0xFB, 0x9F, 0xB9, 0x4E, 0x1E, 0x22, 0x10,
			0xF1, 0xD4, 0x5B, 0x0E, 0x1A, 0x9C, 0xE7, 0xE1,
			0x7D, 0x7A, 0x22, 0xBA, 0x15, 0xDE, 0xDE, 0x84,
			0x1E, 0xD3, 0xC6, 0x2D, 0xF9, 0xB0, 0x45, 0x87,
			0xAB, 0xE4, 0x40, 0xF9, 0x02, 0x63, 0x0A, 0xF4,
			0x37, 0x66, 0x03, 0x52, 0x02, 0xEB, 0x34, 0x80,
			0x65, 0x56, 0x3A, 0x7A, 0x34, 0x19, 0xFF, 0x52,
			0xEC, 0xC6, 0x80, 0xE6, 0x76, 0xC5, 0x6E, 0x22,
			0xAA, 0xAE, 0x1B, 0xAD, 0x12, 0x25, 0xA2, 0x4A,
			0x8B, 0xA5, 0x9D, 0x8E, 0x90, 0xD1, 0x0F, 0xF6,
			0x27, 0x49, 0x71, 0x5C, 0x63, 0x71, 0xAF, 0x50,
			0x37, 0x2F, 0x36, 0xAE, 0xF5, 0xA4, 0xD0, 0xA0,
			0x53, 0xEB, 0x4D, 0x65, 0x11, 0xEF, 0xAF, 0x9D,
			0x95, 0x4C, 0x2C, 0x26, 0x1E
		},
		.len = 1000
	},
	.validAuthLenInBits = {
		.len = 1000
	},
	.digest = {
		.data = {0x99, 0x96, 0x4B, 0x52},
		.len  = 4
	}
};

struct snow3g_hash_test_data snow3g_hash_test_case_3 = {
	.key = {
		.data = {
			0x38, 0xED, 0xE7, 0xC8, 0x83, 0x09, 0xB5, 0x64,
			0xCC, 0x2F, 0x11, 0x0B, 0xFB, 0x39, 0x53, 0x28
		},
	.len = 16
	},
	.auth_iv = {
		.data = {
			0x5E, 0x67, 0xB8, 0xB2, 0x9E, 0xDC, 0xDF, 0xA9,
			0xA1, 0xB0, 0x1F, 0x03, 0xE2, 0x08, 0xC5, 0xE8
		},
	.len = 16
	},
	.plaintext = {
		.data = {
			0x7A, 0xB5, 0x1A, 0x1F, 0x92, 0xB5, 0x90, 0xCE,
			0x9A, 0xD1, 0x6A, 0x09, 0x54, 0xA3, 0x84, 0x2A,
			0x47, 0x2C, 0x69, 0x00, 0xCA, 0xBC, 0x61, 0xE4,
			0x7C, 0xDA, 0x71, 0xF7, 0x90, 0x31, 0x01, 0x0B,
			0xE7, 0x1C, 0x2B, 0x79, 0xD1, 0xBC, 0x48, 0x6C,
			0x8D, 0xB2, 0x75, 0xE2, 0x56, 0xF9, 0x0C, 0x9D,
			0x26, 0x75, 0x9D, 0xF0, 0x32, 0xFF, 0xD5, 0xAF,
			0xD9, 0x46, 0xA7, 0x6A, 0x78, 0xA8, 0x75, 0x5F,
			0xC4, 0xA0, 0xD9, 0x96, 0x5C, 0x21, 0x02, 0xEA,
			0xD3, 0x77, 0xCD, 0x29, 0x71, 0xD9, 0xC6, 0x98,
			0x4F, 0x64, 0x89, 0x82, 0x63, 0x5F, 0x31, 0x3C,
			0xA5, 0xD8, 0xA6, 0x1E, 0x80, 0x1C, 0x7D, 0x45,
			0xBC, 0x56, 0xDB, 0x19, 0x77, 0xDD, 0x04, 0x4B,
			0x55, 0xD1, 0x74, 0xC7, 0xAB, 0x3B, 0x60, 0xFB,
			0xA0, 0xE9, 0x7D, 0x03, 0x48, 0xAE, 0x40, 0xEE,
			0x86, 0xE7, 0x0C, 0x07, 0x04, 0x8A, 0x4C, 0xC0,
			0xE0, 0x27, 0xDA, 0x58, 0x05, 0xDF, 0xA3, 0x5B,
			0xB0, 0x18, 0x22, 0x5C, 0x54, 0x83, 0x57, 0xF4,
			0x6C, 0xD4, 0xF8, 0xB5, 0x82, 0x39, 0xA4, 0x09,
			0x21, 0xB0, 0x10, 0x25, 0x3B, 0x5D, 0xE6, 0x1B,
			0x85, 0xC0, 0x74, 0x8A, 0xA0, 0x18, 0xE6, 0x50,
			0x30, 0x09, 0xAD, 0x85, 0x8C, 0x04, 0x7A, 0xF8,
			0xD9, 0x72, 0xAE, 0x5B, 0xAC, 0x52, 0x65, 0xCD,
			0x03, 0x75, 0xF3, 0x3E, 0xD3, 0xD9, 0x5A, 0x58,
			0x9A, 0xCE, 0xE3, 0x3A, 0xE6, 0xC9, 0x8B, 0x17,
			0xD2, 0x38, 0x9C, 0x5E, 0x3D, 0x16, 0x57, 0x16,
			0x89, 0x05, 0x72, 0x36, 0x58, 0xD7, 0x03, 0x5B,
			0x4D, 0xF6, 0x9A, 0x20, 0xD0, 0xF4, 0x79, 0x6A,
			0xC3, 0x5C, 0xA4, 0xA9, 0x25, 0x2F, 0xC0, 0xF8,
			0x68, 0x5D, 0x56, 0xA5, 0x74, 0xAE, 0xBB, 0xFE,
			0xB3, 0x2D, 0x34, 0x0C, 0x05, 0x38, 0x68, 0x52,
			0x2E, 0x02, 0x73, 0xFF, 0xF7, 0xEC, 0x69, 0xBA,
			0x48, 0x0E, 0x64, 0x6E, 0x3E, 0x25, 0x66, 0xA6,
			0x82, 0xBD, 0x4C, 0xF7, 0x6B, 0x08, 0xF5, 0x1F,
			0x35, 0x29, 0x2B, 0x3B, 0x61, 0x93, 0x8D, 0x90,
			0x96, 0x01, 0x8F, 0x8E, 0xED, 0xF9, 0x48, 0x36,
			0x07, 0xAD, 0xA5, 0x46, 0xD2, 0x0B, 0xED, 0x55,
			0xC8, 0x39, 0x4C, 0x33, 0x41, 0x41, 0x52, 0x77,
			0x6B, 0x7E, 0xB2, 0xCD, 0x11, 0x40, 0x5D, 0xA8,
			0x41, 0xED, 0x36, 0x2F, 0xE6, 0x7F, 0x66, 0xEE,
			0x2C, 0x0B, 0x34, 0xFE, 0x17, 0x22, 0x53, 0xDF,
			0x5B, 0xA0, 0x13, 0x9D, 0xE2, 0x66, 0x14, 0x4D,
			0xE5, 0xC7, 0x1B, 0xF7, 0x07, 0x78, 0x9F, 0x49,
			0x65, 0xD5, 0x79, 0x4C, 0x54, 0xE0, 0x3B, 0x81,
			0xEB, 0x70, 0x7F, 0x02, 0x92, 0xD3, 0xE2, 0xED,
			0x73, 0xF6, 0x8B, 0x56, 0x5D, 0xA0, 0xA4, 0x42,
			0x67, 0xBF, 0x39, 0x6F, 0x38, 0xD9, 0xB9, 0x9E,
			0xAF, 0x33, 0xEA, 0x03, 0x13, 0x25, 0x85, 0xFE,
			0x95, 0x04, 0x01, 0x28, 0xD8, 0xE4, 0x15, 0x4C,
			0xDB, 0xA1, 0xA2, 0x38, 0x41, 0x46, 0x7B, 0xA9,
			0x06, 0xB5, 0x19, 0x3E, 0x8E, 0xD2, 0xDC, 0x3D,
			0x05, 0xC7, 0x41, 0x18, 0xED, 0xC6, 0x17, 0x83,
			0xCB, 0x19, 0xAB, 0xA3, 0xFD, 0xC0, 0xEF, 0xD8,
			0x62, 0x92, 0x11, 0xA3, 0xD9, 0x8C, 0x4D, 0xDF,
			0x42, 0x66, 0x1E, 0xD0, 0x38, 0xFA, 0x0D, 0x3E,
			0xC2, 0x4F, 0x57, 0xAF, 0x15, 0x6F, 0x32, 0xE0,
			0x88, 0xDD, 0x84, 0x86, 0x9E, 0x74, 0x5E, 0x00,
			0x06, 0x70, 0xA4, 0xDF, 0xFD, 0xF1, 0xBF, 0x3F,
			0x58, 0xDD, 0x0F, 0x91, 0xD8, 0x1D, 0xCF, 0x9A,
			0x6C, 0x27, 0x49, 0x82, 0x96, 0x7C, 0x62, 0x1E,
			0x59, 0xE7, 0xA4, 0xF7, 0x5B, 0x03, 0xF8, 0x61,
			0x74, 0x9D, 0x41, 0x71, 0x8F, 0x01, 0xB1, 0xE7,
			0xDE, 0xC1, 0x78, 0xB7, 0xDE, 0x48, 0x51, 0x4B,
			0x6F, 0x9B, 0xCD, 0x05, 0x18, 0x30, 0x24, 0x71,
			0x17, 0xC9, 0x69, 0x72, 0xCD, 0x62, 0xD4, 0x42,
			0xFF, 0x15, 0xB3, 0x8E, 0x17, 0x65, 0x75, 0xF5,
			0x26, 0xEE, 0xAD, 0x05, 0x36, 0xFF, 0x50, 0xA6,
			0x9A, 0x1E, 0xAC, 0xB3, 0x4E, 0xD1, 0x24, 0x66,
			0x9A, 0x8E, 0xD9, 0x68, 0xF0, 0xAD, 0xAA, 0xF0,
			0xC3, 0x5E, 0x7E, 0xDA, 0xC3, 0xF4, 0xD0, 0xEA,
			0xE3, 0x7D, 0xEF, 0x19, 0x7C, 0x40, 0xC0, 0x17,
			0x5F, 0x6C, 0xCA, 0xAE, 0x3D, 0xEF, 0x14, 0xD8,
			0x7D, 0xED, 0x40, 0x6E, 0x9A, 0xEB, 0x5E, 0x5E,
			0x49, 0xDD, 0x38, 0x0D, 0xD2, 0x08, 0xF8, 0xB5,
			0x86, 0xE8, 0xCF, 0x02, 0x29, 0x8F, 0x1A, 0x88,
			0xFB, 0xE4, 0x36, 0x38, 0xD3, 0x4B, 0x10, 0x51,
			0x38, 0x51, 0xC0, 0xD3, 0x3C, 0x1E, 0x32, 0x86,
			0xFC, 0x6A, 0x94, 0xCE, 0x73, 0x8C, 0x83, 0xFA,
			0x74, 0x52, 0xFC, 0x9E, 0xE1, 0x16, 0x26, 0xDD,
			0xFB, 0x5D, 0x15, 0xCF, 0xA8, 0x26, 0x20, 0xE1,
			0x77, 0xE0, 0xB4, 0xB4, 0xFF, 0xE6, 0x3A, 0xFC,
			0x51, 0xCE, 0xCA, 0xC5, 0x5B, 0x4D, 0xBF, 0xD0,
			0xA0, 0xBC, 0x6E, 0x82, 0xD2, 0x95, 0x5F, 0xCE,
			0xF2, 0x74, 0x9D, 0x9A, 0x9B, 0xBE, 0x7C, 0x12,
			0x9F, 0x31, 0xC6, 0x9E, 0x18, 0x01, 0x9A, 0x69,
			0xD0, 0x65, 0x2F, 0x2B, 0xB3, 0xEE, 0xFC, 0x53,
			0xAA, 0x6A, 0xD6, 0x7D, 0xFF, 0x35, 0x4B, 0xF2,
			0xAA, 0xE9, 0x8D, 0x45, 0xA7, 0x09, 0x57, 0x46,
			0x3A, 0x1E, 0xE5, 0x52, 0x20, 0x80, 0xBC, 0xF0,
			0xE5, 0xEB, 0x1C, 0x98, 0xD9, 0x18, 0xEC, 0x84,
			0x82, 0xC2, 0x01, 0x82, 0xF7, 0x4D, 0x74, 0xA1,
			0x36, 0x01, 0xE6, 0xDD, 0x0A, 0x3E, 0x24, 0x45,
			0x5D, 0x09, 0x97, 0x7D, 0x89, 0x53, 0x6E, 0x6E,
			0x3E, 0x8A, 0x07, 0x18, 0xA2, 0xF3, 0x9C, 0x24,
			0xB5, 0x9D, 0xA7, 0xAD, 0xEB, 0x1B, 0x4F, 0x21,
			0x1D, 0x35, 0xFF, 0x28, 0x74, 0x23, 0x6D, 0xD2,
			0x2C, 0x04, 0x50, 0xB6, 0x58, 0xBE, 0x24, 0x96,
			0x48, 0x2C, 0xAE, 0xEA, 0x1F, 0x4A, 0x0F, 0xD5,
			0xE8, 0xB6, 0x82, 0xD3, 0xD2, 0xD1, 0xF5, 0xF0,
			0x07, 0xF4, 0x18, 0x7C, 0x17, 0x85, 0x4E, 0x44,
			0x8A, 0x9F, 0xFA, 0xE2, 0x5D, 0x1F, 0x79, 0xA5,
			0x4B, 0x28, 0x90, 0x6B, 0x72, 0x9F, 0x40, 0x5B,
			0x56, 0xC2, 0x2E, 0x29, 0x94, 0x24, 0x19, 0x9C,
			0x18, 0x31, 0x18, 0x30, 0xB7, 0x67, 0x74, 0x42,
			0x06, 0x6F, 0x25, 0x63, 0x8F, 0x9E, 0x09, 0xDA,
			0xC6, 0x99, 0x45, 0x39, 0x38, 0x86, 0x94, 0x8F,
			0x49, 0xC3, 0xB8, 0xDD, 0xE7, 0xD1, 0x79, 0x00,
			0x03, 0x92, 0x30, 0xBB, 0xF9, 0xA5, 0xFD, 0xFF,
			0x14, 0x23, 0x63, 0xA3, 0xC1, 0x6C, 0x7E, 0x88,
			0x05, 0xC3, 0xC2, 0x3E, 0x4A, 0x56, 0xCD, 0x93,
			0x1A, 0x86, 0x71, 0x02, 0x57, 0xEA, 0x02, 0x5B,
			0x7C, 0x33, 0x16, 0x76, 0xD8, 0x14, 0x75, 0xEC,
			0x37, 0xD8, 0x90, 0xF9, 0x45, 0x0E, 0x81, 0x4B,
			0xD2, 0x43, 0x89, 0x1C, 0x9A, 0x57, 0xAF, 0xB5,
			0xDD, 0x20, 0xB8, 0x35, 0x0B, 0xBA, 0x90, 0x88,
			0xEE, 0xA6, 0xFE, 0xC6, 0xBB, 0x74, 0xB2, 0xF2,
			0x4D, 0x43, 0xEB, 0x92, 0x51, 0x6D, 0xDD, 0x23,
			0xB1, 0x66, 0x3F, 0x4C, 0xBE, 0xEF, 0x02, 0x9C,
			0x10, 0xBA, 0xD1, 0x1B, 0x75, 0x62, 0xA3, 0x63,
			0x09, 0xA2, 0x29, 0xC4, 0x16, 0xDC, 0xB6, 0x63,
			0x1F, 0xA2, 0xF5, 0x71, 0x0F, 0xD2, 0x94, 0xC1,
			0x39, 0xD4, 0x0E, 0xF8, 0xC3, 0x10, 0x94, 0xD3,
			0xCA, 0x66, 0xEF, 0x3F, 0xC8, 0x93, 0xA3, 0xD1,
			0x35, 0xCD, 0x95, 0x4B, 0xA9, 0x4C, 0xAF, 0xC9,
			0xEE, 0xA4, 0x3A, 0xFE, 0x77, 0xCF, 0xC0, 0xB1,
			0xA3, 0xCE, 0xAA, 0x67, 0xDE, 0x3E, 0x3A, 0xA9,
			0xA5, 0x2A, 0xE9, 0x6E, 0xBD, 0x8C, 0x3F, 0xF2,
			0x59, 0xD5, 0x3E, 0x03, 0x22, 0xED, 0xCC, 0x11,
			0x92, 0x07, 0x10, 0x0A, 0xD6, 0xD1, 0xBC, 0x79,
			0x9F, 0x66, 0xE1, 0x7E, 0xA5, 0x1B, 0x27, 0x4A,
			0x46, 0x10, 0xB8, 0x03, 0x9C, 0xF8, 0xF6, 0xF6,
			0xCE, 0x34, 0xF9, 0xF1, 0x22, 0xC6, 0x02, 0xB5,
			0xCD, 0x13, 0xBF, 0xA4, 0xE4, 0x7C, 0x1E, 0x84,
			0xE2, 0xFF, 0x02, 0x87, 0x1A, 0x29, 0xD2, 0x61,
			0x3A, 0x8B, 0x64, 0xD7, 0x84, 0x5B, 0xCD, 0x53,
			0x90, 0xC6, 0x44, 0xB2, 0x8D, 0x47, 0x68, 0x5A,
			0x5A, 0x28, 0xFE, 0x3F, 0xA4, 0x1C, 0xC3, 0x86,
			0x1B, 0xC5, 0x0E, 0x36, 0xEF, 0xE1, 0x97, 0x2A,
			0x6C, 0xFC, 0x01, 0xF1, 0x57, 0xCE, 0x44, 0xE7,
			0x95, 0x88, 0x9A, 0x22, 0xD0, 0x02, 0x7D, 0x2B,
			0x2B, 0x7C, 0x6A, 0xCF, 0x99, 0x2D, 0x56, 0xB4,
			0xF3, 0x65, 0xEB, 0xE2, 0x46, 0x83, 0x0C, 0xB3,
			0x80, 0x0E, 0xA4, 0xD7, 0xDD, 0xE9, 0xBF, 0x73,
			0x72, 0x5A, 0x96, 0x42, 0x5D, 0x13, 0x6D, 0x88,
			0x90, 0xD7, 0x58, 0x29, 0x05, 0xAE, 0xDE, 0xF8,
			0x14, 0xC9, 0xDB, 0x5B, 0x4D, 0xE8, 0x0F, 0xCD,
			0xF6, 0xB4, 0xA5, 0xD3, 0x9D, 0x64, 0x47, 0x0F,
			0xBF, 0xDD, 0x52, 0x1C, 0xF1, 0xBF, 0xA4, 0x81,
			0x97, 0xFC, 0xAA, 0x9C, 0xAB, 0x89, 0x95, 0xBF,
			0x53, 0x70, 0x1B, 0xA0, 0x58, 0x2A, 0x6E, 0x4E,
			0xDE, 0x13, 0x22, 0x7B, 0x78, 0x69, 0x8B, 0x37,
			0x47, 0xDD, 0x53, 0x38, 0x9D, 0xF8, 0xBA, 0x35,
			0xF4, 0x65, 0xD2, 0xA0, 0xEE, 0x67, 0x60, 0x41,
			0xD8, 0x7B, 0xE1, 0x30, 0xA5, 0x50, 0x7F, 0x84,
			0x63, 0xA2, 0x00, 0xDC, 0x0C, 0x8B, 0x14, 0x53,
			0x69, 0x67, 0x8C, 0x07, 0x60, 0x46, 0x3D, 0x54,
			0xAB, 0x0F, 0xF5, 0x99, 0x77, 0x55, 0xDB, 0x4F,
			0xD0, 0xBD, 0x80, 0x76, 0x0D, 0xFF, 0xFA, 0x71,
			0xA1, 0xFA, 0x4D, 0xAE, 0x86, 0x61, 0x01, 0xEF,
			0xC9, 0x8D, 0xF7, 0x2A, 0xD4, 0x34, 0x7F, 0x7F,
			0x44, 0x74, 0x19, 0xBB, 0xC9, 0xF4, 0x0B, 0x99,
			0xB1, 0x8B, 0x10, 0xBF, 0x8A, 0x0A, 0x30, 0x2C,
			0x04, 0x7E, 0xDA, 0x8B, 0xE0, 0xDC, 0x7A, 0xAA,
			0x6A, 0x72, 0xD4, 0x3E, 0xA6, 0x53, 0xBD, 0xEB,
			0xC7, 0xD7, 0xA6, 0x90, 0xCC, 0xB1, 0x2A, 0x7E,
			0x3C, 0x3A, 0x3D, 0xC7, 0x45, 0x6D, 0xF3, 0x4A,
			0xEC, 0xCE, 0xD5, 0xCC, 0xAA, 0x50, 0x76, 0x14,
			0xC2, 0x4B, 0x52, 0x69, 0x9E, 0x10, 0x54, 0x66,
			0xE8, 0xFA, 0xF7, 0xB4, 0xAC, 0x22, 0x32, 0xE9,
			0x5D, 0x70, 0xB0, 0xA2, 0xDE, 0xA4, 0xEC, 0xCA,
			0x73, 0xC1, 0x96, 0x1D, 0x11, 0x0D, 0x32, 0xD4,
			0x59, 0x85, 0x3D, 0xF8, 0x96, 0x91, 0x5E, 0x7E,
			0x8C, 0x55, 0x33, 0x39, 0x78, 0x65, 0x22, 0xD5,
			0xD5, 0xD3, 0x77, 0xB3, 0x77, 0x64, 0x7E, 0xEB,
			0x25, 0x15, 0x08, 0x37, 0x23, 0x3B, 0x0B, 0x7C,
			0xC1, 0x49, 0x74, 0x57, 0xDB, 0xD2, 0xD6, 0x67,
			0x28, 0x09, 0xA0, 0xA0, 0x6F, 0xC3, 0x75, 0x45,
			0x96, 0xED, 0xF9, 0x0E, 0x51, 0x77, 0xF9, 0x77,
			0x8C, 0x02, 0xAF, 0xAF, 0x3E, 0xBB, 0x2B, 0xFF,
			0x04, 0x9F, 0x56, 0xDF, 0x72, 0x2C, 0x47, 0x9B,
			0x36, 0xE8, 0x3C, 0xA5, 0xAB, 0xB1, 0xEA, 0x42,
			0x9F, 0xE3, 0x51, 0xF1, 0x5A, 0x4A, 0x68, 0xE7,
			0x4D, 0x18, 0x96, 0x8B, 0xD3, 0xC2, 0x8A, 0xD8,
			0x62, 0xE1, 0xB8, 0xD5, 0x0D, 0xFF, 0x70, 0x43,
			0xE8, 0xAC, 0xE9, 0x93, 0x5E, 0xD3, 0xD6, 0xFE,
			0xB7, 0x27, 0xEF, 0x12, 0x72, 0x57, 0xF9, 0xBF,
			0x6F, 0x90, 0x4B, 0x43, 0x52, 0xD5, 0x1B, 0xB4,
			0xB6, 0xD3, 0x8A, 0xC4, 0xD3, 0xFA, 0x08, 0xBB,
			0xA7, 0xF2, 0x4F, 0x05, 0xC5, 0x25, 0x03, 0x7D,
			0x4D, 0xF2, 0x8F, 0xC0, 0x4A, 0x88, 0x80, 0xBA,
			0x18, 0xCB, 0xFD, 0x6B, 0xA1, 0x19, 0x20, 0x58,
			0xED, 0xAA, 0x1D, 0xC0, 0xA4, 0x26, 0x7C, 0x4C,
			0x18, 0xCB, 0x52, 0xDE, 0xF1, 0x55, 0x5B, 0x3F,
			0x48, 0xEA, 0xFF, 0x93, 0x73, 0x80, 0x4E, 0x8C,
			0x4B, 0x4C, 0xF7, 0xED, 0x65, 0x17, 0x46, 0x52,
			0xC1, 0x63, 0x13, 0x66, 0x89, 0x90, 0xB2, 0xA1,
			0x5C, 0x04, 0x80, 0x4E, 0x5A, 0xDB, 0x8D, 0xA3,
			0xC6, 0x8D, 0x37, 0x39, 0x0D, 0x85, 0xC6, 0x59,
			0xD1, 0xBD, 0x47, 0x37, 0xD5, 0x8D, 0x8A, 0x97,
			0xF0, 0x9E, 0xFE, 0x7A, 0x2E, 0xB1, 0x1C, 0x8A,
			0xB6, 0x9C, 0xD8, 0x10, 0x77, 0x66, 0xB4, 0x3E,
			0xF4, 0xEC, 0x78, 0x02, 0x71, 0x3E, 0x5C, 0x43,
			0xFC, 0xA3, 0x7A, 0xD1, 0x30, 0x05, 0x69, 0x21,
			0xA3, 0x67, 0x9B, 0xD1, 0x18, 0xB7, 0x5C, 0xCE,
			0x53, 0x35, 0xDF, 0xCB, 0x9B, 0x94, 0x09, 0x90,
			0x80, 0x81, 0x92, 0xF2, 0xBF, 0xEE, 0x35, 0xBB,
			0x92, 0xB0, 0x8D, 0xC2, 0xB5, 0xF6, 0xE4, 0x59,
			0x5D, 0x7F, 0x2A, 0x76, 0x37, 0x87, 0x44, 0x8B,
			0xBC, 0x24, 0x56, 0x58, 0xB8, 0x60, 0xE8, 0x39,
			0xE1, 0x7B, 0x2B, 0xA1, 0x6A, 0x61, 0x5D, 0xFC,
			0x12, 0xEA, 0xBF, 0xC8, 0xE1, 0xA3, 0x21, 0x3E,
			0x23, 0x4B, 0xB5, 0x5B, 0xD3, 0xFA, 0xE6, 0x8F,
			0x1E, 0x3D, 0xE8, 0xD7, 0x9D, 0xD1, 0x11, 0x7E,
			0x4C, 0x3C, 0x1F, 0xB6, 0x9E, 0x7C, 0xB3, 0xB0,
			0x67, 0x72, 0x78, 0x48, 0x16, 0x99, 0x87, 0x3A,
			0xE5, 0x3C, 0x95, 0xB8, 0x37, 0x7C, 0x48, 0x56,
			0xB9, 0x30, 0x2D, 0x56, 0x01, 0x3E, 0xD5, 0x4E,
			0x7B, 0xF5, 0x04, 0x19, 0x72, 0xB8, 0xC9, 0xD9,
			0x2B, 0x42, 0x22, 0x41, 0xDB, 0xA9, 0x7C, 0xC1,
			0xE6, 0x12, 0x79, 0x1D, 0x8E, 0xC2, 0x73, 0x47,
			0xF3, 0xA1, 0x9E, 0xF5, 0xE0, 0x73, 0x43, 0x5B,
			0x68, 0x48, 0x74, 0xDB, 0x00, 0x3E, 0xB4, 0x2B,
			0x80, 0xD6, 0x6D, 0x5C, 0x80, 0xE9, 0x1D, 0x67,
			0xFC, 0x97, 0x84, 0x8A, 0x59, 0xF8, 0xD2, 0x4D,
			0x99, 0x70, 0x42, 0x7A, 0xE4, 0x85, 0xD5, 0x4D,
			0xCD, 0x4A, 0x28, 0xCE, 0x89, 0xDD, 0xFA, 0x09,
			0xB3, 0x67, 0x66, 0x34, 0x51, 0x83, 0x9B, 0x4D,
			0x1B, 0x20, 0xD8, 0x75, 0x19, 0xAA, 0xC2, 0xB3,
			0x1B, 0x04, 0x2D, 0x00, 0x8A, 0x02, 0x4D, 0x57,
			0x4D, 0x75, 0x26, 0xD6, 0x53, 0x20, 0xE0, 0x06,
			0x88, 0x46, 0x3B, 0xD9, 0xCA, 0xD7, 0x27, 0xE5,
			0xF7, 0xFF, 0x5A, 0x10, 0xAA, 0x1C, 0xC3, 0xC6,
			0x21, 0xF1, 0xC6, 0xAB, 0xF3, 0x13, 0x03, 0x40,
			0x89, 0x29, 0x16, 0xDC, 0x49, 0xF6, 0xE3, 0xD1,
			0x3D, 0x1F, 0xAB, 0x07, 0xF6, 0xD2, 0xEC, 0xED,
			0xD2, 0x47, 0xFE, 0x7D, 0x64, 0xC2, 0x43, 0x85,
			0xB3, 0x09, 0x31, 0xA7, 0x1D, 0x34, 0xE7, 0xA6,
			0x5E, 0xFE, 0x83, 0xA7, 0xF5, 0x67, 0x79, 0x33,
			0x86, 0x25, 0x3A, 0x7C, 0xF8, 0x27, 0x6A, 0xCB,
			0x6E, 0x68, 0x48, 0xD2, 0x2B, 0x8B, 0x58, 0xDE,
			0x95, 0x89, 0x85, 0xB2, 0xBE, 0x6D, 0x58, 0x1C,
			0x6C, 0xDC, 0xC4, 0x62, 0x43, 0x3E, 0x95, 0xC9,
			0x63, 0xD0, 0x46, 0x5B, 0xF7, 0xB0, 0x27, 0x66,
			0x19, 0x6F, 0x39, 0x44, 0xFA, 0x91, 0x23, 0x8F,
			0x1B, 0xA9, 0x42, 0xD9, 0x17, 0x9B, 0xF6, 0x84,
			0x77, 0xBA, 0xE6, 0xBB, 0xF8, 0x7C, 0x85, 0x5B,
			0x4D, 0xCB, 0xB7, 0x44, 0x7B, 0xDE, 0xAB, 0x95,
			0x4D, 0xE4, 0xD9, 0x48, 0x76, 0xFD, 0xD8, 0x91,
			0xA6, 0x1A, 0x6B, 0xBD, 0xB6, 0x61, 0x42, 0x2D,
			0x1C, 0x28, 0xE8, 0x14, 0xA5, 0x6D, 0x70, 0xF2,
			0x39, 0x27, 0x37, 0xB4, 0x06, 0xE2, 0x4A, 0x54,
			0xC7, 0x23, 0x9D, 0x3D, 0x21, 0x76, 0xCF, 0xC7,
			0x91, 0x3B, 0x85, 0x47, 0x9C, 0xC7, 0x74, 0xB9,
			0xF0, 0x5D, 0xCD, 0x96, 0xCB, 0x3D, 0x88, 0x04,
			0x65, 0xC0, 0xB9, 0x6B, 0xA2, 0x03, 0xC0, 0x6A,
			0x27, 0x5D, 0xA7, 0x48, 0xD3, 0x77, 0x10, 0x64,
			0xB2, 0x96, 0xAB, 0x4F, 0x5E, 0x20, 0x08, 0x4E
		},
	.len = 16448
	},
	.validAuthLenInBits = {
		.len = 16448
	},
	.digest = {
		.data = {0x26, 0xA4, 0x0E, 0xBD},
		.len  = 4
	}
};

struct snow3g_hash_test_data snow3g_hash_test_case_4 = {
	.key = {
		.data = {
			0x92, 0x3B, 0xBF, 0x90, 0xF3, 0x75, 0x33, 0x51,
			0x4B, 0x41, 0x12, 0xC9, 0xA6, 0x84, 0x5E, 0xF9
		},
	.len = 16
	},
	.auth_iv = {
		.data = {
			0x93, 0x9E, 0x5D, 0x4D, 0x7C, 0x01, 0x39, 0x61,
			0x6E, 0x58, 0xA3, 0x6A, 0xF4, 0xE0, 0xCF, 0xB0
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			0x86, 0xAB, 0x33, 0x54, 0xC4, 0x71, 0xB4, 0xA2,
			0x4D, 0x49, 0xC0, 0x35, 0x61, 0xF5, 0x3A, 0xC2,
			0x24, 0xEE, 0x2E, 0x2C, 0xD5, 0xE6, 0x7D, 0x65
		},
		.len = 189
	},
	.validAuthLenInBits = {
		.len = 189
		},
	.digest = {
		.data = {0x1D, 0x4F, 0x18, 0xBD},
		.len  = 4
	}
};

struct snow3g_hash_test_data snow3g_hash_test_case_5 = {
	.key = {
		.data = {
			0xC1, 0xC7, 0x8A, 0x73, 0x35, 0xF8, 0x7A, 0xDA,
			0xAF, 0x23, 0xE6, 0x4B, 0x4B, 0x48, 0x97, 0x9A
		},
		.len = 16
	},
	.auth_iv = {
		.data = {
			0xE9, 0xC3, 0xDC, 0x60, 0xD2, 0xCD, 0x26, 0x21,
			0x86, 0x27, 0xF7, 0xC6, 0x37, 0x1B, 0x0D, 0xE0
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			0xFD, 0x63, 0x62, 0x92, 0xA7, 0x81, 0x41, 0x13,
			0x13, 0xFE, 0x0D, 0x63, 0x37, 0x1C, 0xF5, 0x76,
			0xF3, 0x63, 0x5C, 0x59, 0x25, 0xA1, 0xF9, 0xC4,
			0x61, 0x79, 0x81, 0x94, 0x46, 0x3B, 0x4C, 0x40
		},
		.len = 254
	},
	.validAuthLenInBits = {
		.len = 254
		},
	.digest = {
		.data = {0xBD, 0x18, 0xE3, 0xCD},
		.len  = 4
	}
};

struct snow3g_hash_test_data snow3g_hash_test_case_6 = {
	.key = {
		.data = {
			0xA6, 0x42, 0x51, 0xBC, 0x4C, 0x60, 0x73, 0x69,
			0x8B, 0x18, 0x2D, 0xFE, 0xF0, 0x49, 0xD9, 0x70
		},
		.len = 16
	},
	.auth_iv = {
		.data = {
			0x4E, 0x3D, 0x23, 0x28, 0xE8, 0x35, 0x9E, 0xAF,
			0xE2, 0x9C, 0xBD, 0xF9, 0xDA, 0x1A, 0xCF, 0x34
		},
		.len = 16
	},
	.plaintext = {
		.data = {
			0xCF, 0x03, 0x5D, 0x21, 0x78, 0xC8, 0x29, 0xCD,
			0x25, 0xC7, 0x5A, 0xB8, 0xF3, 0xCF, 0xAB, 0x9D,
			0x6E, 0x17, 0x39, 0xAD, 0x84, 0xA7, 0x27, 0xFE,
			0x70, 0xC6, 0xE7, 0x03, 0x4E, 0x8F, 0x99, 0x1E,
			0x92, 0xF6, 0x3F, 0x0B, 0xBF, 0x68, 0xD8, 0xE2
		},
		.len = 319
	},
	.validAuthLenInBits = {
		.len = 319
		},
	.digest = {
		.data = {0x8B, 0x6E, 0xE2, 0xE8},
		.len  = 4
	}
};
#endif /* TEST_CRYPTODEV_SNOW3G_HASH_TEST_VECTORS_H_ */
