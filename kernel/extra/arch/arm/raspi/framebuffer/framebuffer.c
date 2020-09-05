#include "mm/mmu.h"
#include "mm/kmalloc.h"
#include "mm/kalloc.h"
#include "kstring.h"
#include "bcm283x/framebuffer.h"
#include "bcm283x/mailbox.h"
#include <kstring.h>
#include <kernel/system.h>
#include <kernel/kernel.h>

int32_t fb_dev_init(uint32_t w, uint32_t h, uint32_t dep) {
	return bcm283x_fb_init(w, h, dep);
}

fbinfo_t* fb_get_info(void) {
	return bcm283x_get_fbinfo();
}

/*static inline void argb2abgr(uint32_t* dst, const uint32_t* src, uint32_t size) {
	while(size > 0) {
		register uint32_t c = src[size-1];
		uint8_t a = c >> 24;
		uint8_t r = c >> 16;
		uint8_t g = c >> 8;
		uint8_t b = c & 0xff;

		dst[size-1] = a << 24 | b << 16 | g << 8 | r;
		size--;
	}
}
*/

static inline void dup16(uint16_t* dst, uint32_t* src, uint32_t w, uint32_t h) {
	register int32_t i, size;
	size = w * h;
	for(i=0; i < size; i++) {
		register uint32_t s = src[i];
		register uint8_t r = (s >> 16) & 0xff;
		register uint8_t g = (s >> 8)  & 0xff;
		register uint8_t b = s & 0xff;
		dst[i] = ((r >> 3) <<11) | ((g >> 3) << 6) | (b >> 3);
	}
}

int32_t fb_dev_write(const void* buf, uint32_t size) {
	fbinfo_t* info = fb_get_info();
	uint32_t sz = (info->depth/8) * info->width * info->height;
	if(size > sz)
		size = sz;
	if(info->depth == 32) 
		//argb2abgr((uint32_t*)info->pointer, (const uint32_t*)buf, size/4);
		memcpy((void*)info->pointer, buf, size);
	else if(info->depth == 16) 
		dup16((uint16_t*)info->pointer, (uint32_t*)buf,  info->width, info->height);
	return (int32_t)size;
}
