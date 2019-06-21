/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2019 Intel Corporation
 */

#ifndef _ICE_GENERIC_FLOW_H_
#define _ICE_GENERIC_FLOW_H_

#include <rte_flow_driver.h>

struct ice_flow_pattern {
	enum rte_flow_item_type *items;
	uint64_t sw_fields;
};

#define ICE_INSET_NONE            0x00000000000000000ULL

/* bit0 ~ bit 7 */
#define ICE_INSET_SMAC            0x0000000000000001ULL
#define ICE_INSET_DMAC            0x0000000000000002ULL
#define ICE_INSET_ETHERTYPE       0x0000000000000020ULL

/* bit 8 ~ bit 15 */
#define ICE_INSET_IPV4_SRC        0x0000000000000100ULL
#define ICE_INSET_IPV4_DST        0x0000000000000200ULL
#define ICE_INSET_IPV6_SRC        0x0000000000000400ULL
#define ICE_INSET_IPV6_DST        0x0000000000000800ULL
#define ICE_INSET_SRC_PORT        0x0000000000001000ULL
#define ICE_INSET_DST_PORT        0x0000000000002000ULL
#define ICE_INSET_ARP             0x0000000000004000ULL

/* bit 16 ~ bit 31 */
#define ICE_INSET_IPV4_TOS        0x0000000000010000ULL
#define ICE_INSET_IPV4_PROTO      0x0000000000020000ULL
#define ICE_INSET_IPV4_TTL        0x0000000000040000ULL
#define ICE_INSET_IPV6_PROTO      0x0000000000200000ULL
#define ICE_INSET_IPV6_HOP_LIMIT  0x0000000000400000ULL
#define ICE_INSET_ICMP            0x0000000001000000ULL
#define ICE_INSET_ICMP6           0x0000000002000000ULL

/* bit 32 ~ bit 47, tunnel fields */
#define ICE_INSET_TUN_SMAC           0x0000000100000000ULL
#define ICE_INSET_TUN_DMAC           0x0000000200000000ULL
#define ICE_INSET_TUN_IPV4_SRC       0x0000000400000000ULL
#define ICE_INSET_TUN_IPV4_DST       0x0000000800000000ULL
#define ICE_INSET_TUN_IPV4_TTL       0x0000001000000000ULL
#define ICE_INSET_TUN_IPV4_PROTO     0x0000002000000000ULL
#define ICE_INSET_TUN_IPV6_SRC       0x0000004000000000ULL
#define ICE_INSET_TUN_IPV6_DST       0x0000008000000000ULL
#define ICE_INSET_TUN_IPV6_TTL       0x0000010000000000ULL
#define ICE_INSET_TUN_IPV6_PROTO     0x0000020000000000ULL
#define ICE_INSET_TUN_SRC_PORT       0x0000040000000000ULL
#define ICE_INSET_TUN_DST_PORT       0x0000080000000000ULL
#define ICE_INSET_TUN_ID             0x0000100000000000ULL

/* bit 48 ~ bit 55 */
#define ICE_INSET_LAST_ETHER_TYPE 0x0001000000000000ULL

#define ICE_FLAG_VLAN_INNER  0x00000001ULL
#define ICE_FLAG_VLAN_OUTER  0x00000002ULL

#define INSET_ETHER ( \
	ICE_INSET_DMAC | ICE_INSET_SMAC | ICE_INSET_ETHERTYPE)
#define INSET_MAC_IPV4 ( \
	ICE_INSET_IPV4_DST | ICE_INSET_IPV4_SRC | \
	ICE_INSET_IPV4_PROTO | ICE_INSET_IPV4_TOS)
#define INSET_MAC_IPV4_L4 ( \
	ICE_INSET_IPV4_DST | ICE_INSET_IPV4_SRC | \
	ICE_INSET_IPV4_TOS | ICE_INSET_DST_PORT | \
	ICE_INSET_SRC_PORT)
#define INSET_MAC_IPV4_ICMP ( \
	ICE_INSET_IPV4_DST | ICE_INSET_IPV4_SRC | \
	ICE_INSET_IPV4_TOS | ICE_INSET_ICMP)
#define INSET_MAC_IPV6 ( \
	ICE_INSET_IPV6_DST | ICE_INSET_IPV6_SRC | \
	ICE_INSET_IPV6_PROTO | ICE_INSET_IPV6_HOP_LIMIT)
#define INSET_MAC_IPV6_L4 ( \
	ICE_INSET_IPV6_DST | ICE_INSET_IPV6_SRC | \
	ICE_INSET_IPV6_HOP_LIMIT | ICE_INSET_DST_PORT | \
	ICE_INSET_SRC_PORT)
