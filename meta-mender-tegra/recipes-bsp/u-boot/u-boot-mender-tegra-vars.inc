MENDER_UBOOT_AUTO_CONFIGURE = "0"
# Use the mmcblk0boot1 partition for uboot environment (partition 2
# in uboot) or user partition (0) on Nano with SDcard environment
MENDER_UBOOT_CONFIG_SYS_MMC_ENV_PART = "${@'0' if (d.getVar('TEGRA_MENDER_UBOOT_ENV_IN_SPIFLASH') or '') != '1' and (d.getVar('TEGRA_SPIFLASH_BOOT') or '') == '1' else '2'}"

# Calculate this offset by adding up the offsets of each partition preceeding the uboot_env partition in sdmmc_boot and aligning to the next
# 4096 byte boundary, then subtracting 4 MiB (4194304) since the sdmmc_boot represents the combined boot0 and boot1 partitions
# Please note the suggestions in the nvidia thread at https://devtalk.nvidia.com/default/topic/1063652/jetson-tx2/mmcblk0boot1-usage-at-address-4177408-and-u-boot-parameter-storage-space-availability/
# regarding suggestions about locating this environment area in mmcblk0boot1 at 0x3BB000 (3911680)
# We use the same offset for tegra210 eMMC. For tegra210 SPI/SD, we
# locate the environment in the padding block, before the VER
# partition when using SPI flash.  Otherwise, we use the location of
# the ENV partition on the SDcard - 0xC00000 (12582912).
MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_tegra186 = "3911680"
MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_tegra210 = "${@'3866624' if (d.getVar('TEGRA_MENDER_UBOOT_ENV_IN_SPIFLASH') or '') == '1' else '12582912' if (d.getVar('TEGRA_SPIFLASH_BOOT') or '') == '1' else '3911680'}"
MENDER_UBOOT_MMC_ENV_LINUX_DEVICE_PATH_tegra210 = "${@'/dev/mtd0' if (d.getVar('TEGRA_MENDER_UBOOT_ENV_IN_SPIFLASH') or '') == '1' else '/dev/mmcblk0' if (d.getVar('TEGRA_SPIFLASH_BOOT') or '') == '1' else mender_get_uboot_env_mmc_linux_device_path(d)}"

# Optionally use SDcard instead of SPI flash on Jetson Nano dev kits.
# Starting with meta-tegra zeus branch, Nano dev kits use the SPI flash
# for the U-Boot environment (as NVIDIA does with L4T R32.3.1 and
# later).
#
# However, for Mender it may be more convenient to use an SDcard
# partition, so the Mender environment is more self-contained.
# Just set:
#
# TEGRA_MENDER_UBOOT_ENV_IN_SPIFLASH = ""
#
# in your build configuration to do that.
TEGRA_MENDER_UBOOT_ENV_IN_SPIFLASH ?= "${TEGRA_SPIFLASH_BOOT}"
MENDER_UBOOT_STORAGE_DEVICE_tegra210 = "${@'1' if (d.getVar('TEGRA_SPIFLASH_BOOT') or '') == '1' else '0'}"