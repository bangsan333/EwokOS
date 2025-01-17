#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fbd/fbd.h>
#include <graph/graph.h>
#include <arch/bcm283x/framebuffer.h>

static graph_t* _g = NULL;

static uint32_t flush(const fbinfo_t* fbinfo, const void* buf, uint32_t size, int rotate) {
	uint32_t sz = 4 * fbinfo->width * fbinfo->height;
	if(size < sz || fbinfo->depth != 32)
		return -1;

	graph_t g;
	if(rotate == G_ROTATE_N90 || rotate == G_ROTATE_90) {
		graph_init(&g, buf, fbinfo->height, fbinfo->width);
		if(_g == NULL)
			_g = graph_new(NULL, fbinfo->width, fbinfo->height);
	}
	else if(rotate == G_ROTATE_180) {
		graph_init(&g, buf, fbinfo->width, fbinfo->height);
		if(_g == NULL)
			_g = graph_new(NULL, fbinfo->width, fbinfo->height);
	}

	if(_g != NULL) {
		graph_rotate_to(&g, _g, rotate);
		memcpy((void*)fbinfo->pointer, _g->buffer, sz);
	}
	else  {
		memcpy((void*)fbinfo->pointer, buf, sz);
	}
	return size;
}

static fbinfo_t* get_info(void) {
	return bcm283x_get_fbinfo();
}

static int32_t init(uint32_t w, uint32_t h, uint32_t dep) {
	return bcm283x_fb_init(w, h, dep);
}

int main(int argc, char** argv) {
	fbd_t fbd;
	_g = NULL;

	fbd.flush = flush;
	fbd.init = init;
	fbd.get_info = get_info;

	int res = fbd_run(&fbd, argc, argv);
	if(_g != NULL)
		graph_free(_g);
	return res;
}
