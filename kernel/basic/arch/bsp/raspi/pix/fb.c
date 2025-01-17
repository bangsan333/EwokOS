#include <dev/fb.h>
#include <bcm283x/framebuffer.h>
#include <kstring.h>
#include <graph/graph.h>
#include <stddef.h>

static uint16_t* _g16 = NULL;
int32_t fb_init(uint32_t w, uint32_t h, fbinfo_t* fbinfo) {
	_g16 = NULL;
	if(bcm283x_fb_init(w, h, 32) != 0)
		return -1;
	memcpy(fbinfo, bcm283x_get_fbinfo(), sizeof(fbinfo_t));
	if(fbinfo->depth == 16)
		_g16 = (uint16_t*)fbinfo->pointer;
	return 0;
}

void fb_flush32(uint32_t* g32, uint32_t w, uint32_t h) {
	if(_g16 != NULL)
		blt16(g32, _g16, w, h);
}