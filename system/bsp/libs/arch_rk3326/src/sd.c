#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/mmio.h>
#include <sysinfo.h>
#include <sys/mmio.h>

#include "dwmmc.h"

extern int mmc_read_blocks(struct dwmci_host *host, void *dst, uint32_t sector);

static struct dwmci_host dwc_host = {
    .fifoth_val = 0,
    .fifo_mode = true,
};

/**
 * initialize EMMC to read SDHC card
 */
int32_t rk3326_sd_init(void) {
	_mmio_base = mmio_map();
	dwc_host.ioaddr = (void*)(_mmio_base + 0x370000);
	return 0;
}


int32_t rk3326_sd_read_sector(int32_t sector, void* buf) {
	return mmc_read_blocks(&dwc_host, buf, sector);
}

int32_t rk3326_sd_write_sector(int32_t sector, const void* buf) {
	(void)sector;
	(void)buf;
	return 0;
}