#define INSET_MAC_IPV6_ICMP ( \
	ICE_INSET_IPV6_DST | ICE_INSET_IPV6_SRC | \
	ICE_INSET_IPV6_HOP_LIMIT | ICE_INSET_ICMP6)
#define INSET_TUNNEL_IPV4_TYPE1 ( \
	ICE_INSET_TUN_IPV4_SRC | ICE_INSET_TUN_IPV4_DST | \
	ICE_INSET_TUN_IPV4_TTL | ICE_INSET_TUN_IPV4_PROTO)
#define INSET_TUNNEL_IPV4_TYPE2 ( \
	ICE_INSET_TUN_IPV4_SRC | ICE_INSET_TUN_IPV4_DST | \
	ICE_INSET_TUN_IPV4_TTL | ICE_INSET_TUN_IPV4_PROTO | \
	ICE_INSET_TUN_SRC_PORT | ICE_INSET_TUN_DST_PORT)
#define INSET_TUNNEL_IPV4_TYPE3 ( \
	ICE_INSET_TUN_IPV4_SRC | ICE_INSET_TUN_IPV4_DST | \
	ICE_INSET_TUN_IPV4_TTL | ICE_INSET_ICMP)
#define INSET_TUNNEL_IPV6_TYPE1 ( \
	ICE_INSET_TUN_IPV6_SRC | ICE_INSET_TUN_IPV6_DST | \
	ICE_INSET_TUN_IPV6_TTL | ICE_INSET_TUN_IPV6_PROTO)
#define INSET_TUNNEL_IPV6_TYPE2 ( \
	ICE_INSET_TUN_IPV6_SRC | ICE_INSET_TUN_IPV6_DST | \
	ICE_INSET_TUN_IPV6_TTL | ICE_INSET_TUN_IPV6_PROTO | \
	ICE_INSET_TUN_SRC_PORT | ICE_INSET_TUN_DST_PORT)
#define INSET_TUNNEL_IPV6_TYPE3 ( \
	ICE_INSET_TUN_IPV6_SRC | ICE_INSET_TUN_IPV6_DST | \
	ICE_INSET_TUN_IPV6_TTL | ICE_INSET_ICMP6)

/* L2 */
static enum rte_flow_item_type pattern_ethertype[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_END,
};

/* non-tunnel IPv4 */
static enum rte_flow_item_type pattern_ipv4[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_icmp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_ICMP,
	RTE_FLOW_ITEM_TYPE_END,
};

