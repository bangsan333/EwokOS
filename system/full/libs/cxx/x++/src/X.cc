#include "x++/X.h"
#include "x++/XWin.h"
#include <stdio.h>

using namespace Ewok;

bool X::open(XWin* xwin, int x, int y, uint32_t w, uint32_t h, const char* title, uint32_t style) {
	xwin_t* xw = xwin_open(&this->x, x, y, w, h, title, style);
	if(xw == NULL)
		return false;
	xwin->setX(this);
	xwin->setCWin(xw);
	return true;
}

X::X(void) {
	x_init(&x, this);
}

void X::run(void (*loop)(void*), void* p) {
	x.on_loop = loop;
	x_run(&x, p);
}

void X::terminate(void) {
	x.terminated = true;
}

bool X::screenInfo(xscreen_t& scr, int index) {
	return (x_screen_info(&scr, index) == 0);
}

const char* X::getResName(const char* name) {
	static char ret[FS_FULL_NAME_MAX];
	const char* wkdir = x_get_work_dir();
	if(wkdir[1] == 0 && wkdir[0] == '/')
		snprintf(ret, FS_FULL_NAME_MAX-1, "/res/%s", name);
	else
		snprintf(ret, FS_FULL_NAME_MAX-1, "%s/res/%s", wkdir, name);
	return ret;
}
