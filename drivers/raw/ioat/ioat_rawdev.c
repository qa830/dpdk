/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2019 Intel Corporation
 */

#include <rte_cycles.h>
#include <rte_bus_pci.h>
#include <rte_rawdev_pmd.h>

#include "rte_ioat_rawdev.h"

/* Dynamic log type identifier */
int ioat_pmd_logtype;

static struct rte_pci_driver ioat_pmd_drv;

#define IOAT_VENDOR_ID		0x8086
#define IOAT_DEVICE_ID		0x2021

#define IOAT_PMD_LOG(level, fmt, args...) rte_log(RTE_LOG_ ## level, \
	ioat_pmd_logtype, "%s(): " fmt "\n", __func__, ##args)

#define IOAT_PMD_DEBUG(fmt, args...)  IOAT_PMD_LOG(DEBUG, fmt, ## args)
#define IOAT_PMD_INFO(fmt, args...)   IOAT_PMD_LOG(INFO, fmt, ## args)
#define IOAT_PMD_ERR(fmt, args...)    IOAT_PMD_LOG(ERR, fmt, ## args)
#define IOAT_PMD_WARN(fmt, args...)   IOAT_PMD_LOG(WARNING, fmt, ## args)

#define DESC_SZ sizeof(struct rte_ioat_desc)
#define COMPLETION_SZ sizeof(__m128i)

static int
ioat_dev_configure(const struct rte_rawdev *dev, rte_rawdev_obj_t config)
{
	struct rte_ioat_rawdev_config *params = config;
	struct rte_ioat_rawdev *ioat = dev->dev_private;
	unsigned short i;

	if (dev->started)
		return -EBUSY;

	if (params == NULL)
		return -EINVAL;

	if (params->ring_size > 4096 || params->ring_size < 64 ||
			!rte_is_power_of_2(params->ring_size))
		return -EINVAL;

	ioat->ring_size = params->ring_size;
	if (ioat->desc_ring != NULL) {
		rte_free(ioat->desc_ring);
		ioat->desc_ring = NULL;
	}

	/* allocate one block of memory for both descriptors
	 * and completion handles.
	 */
	ioat->desc_ring = rte_zmalloc_socket(NULL,
			(DESC_SZ + COMPLETION_SZ) * ioat->ring_size,
			0, /* alignment, default to 64Byte */
			dev->device->numa_node);
	if (ioat->desc_ring == NULL)
		return -ENOMEM;
	ioat->hdls = (void *)&ioat->desc_ring[ioat->ring_size];

	ioat->ring_addr = rte_malloc_virt2iova(ioat->desc_ring);

	/* configure descriptor ring - each one points to next */
	for (i = 0; i < ioat->ring_size; i++) {
		ioat->desc_ring[i].next_desc_addr = ioat->ring_addr +
				(((i + 1) % ioat->ring_size) * DESC_SZ);
	}

	return 0;
}

static int
ioat_dev_start(struct rte_rawdev *dev)
{
	struct rte_ioat_rawdev *ioat = dev->dev_private;

	if (ioat->ring_size == 0 || ioat->desc_ring == NULL)
		return -EBUSY;

	/* inform hardware of where the descriptor ring is */
	ioat->regs->chainaddr = ioat->ring_addr;
	/* inform hardware of where to write the status/completions */
	ioat->regs->chancmp = ioat->status_addr;

	/* prime the status register to be set to the last element */
	ioat->status =  ioat->ring_addr + ((ioat->ring_size - 1) * DESC_SZ);
	return 0;
}

static void
ioat_dev_stop(struct rte_rawdev *dev)
{
	RTE_SET_USED(dev);
}

static void
ioat_dev_info_get(struct rte_rawdev *dev, rte_rawdev_obj_t dev_info)
{
	struct rte_ioat_rawdev_config *cfg = dev_info;
	struct rte_ioat_rawdev *ioat = dev->dev_private;

	if (cfg != NULL)
		cfg->ring_size = ioat->ring_size;
}

