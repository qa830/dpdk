/* SPDX-License-Identifier: BSD-3-Clause
 *
 * This file contain the application main file
 * This application provides the user the ability to test the
 * insertion rate for specific rte_flow rule under stress state ~4M rule/
 *
 * Then it will also provide packet per second measurement after installing
 * all rules, the user may send traffic to test the PPS that match the rules
 * after all rules are installed, to check performance or functionality after
 * the stress.
 *
 * The flows insertion will go for all ports first, then it will print the
 * results, after that the application will go into forwarding packets mode
 * it will start receiving traffic if any and then forwarding it back and
 * gives packet per second measurement.
 *
 * Copyright 2020 Mellanox Technologies, Ltd
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>


#include <rte_eal.h>
#include <rte_common.h>
#include <rte_malloc.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_net.h>
#include <rte_flow.h>
#include <rte_cycles.h>
#include <rte_memory.h>

#include "flow_gen.h"
#include "user_parameters.h"

#define MAX_ITERATIONS 100

struct rte_flow *flow;
static uint8_t flow_group;

static uint16_t flow_items;
static uint16_t flow_actions;
static uint8_t flow_attrs;
static volatile bool force_quit;
static volatile bool dump_iterations;
static volatile bool dump_socket_mem_flag;
static volatile bool delete_flag;
static struct rte_mempool *mbuf_mp;
static uint32_t nb_lcores;
static uint32_t flows_count;
static uint32_t iterations_number;

static void usage(char *progname)
{
	printf("\nusage: %s", progname);
	printf("\nControl configurations:\n");
	printf("  --flows-count=N: to set the number of needed"
		" flows to insert, default is 4,000,000\n");
	printf("  --dump-iterations: To print rates for each"
		" iteration\n");
	printf("  --deletion-rate: Enable deletion rate"
		" calculations\n");
	printf("  --dump-socket-mem: to dump all socket memory\n");

	printf("To set flow attributes:\n");
	printf("  --ingress: set ingress attribute in flows\n");
	printf("  --egress: set egress attribute in flows\n");
	printf("  --transfer: set transfer attribute in flows\n");
	printf("  --group=N: set group for all flows,"
		" default is 0\n");

	printf("To set flow items:\n");
	printf("  --ether: add ether layer in flow items\n");
	printf("  --vlan: add vlan layer in flow items\n");
	printf("  --ipv4: add ipv4 layer in flow items\n");
	printf("  --ipv6: add ipv6 layer in flow items\n");
	printf("  --tcp: add tcp layer in flow items\n");
	printf("  --udp: add udp layer in flow items\n");
	printf("  --vxlan: add vxlan layer in flow items\n");
	printf("  --vxlan-gpe: add vxlan-gpe layer in flow items\n");
	printf("  --gre: add gre layer in flow items\n");
	printf("  --geneve: add geneve layer in flow items\n");
	printf("  --gtp: add gtp layer in flow items\n");
	printf("  --meta: add meta layer in flow items\n");
	printf("  --tag: add tag layer in flow items\n");

	printf("To set flow actions:\n");
	printf("  --port-id: add port-id action in flow actions\n");
	printf("  --rss: add rss action in flow actions\n");
	printf("  --queue: add queue action in flow actions\n");
	printf("  --jump: add jump action in flow actions\n");
	printf("  --mark: add mark action in flow actions\n");
	printf("  --count: add count action in flow actions\n");
	printf("  --set-meta: add set meta action in flow actions\n");
	printf("  --set-tag: add set tag action in flow actions\n");
	printf("  --drop: add drop action in flow actions\n");
	printf("  --hairpin-queue: add hairpin-queue action in flow actions\n");
	printf("  --hairpin-rss: add hairping-rss action in flow actions\n");
}

static void
args_parse(int argc, char **argv)
{
	char **argvopt;
	int n, opt;
	int opt_idx;
	static struct option lgopts[] = {
		/* Control */
		{ "help",                       0, 0, 0 },
		{ "flows-count",                1, 0, 0 },
		{ "dump-iterations",            0, 0, 0 },
		{ "deletion-rate",              0, 0, 0 },
		{ "dump-socket-mem",            0, 0, 0 },
		/* Attributes */
		{ "ingress",                    0, 0, 0 },
		{ "egress",                     0, 0, 0 },
		{ "transfer",                   0, 0, 0 },
		{ "group",                      1, 0, 0 },
		/* Items */
		{ "ether",                      0, 0, 0 },
		{ "vlan",                       0, 0, 0 },
		{ "ipv4",                       0, 0, 0 },
		{ "ipv6",                       0, 0, 0 },
		{ "tcp",                        0, 0, 0 },
		{ "udp",                        0, 0, 0 },
		{ "vxlan",                      0, 0, 0 },
		{ "vxlan-gpe",                  0, 0, 0 },
		{ "gre",                        0, 0, 0 },
		{ "geneve",                     0, 0, 0 },
		{ "gtp",                        0, 0, 0 },
		{ "meta",                       0, 0, 0 },
		{ "tag",                        0, 0, 0 },
		/* Actions */
		{ "port-id",                    0, 0, 0 },
		{ "rss",                        0, 0, 0 },
		{ "queue",                      0, 0, 0 },
		{ "jump",                       0, 0, 0 },
		{ "mark",                       0, 0, 0 },
		{ "count",                      0, 0, 0 },
		{ "set-meta",                   0, 0, 0 },
		{ "set-tag",                    0, 0, 0 },
		{ "drop",                       0, 0, 0 },
		{ "hairpin-queue",              0, 0, 0 },
		{ "hairpin-rss",                0, 0, 0 },
	};

	flow_items = 0;
	flow_actions = 0;
	flow_attrs = 0;
	argvopt = argv;

	printf(":: Flow -> ");
	while ((opt = getopt_long(argc, argvopt, "",
				lgopts, &opt_idx)) != EOF) {
		switch (opt) {
		case 0:
			if (!strcmp(lgopts[opt_idx].name, "help")) {
				usage(argv[0]);
				rte_exit(EXIT_SUCCESS, "Displayed help\n");
			}
			/* Attributes */
			if (!strcmp(lgopts[opt_idx].name, "ingress")) {
				flow_attrs |= INGRESS;
				printf("ingress ");
			}
			if (!strcmp(lgopts[opt_idx].name, "egress")) {
				flow_attrs |= EGRESS;
				printf("egress ");
			}
			if (!strcmp(lgopts[opt_idx].name, "transfer")) {
				flow_attrs |= TRANSFER;
				printf("transfer ");
			}
			if (!strcmp(lgopts[opt_idx].name, "group")) {
				n = atoi(optarg);
				if (n >= 0)
					flow_group = n;
				else
					rte_exit(EXIT_SUCCESS,
						"flow group should be >= 0");
				printf("group %d ", flow_group);
			}
			/* Items */
			if (!strcmp(lgopts[opt_idx].name, "ether")) {
				flow_items |= ETH_ITEM;
				printf("ether / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "ipv4")) {
				flow_items |= IPV4_ITEM;
				printf("ipv4 / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "vlan")) {
				flow_items |= VLAN_ITEM;
				printf("vlan / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "ipv6")) {
				flow_items |= IPV6_ITEM;
				printf("ipv6 / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "tcp")) {
				flow_items |= TCP_ITEM;
				printf("tcp / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "udp")) {
				flow_items |= UDP_ITEM;
				printf("udp / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "vxlan")) {
				flow_items |= VXLAN_ITEM;
				printf("vxlan / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "vxlan-gpe")) {
				flow_items |= VXLAN_GPE_ITEM;
				printf("vxlan-gpe / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "gre")) {
				flow_items |= GRE_ITEM;
				printf("gre / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "geneve")) {
				flow_items |= GENEVE_ITEM;
				printf("geneve / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "gtp")) {
				flow_items |= GTP_ITEM;
				printf("gtp / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "meta")) {
				flow_items |= META_ITEM;
				printf("meta / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "tag")) {
				flow_items |= TAG_ITEM;
				printf("tag / ");
			}
			/* Actions */
			if (!strcmp(lgopts[opt_idx].name, "port-id")) {
				flow_actions |= PORT_ID_ACTION;
				printf("port-id / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "rss")) {
				flow_actions |= RSS_ACTION;
				printf("rss / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "hairpin-rss")) {
				flow_actions |= HAIRPIN_RSS_ACTION;
				printf("hairpin-rss / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "queue")) {
				flow_actions |= QUEUE_ACTION;
				printf("queue / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "hairpin-queue")) {
				flow_actions |= HAIRPIN_QUEUE_ACTION;
				printf("hairpin-queue / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "jump")) {
				flow_actions |= JUMP_ACTION;
				printf("jump / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "mark")) {
				flow_actions |= MARK_ACTION;
				printf("mark / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "count")) {
				flow_actions |= COUNT_ACTION;
				printf("count / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "set-meta")) {
				flow_actions |= META_ACTION;
				printf("set-meta / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "set-tag")) {
				flow_actions |= TAG_ACTION;
				printf("set-tag / ");
			}
			if (!strcmp(lgopts[opt_idx].name, "drop")) {
				flow_actions |= DROP_ACTION;
				printf("drop / ");
			}

			/* Control */
			if (!strcmp(lgopts[opt_idx].name, "flows-count")) {
				n = atoi(optarg);
				if (n > (int) iterations_number)
					flows_count = n;
				else {
					printf("\n\nflows_count should be > %d",
						iterations_number);
					rte_exit(EXIT_SUCCESS, " ");
				}
			}
			if (!strcmp(lgopts[opt_idx].name, "dump-iterations"))
				dump_iterations = true;
			if (!strcmp(lgopts[opt_idx].name, "deletion-rate"))
				delete_flag = true;
			if (!strcmp(lgopts[opt_idx].name, "dump-socket-mem"))
				dump_socket_mem_flag = true;
			break;
		default:
			usage(argv[0]);
			printf("Invalid option: %s\n", argv[optind]);
			rte_exit(EXIT_SUCCESS, "Invalid option\n");
			break;
		}
	}
	printf("end_flow\n");
}

/* Dump the socket memory statistics on console */
static size_t
dump_socket_mem(FILE *f)
{
	struct rte_malloc_socket_stats socket_stats;
	unsigned int i = 0;
	size_t total = 0;
	size_t alloc = 0;
	size_t free = 0;
	unsigned int n_alloc = 0;
	unsigned int n_free = 0;
	bool active_nodes = false;


	for (i = 0; i < RTE_MAX_NUMA_NODES; i++) {
		if (rte_malloc_get_socket_stats(i, &socket_stats) ||
		    !socket_stats.heap_totalsz_bytes)
			continue;
		active_nodes = true;
		total += socket_stats.heap_totalsz_bytes;
		alloc += socket_stats.heap_allocsz_bytes;
		free += socket_stats.heap_freesz_bytes;
		n_alloc += socket_stats.alloc_count;
		n_free += socket_stats.free_count;
		if (dump_socket_mem_flag) {
			fprintf(f, "::::::::::::::::::::::::::::::::::::::::");
			fprintf(f,
				"\nSocket %u:\nsize(M) total: %.6lf\nalloc:"
				" %.6lf(%.3lf%%)\nfree: %.6lf"
				"\nmax: %.6lf"
				"\ncount alloc: %u\nfree: %u\n",
				i,
				socket_stats.heap_totalsz_bytes / 1.0e6,
				socket_stats.heap_allocsz_bytes / 1.0e6,
				(double)socket_stats.heap_allocsz_bytes * 100 /
				(double)socket_stats.heap_totalsz_bytes,
				socket_stats.heap_freesz_bytes / 1.0e6,
				socket_stats.greatest_free_size / 1.0e6,
				socket_stats.alloc_count,
				socket_stats.free_count);
				fprintf(f, "::::::::::::::::::::::::::::::::::::::::");
		}
	}
	if (dump_socket_mem_flag && active_nodes) {
		fprintf(f,
			"\nTotal: size(M)\ntotal: %.6lf"
			"\nalloc: %.6lf(%.3lf%%)\nfree: %.6lf"
			"\ncount alloc: %u\nfree: %u\n",
			total / 1.0e6, alloc / 1.0e6,
			(double)alloc * 100 / (double)total, free / 1.0e6,
			n_alloc, n_free);
		fprintf(f, "::::::::::::::::::::::::::::::::::::::::\n");
	}
	return alloc;
}

static void
print_flow_error(struct rte_flow_error error)
{
	printf("Flow can't be created %d message: %s\n",
		error.type,
		error.message ? error.message : "(no stated reason)");
}

static inline void
destroy_flows(int port_id, struct rte_flow **flow_list)
{
	struct rte_flow_error error;
	clock_t start_iter, end_iter;
	double cpu_time_used = 0;
	double flows_rate;
	double cpu_time_per_iter[MAX_ITERATIONS];
	double delta;
	uint32_t i;
	int iter_id;

	for (i = 0; i < MAX_ITERATIONS; i++)
		cpu_time_per_iter[i] = -1;

	if (iterations_number > flows_count)
		iterations_number = flows_count;

	/* Deletion Rate */
	printf("Flows Deletion on port = %d\n", port_id);
	start_iter = clock();
	for (i = 0; i < flows_count; i++) {
		if (!flow_list[i])
			break;

		memset(&error, 0x33, sizeof(error));
		if (rte_flow_destroy(port_id, flow_list[i], &error)) {
			print_flow_error(error);
			rte_exit(EXIT_FAILURE, "Error in deleting flow");
		}

		if (i && !((i + 1) % iterations_number)) {
			/* Save the deletion rate of each iter */
			end_iter = clock();
			delta = (double) (end_iter - start_iter);
			iter_id = ((i + 1) / iterations_number) - 1;
			cpu_time_per_iter[iter_id] =
				delta / CLOCKS_PER_SEC;
			cpu_time_used += cpu_time_per_iter[iter_id];
			start_iter = clock();
		}
	}

	/* Deletion rate per iteration */
	if (dump_iterations)
		for (i = 0; i < MAX_ITERATIONS; i++) {
			if (cpu_time_per_iter[i] == -1)
				continue;
			delta = (double)(iterations_number /
				cpu_time_per_iter[i]);
			flows_rate = delta / 1000;
			printf(":: Iteration #%d: %d flows "
				"in %f sec[ Rate = %f K/Sec ]\n",
				i, iterations_number,
				cpu_time_per_iter[i], flows_rate);
		}

	/* Deletion rate for all flows */
	flows_rate = ((double) (flows_count / cpu_time_used) / 1000);
	printf("\n:: Total flow deletion rate -> %f K/Sec\n",
		flows_rate);
	printf(":: The time for deleting %d in flows %f seconds\n",
		flows_count, cpu_time_used);
}

static inline void
flows_handler(void)
{
	struct rte_flow **flow_list;
	struct rte_flow_error error;
	clock_t start_iter, end_iter;
	double cpu_time_used = 0;
	double flows_rate;
	double cpu_time_per_iter[MAX_ITERATIONS];
	double delta;
	uint16_t nr_ports;
	uint32_t i;
	int port_id;
	int iter_id;
	uint32_t eagain_counter = 0;
	uint32_t flow_index;

	nr_ports = rte_eth_dev_count_avail();

	for (i = 0; i < MAX_ITERATIONS; i++)
		cpu_time_per_iter[i] = -1;

	if (iterations_number > flows_count)
		iterations_number = flows_count;

	printf(":: Flows Count per port: %d\n", flows_count);

	flow_list = rte_zmalloc("flow_list",
		(sizeof(struct rte_flow *) * flows_count) + 1, 0);
	if (flow_list == NULL)
		rte_exit(EXIT_FAILURE, "No Memory available!");

	for (port_id = 0; port_id < nr_ports; port_id++) {
		flow_index = 0;

		if (flow_group > 0) {
			/*
			 * Create global rule to jumo into flow_group
			 * This way the app will avoid the default rules
			 *
			 * Golbal rule:
			 * group 0 eth / end actions jump group <flow_group>
			 *
			 */
			flow = generate_flow(port_id, 0, flow_attrs, ETH_ITEM,
				JUMP_ACTION, flow_group, 0, &error);

			if (!flow) {
				print_flow_error(error);
				rte_exit(EXIT_FAILURE, "error in creating flow");
			}
			flow_list[flow_index++] = flow;
		}

		/* Insertion Rate */
		printf("Flows insertion on port = %d\n", port_id);
		start_iter = clock();
		for (i = 0; i < flows_count; i++) {
			do {
				rte_errno = 0;
				flow = generate_flow(port_id, flow_group,
					flow_attrs, flow_items, flow_actions,
					JUMP_ACTION_TABLE, i,  &error);
				if (!flow)
					eagain_counter++;
			} while (rte_errno == EAGAIN);

			if (force_quit)
				i = flows_count;

			if (!flow) {
				print_flow_error(error);
				rte_exit(EXIT_FAILURE, "error in creating flow");
			}

			flow_list[flow_index++] = flow;

			if (i && !((i + 1) % iterations_number)) {
				/* Save the insertion rate of each iter */
				end_iter = clock();
				delta = (double) (end_iter - start_iter);
				iter_id = ((i + 1) / iterations_number) - 1;
				cpu_time_per_iter[iter_id] =
					delta / CLOCKS_PER_SEC;
				cpu_time_used += cpu_time_per_iter[iter_id];
				start_iter = clock();
			}
		}

		/* Iteration rate per iteration */
		if (dump_iterations)
			for (i = 0; i < MAX_ITERATIONS; i++) {
				if (cpu_time_per_iter[i] == -1)
					continue;
				delta = (double)(iterations_number /
					cpu_time_per_iter[i]);
				flows_rate = delta / 1000;
				printf(":: Iteration #%d: %d flows "
					"in %f sec[ Rate = %f K/Sec ]\n",
					i, iterations_number,
					cpu_time_per_iter[i], flows_rate);
			}

		/* Insertion rate for all flows */
		flows_rate = ((double) (flows_count / cpu_time_used) / 1000);
		printf("\n:: Total flow insertion rate -> %f K/Sec\n",
						flows_rate);
		printf(":: The time for creating %d in flows %f seconds\n",
						flows_count, cpu_time_used);
		printf(":: EAGAIN counter = %d\n", eagain_counter);

		if (delete_flag)
			destroy_flows(port_id, flow_list);
	}
}

static void
signal_handler(int signum)
{
	if (signum == SIGINT || signum == SIGTERM) {
		printf("\n\nSignal %d received, preparing to exit...\n",
					signum);
		printf("Error: Stats are wrong due to sudden signal!\n\n");
		force_quit = true;
	}
}

static void
init_port(void)
{
	int ret;
	uint16_t i, j;
	uint16_t port_id;
	uint16_t nr_queues;
	bool hairpin_flag = false;
	uint16_t nr_ports = rte_eth_dev_count_avail();
	struct rte_eth_hairpin_conf hairpin_conf = {
			.peer_count = 1,
	};
	struct rte_eth_conf port_conf = {
		.rxmode = {
			.split_hdr_size = 0,
		},
		.rx_adv_conf = {
			.rss_conf.rss_hf =
					ETH_RSS_IP  |
					ETH_RSS_UDP |
					ETH_RSS_TCP,
		}
	};
	struct rte_eth_txconf txq_conf;
	struct rte_eth_rxconf rxq_conf;
	struct rte_eth_dev_info dev_info;

	nr_queues = RXQs;
	if (flow_actions & HAIRPIN_QUEUE_ACTION ||
		flow_actions & HAIRPIN_RSS_ACTION) {
		nr_queues = RXQs + HAIRPIN_QUEUES;
		hairpin_flag = true;
	}

	if (nr_ports == 0)
		rte_exit(EXIT_FAILURE, "Error: no port detected\n");
	mbuf_mp = rte_pktmbuf_pool_create("mbuf_pool",
					TOTAL_MBUF_NUM, MBUF_CACHE_SIZE,
					0, MBUF_SIZE,
					rte_socket_id());

	if (mbuf_mp == NULL)
		rte_exit(EXIT_FAILURE, "Error: can't init mbuf pool\n");

	for (port_id = 0; port_id < nr_ports; port_id++) {
		ret = rte_eth_dev_info_get(port_id, &dev_info);
		if (ret != 0)
			rte_exit(EXIT_FAILURE,
					"Error during getting device (port %u) info: %s\n",
					port_id, strerror(-ret));

		port_conf.txmode.offloads &= dev_info.tx_offload_capa;
		printf(":: initializing port: %d\n", port_id);
		ret = rte_eth_dev_configure(port_id, nr_queues,
				nr_queues, &port_conf);
		if (ret < 0)
			rte_exit(EXIT_FAILURE,
					":: cannot configure device: err=%d, port=%u\n",
					ret, port_id);

		rxq_conf = dev_info.default_rxconf;
		rxq_conf.offloads = port_conf.rxmode.offloads;
		for (i = 0; i < RXQs; i++) {
			ret = rte_eth_rx_queue_setup(port_id, i, NR_RXD,
						rte_eth_dev_socket_id(port_id),
						&rxq_conf,
						mbuf_mp);
			if (ret < 0)
				rte_exit(EXIT_FAILURE,
						":: Rx queue setup failed: err=%d, port=%u\n",
						ret, port_id);
		}

		txq_conf = dev_info.default_txconf;
		txq_conf.offloads = port_conf.txmode.offloads;

		for (i = 0; i < TXQs; i++) {
			ret = rte_eth_tx_queue_setup(port_id, i, NR_TXD,
						rte_eth_dev_socket_id(port_id),
						&txq_conf);
			if (ret < 0)
				rte_exit(EXIT_FAILURE,
						":: Tx queue setup failed: err=%d, port=%u\n",
						ret, port_id);
		}

		ret = rte_eth_promiscuous_enable(port_id);
		if (ret != 0)
			rte_exit(EXIT_FAILURE,
					":: promiscuous mode enable failed: err=%s, port=%u\n",
					rte_strerror(-ret), port_id);

		if (hairpin_flag) {
			for (i = RXQs, j = 0;
					i < RXQs + HAIRPIN_QUEUES; i++, j++) {
				hairpin_conf.peers[0].port = port_id;
				hairpin_conf.peers[0].queue = j + TXQs;
				ret = rte_eth_rx_hairpin_queue_setup(port_id, i,
					NR_RXD, &hairpin_conf);
				if (ret != 0)
					rte_exit(EXIT_FAILURE,
						":: Hairpin rx queue setup failed: err=%d, port=%u\n",
						ret, port_id);
			}

			for (i = TXQs, j = 0;
					i < TXQs + HAIRPIN_QUEUES; i++, j++) {
				hairpin_conf.peers[0].port = port_id;
				hairpin_conf.peers[0].queue = j + RXQs;
				ret = rte_eth_tx_hairpin_queue_setup(port_id, i,
					NR_TXD, &hairpin_conf);
				if (ret != 0)
					rte_exit(EXIT_FAILURE,
						":: Hairpin tx queue setup failed: err=%d, port=%u\n",
						ret, port_id);
			}
		}

		ret = rte_eth_dev_start(port_id);
		if (ret < 0)
			rte_exit(EXIT_FAILURE,
				"rte_eth_dev_start:err=%d, port=%u\n",
				ret, port_id);

		printf(":: initializing port: %d done\n", port_id);
	}
}

int
main(int argc, char **argv)
{
	uint16_t lcore_id;
	uint16_t port;
	uint16_t nr_ports;
	int ret;
	struct rte_flow_error error;
	int64_t alloc, last_alloc;

	nr_ports = rte_eth_dev_count_avail();
	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "EAL init failed\n");

	force_quit = false;
	dump_iterations = false;
	delete_flag = false;
	dump_socket_mem_flag = false;
	flows_count = 4000000;
	iterations_number = 100000;
	flow_group = 0;

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	argc -= ret;
	argv += ret;

	if (argc > 1)
		args_parse(argc, argv);

	init_port();

	nb_lcores = rte_lcore_count();

	if (nb_lcores <= 1)
		rte_exit(EXIT_FAILURE, "This app needs at least two cores\n");

	last_alloc = (int64_t)dump_socket_mem(stdout);
	flows_handler();
	alloc = (int64_t)dump_socket_mem(stdout);

	if (last_alloc)
		fprintf(stdout, ":: Memory allocation change(M): %.6lf\n",
		(alloc - last_alloc) / 1.0e6);

	RTE_LCORE_FOREACH_SLAVE(lcore_id)

	if (rte_eal_wait_lcore(lcore_id) < 0)
		break;

	for (port = 0; port < nr_ports; port++) {
		rte_flow_flush(port, &error);
		rte_eth_dev_stop(port);
		rte_eth_dev_close(port);
	}
	return 0;
}