/* non-tunnel IPv6 */
static enum rte_flow_item_type pattern_ipv6[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv6_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv6_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv6_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv6_icmp6[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_ICMP6,
	RTE_FLOW_ITEM_TYPE_END,
};

/* IPv4 VXLAN IPv4 */
static enum rte_flow_item_type pattern_ipv4_vxlan_ipv4[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_ipv4_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_ipv4_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_ipv4_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_ipv4_icmp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_ICMP,
	RTE_FLOW_ITEM_TYPE_END,
};

/* IPv4 VXLAN MAC IPv4 */
static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv4[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv4_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv4_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv4_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv4_icmp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_ICMP,
	RTE_FLOW_ITEM_TYPE_END,
};

/* IPv4 VXLAN IPv6 */
static enum rte_flow_item_type pattern_ipv4_vxlan_ipv6[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_ipv6_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_ipv6_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_ipv6_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_ipv6_icmp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_ICMP,
	RTE_FLOW_ITEM_TYPE_END,
};

/* IPv4 VXLAN MAC IPv6 */
static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv6[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv6_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv6_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv6_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_vxlan_eth_ipv6_icmp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_ICMP,
	RTE_FLOW_ITEM_TYPE_END,
};

/* IPv4 NVGRE IPv4 */
static enum rte_flow_item_type pattern_ipv4_nvgre_ipv4[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_ipv4_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_ipv4_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_ipv4_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_ipv4_icmp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_ICMP,
	RTE_FLOW_ITEM_TYPE_END,
};

/* IPv4 NVGRE MAC IPv4 */
static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv4[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv4_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv4_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv4_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv4_icmp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_ICMP,
	RTE_FLOW_ITEM_TYPE_END,
};

/* IPv4 NVGRE IPv6 */
static enum rte_flow_item_type pattern_ipv4_nvgre_ipv6[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_ipv6_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_ipv6_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_ipv6_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_ipv6_icmp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_ICMP,
	RTE_FLOW_ITEM_TYPE_END,
};

/* IPv4 NVGRE MAC IPv6 */
static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv6[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv6_udp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv6_tcp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_TCP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv6_sctp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_SCTP,
	RTE_FLOW_ITEM_TYPE_END,
};

static enum rte_flow_item_type pattern_ipv4_nvgre_eth_ipv6_icmp[] = {
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV4,
	RTE_FLOW_ITEM_TYPE_UDP,
	RTE_FLOW_ITEM_TYPE_VXLAN,
	RTE_FLOW_ITEM_TYPE_ETH,
	RTE_FLOW_ITEM_TYPE_IPV6,
	RTE_FLOW_ITEM_TYPE_ICMP,
	RTE_FLOW_ITEM_TYPE_END,
};

static struct ice_flow_pattern ice_supported_patterns[] = {
	{pattern_ethertype, INSET_ETHER},
	{pattern_ipv4, INSET_MAC_IPV4},
	{pattern_ipv4_udp, INSET_MAC_IPV4_L4},
	{pattern_ipv4_sctp, INSET_MAC_IPV4_L4},
	{pattern_ipv4_tcp, INSET_MAC_IPV4_L4},
	{pattern_ipv4_icmp, INSET_MAC_IPV4_ICMP},
	{pattern_ipv6, INSET_MAC_IPV6},
	{pattern_ipv6_udp, INSET_MAC_IPV6_L4},
	{pattern_ipv6_sctp, INSET_MAC_IPV6_L4},
	{pattern_ipv6_tcp, INSET_MAC_IPV6_L4},
	{pattern_ipv6_icmp6, INSET_MAC_IPV6_ICMP},
	{pattern_ipv4_vxlan_ipv4, INSET_TUNNEL_IPV4_TYPE1},
	{pattern_ipv4_vxlan_ipv4_udp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_vxlan_ipv4_tcp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_vxlan_ipv4_sctp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_vxlan_ipv4_icmp, INSET_TUNNEL_IPV4_TYPE3},
	{pattern_ipv4_vxlan_eth_ipv4, INSET_TUNNEL_IPV4_TYPE1},
	{pattern_ipv4_vxlan_eth_ipv4_udp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_vxlan_eth_ipv4_tcp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_vxlan_eth_ipv4_sctp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_vxlan_eth_ipv4_icmp, INSET_TUNNEL_IPV4_TYPE3},
	{pattern_ipv4_vxlan_ipv6, INSET_TUNNEL_IPV6_TYPE1},
	{pattern_ipv4_vxlan_ipv6_udp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_vxlan_ipv6_tcp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_vxlan_ipv6_sctp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_vxlan_ipv6_icmp, INSET_TUNNEL_IPV6_TYPE3},
	{pattern_ipv4_vxlan_eth_ipv6, INSET_TUNNEL_IPV6_TYPE1},
	{pattern_ipv4_vxlan_eth_ipv6_udp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_vxlan_eth_ipv6_tcp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_vxlan_eth_ipv6_sctp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_vxlan_eth_ipv6_icmp, INSET_TUNNEL_IPV6_TYPE3},
	{pattern_ipv4_nvgre_ipv4, INSET_TUNNEL_IPV4_TYPE1},
	{pattern_ipv4_nvgre_ipv4_udp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_nvgre_ipv4_tcp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_nvgre_ipv4_sctp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_nvgre_ipv4_icmp, INSET_TUNNEL_IPV4_TYPE3},
	{pattern_ipv4_nvgre_eth_ipv4, INSET_TUNNEL_IPV4_TYPE1},
	{pattern_ipv4_nvgre_eth_ipv4_udp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_nvgre_eth_ipv4_tcp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_nvgre_eth_ipv4_sctp, INSET_TUNNEL_IPV4_TYPE2},
	{pattern_ipv4_nvgre_eth_ipv4_icmp, INSET_TUNNEL_IPV4_TYPE3},
	{pattern_ipv4_nvgre_ipv6, INSET_TUNNEL_IPV6_TYPE1},
	{pattern_ipv4_nvgre_ipv6_udp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_nvgre_ipv6_tcp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_nvgre_ipv6_sctp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_nvgre_ipv6_icmp, INSET_TUNNEL_IPV6_TYPE3},
	{pattern_ipv4_nvgre_eth_ipv6, INSET_TUNNEL_IPV6_TYPE1},
	{pattern_ipv4_nvgre_eth_ipv6_udp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_nvgre_eth_ipv6_tcp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_nvgre_eth_ipv6_sctp, INSET_TUNNEL_IPV6_TYPE2},
	{pattern_ipv4_nvgre_eth_ipv6_icmp, INSET_TUNNEL_IPV6_TYPE3},
};

#endif
