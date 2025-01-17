#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/vfs.h>
#include <sys/vdevice.h>
#include <sys/charbuf.h>
#include <sys/mmio.h>
#include <sys/proc.h>
#include <sys/ipc.h>
#include <sys/interrupt.h>
#include <sys/interrupt.h>

#define UART_LSR_THRE	0x20
#define UART_LSR_DR 	0x01      
#define REG32(x) (*(volatile uint32_t*)(x))

static int uart_read(int fd, int from_pid, fsinfo_t* info, 
		void* buf, int size, int offset, void* p) {
	(void)fd;
	(void)from_pid;
	(void)info;
	(void)size;
	(void)offset;
	(void)p;

	if((REG32(_mmio_base + 0x160014) & UART_LSR_DR) == 0)
		return ERR_RETRY_NON_BLOCK;

	((char*)buf)[0] = (char)REG32(_mmio_base + 0x160000);
	return 1;
}

static int uart_write(int fd, int from_pid, fsinfo_t* info,
		const void* buf, int size, int offset, void* p) {
	(void)fd;
	(void)info;
	(void)from_pid;
	(void)offset;
	(void)p;
	char c;

	for(int i = 0; i < size; i++){
		c = ((char*)buf)[i];
		if(c == '\r') c = '\n';
		while (((REG32(_mmio_base + 0x160014)) & UART_LSR_THRE) == 0);	
			REG32(_mmio_base + 0x160000) = c;
	}
	return size;
}

// static void interrupt_handle(uint32_t interrupt, uint32_t data) {
// 	(void)interrupt;
// 	(void)data;

// 	sys_interrupt_end();
// }

int main(int argc, char** argv) {
	const char* mnt_point = argc > 1 ? argv[1]: "/dev/tty1";
	_mmio_base = mmio_map();
	vdevice_t dev;
	memset(&dev, 0, sizeof(vdevice_t));
	strcpy(dev.name, "rk_uart");
	dev.read = uart_read;
	dev.write = uart_write;

	//sys_interrupt_setup(SYS_INT_TIMER0, interrupt_handle, 0);
	device_run(&dev, mnt_point, FS_TYPE_CHAR);
	return 0;
}