static int
ioat_rawdev_create(const char *name, struct rte_pci_device *dev)
{
	static const struct rte_rawdev_ops ioat_rawdev_ops = {
			.dev_configure = ioat_dev_configure,
			.dev_start = ioat_dev_start,
			.dev_stop = ioat_dev_stop,
			.dev_info_get = ioat_dev_info_get,
	};

	struct rte_rawdev *rawdev = NULL;
	struct rte_ioat_rawdev *ioat = NULL;
	int ret = 0;
	int retry = 0;

	if (!name) {
		IOAT_PMD_ERR("Invalid name of the device!");
		ret = -EINVAL;
		goto cleanup;
	}

	/* Allocate device structure */
	rawdev = rte_rawdev_pmd_allocate(name, sizeof(struct rte_ioat_rawdev),
					 dev->device.numa_node);
	if (rawdev == NULL) {
		IOAT_PMD_ERR("Unable to allocate raw device");
		ret = -EINVAL;
		goto cleanup;
	}

	rawdev->dev_ops = &ioat_rawdev_ops;
	rawdev->device = &dev->device;
	rawdev->driver_name = dev->device.driver->name;

	ioat = rawdev->dev_private;
	ioat->rawdev = rawdev;
	ioat->regs = dev->mem_resource[0].addr;
	ioat->ring_size = 0;
	ioat->desc_ring = NULL;
	ioat->status_addr = rte_malloc_virt2iova(ioat) +
			offsetof(struct rte_ioat_rawdev, status);

	/* do device initialization - reset and set error behaviour */
	if (ioat->regs->chancnt != 1)
		IOAT_PMD_ERR("%s: Channel count == %d\n", __func__,
				ioat->regs->chancnt);

	if (ioat->regs->chanctrl & 0x100) { /* locked by someone else */
		IOAT_PMD_WARN("%s: Channel appears locked\n", __func__);
		ioat->regs->chanctrl = 0;
	}

	ioat->regs->chancmd = RTE_IOAT_CHANCMD_SUSPEND;
	rte_delay_ms(1);
	ioat->regs->chancmd = RTE_IOAT_CHANCMD_RESET;
	rte_delay_ms(1);
	while (ioat->regs->chancmd & RTE_IOAT_CHANCMD_RESET) {
		ioat->regs->chainaddr = 0;
		rte_delay_ms(1);
		if (++retry >= 200) {
			IOAT_PMD_ERR("%s: cannot reset device. CHANCMD=0x%llx, CHANSTS=0x%llx, CHANERR=0x%llx\n",
					__func__,
					(unsigned long long)ioat->regs->chancmd,
					(unsigned long long)ioat->regs->chansts,
					(unsigned long long)ioat->regs->chanerr);
			ret = -EIO;
		}
	}
	ioat->regs->chanctrl = RTE_IOAT_CHANCTRL_ANY_ERR_ABORT_EN |
			RTE_IOAT_CHANCTRL_ERR_COMPLETION_EN;

	return 0;

cleanup:
	if (rawdev)
		rte_rawdev_pmd_release(rawdev);

	return ret;
}

static int
ioat_rawdev_destroy(const char *name)
{
	int ret;
	struct rte_rawdev *rdev;

	if (!name) {
		IOAT_PMD_ERR("Invalid device name");
		return -EINVAL;
	}

	rdev = rte_rawdev_pmd_get_named_dev(name);
	if (!rdev) {
		IOAT_PMD_ERR("Invalid device name (%s)", name);
		return -EINVAL;
	}

	/* rte_rawdev_close is called by pmd_release */
	ret = rte_rawdev_pmd_release(rdev);
	if (ret)
		IOAT_PMD_DEBUG("Device cleanup failed");

	return 0;
}

static int
ioat_rawdev_probe(struct rte_pci_driver *drv, struct rte_pci_device *dev)
{
	char name[32];
	int ret = 0;


	rte_pci_device_name(&dev->addr, name, sizeof(name));
	IOAT_PMD_INFO("Init %s on NUMA node %d", name, dev->device.numa_node);

	dev->device.driver = &drv->driver;
	ret = ioat_rawdev_create(name, dev);
	return ret;
}

static int
ioat_rawdev_remove(struct rte_pci_device *dev)
{
	char name[32];
	int ret;

	rte_pci_device_name(&dev->addr, name, sizeof(name));

	IOAT_PMD_INFO("Closing %s on NUMA node %d",
			name, dev->device.numa_node);

	ret = ioat_rawdev_destroy(name);
	return ret;
}

static const struct rte_pci_id pci_id_ioat_map[] = {
	{ RTE_PCI_DEVICE(IOAT_VENDOR_ID, IOAT_DEVICE_ID) },
	{ .vendor_id = 0, /* sentinel */ },
};

static struct rte_pci_driver ioat_pmd_drv = {
	.id_table = pci_id_ioat_map,
	.drv_flags = RTE_PCI_DRV_NEED_MAPPING | RTE_PCI_DRV_INTR_LSC |
		     RTE_PCI_DRV_IOVA_AS_VA,
	.probe = ioat_rawdev_probe,
	.remove = ioat_rawdev_remove,
};

RTE_PMD_REGISTER_PCI(IOAT_PMD_RAWDEV_NAME, ioat_pmd_drv);
RTE_PMD_REGISTER_PCI_TABLE(IOAT_PMD_RAWDEV_NAME, pci_id_ioat_map);
RTE_PMD_REGISTER_KMOD_DEP(IOAT_PMD_RAWDEV_NAME, "* igb_uio | uio_pci_generic");

RTE_INIT(ioat_pmd_init_log)
{
	ioat_pmd_logtype = rte_log_register(IOAT_PMD_LOG_NAME);
	if (ioat_pmd_logtype >= 0)
		rte_log_set_level(ioat_pmd_logtype, RTE_LOG_INFO);
}
