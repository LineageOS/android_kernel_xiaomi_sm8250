# SPDX-License-Identifier: GPL-2.0-only

ccflags-y += -I$(srctree)/techpack/camera-xiaomi/include/uapi
ccflags-y += -I$(srctree)/techpack/camera-xiaomi/drivers/cam_utils
ccflags-y += -I$(srctree)/techpack/camera-xiaomi/drivers/cam_core
ccflags-y += -I$(srctree)/techpack/camera-xiaomi/drivers/cam_cpas/include
ccflags-y += -I$(srctree)/techpack/camera-xiaomi/drivers/cam_cpas

obj-$(CONFIG_SPECTRA_CAMERA) += cam_cpastop_hw.o
