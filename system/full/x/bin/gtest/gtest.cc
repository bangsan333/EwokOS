#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <vprintf.h>
#include <upng/upng.h>
#include <sys/basic_math.h>
#include <x++/X.h>

using namespace Ewok;

class TestX : public XWin {
	int count, imgX, imgY;
	bool circle;
	graph_t* img_big;
	graph_t* img_small;
	font_t* font;

	void drawImage(Graph& g, graph_t* img) {
		if(img == NULL)
			return;
		g.blt(img, 0, 0, img->w, img->h,
				imgX, imgY, img->w, img->h, 0xff);
	}
public:
	inline TestX() {
		count = 0;
		circle = true;
        imgX = imgY = 0;
		img_big = png_image_new("/data/images/rokid.png");	
		img_small = png_image_new("/data/images/rokid_small.png");	
		font = font_by_name("8x16");
	}
	
	inline ~TestX() {
		if(img_big != NULL)
			graph_free(img_big);
		if(img_small != NULL)
			graph_free(img_small);
	}
protected:
	void onEvent(xevent_t* ev) {
		int key = 0;
		if(ev->type == XEVT_IM) {
			key = ev->value.im.value;
			if(key == 27) //esc
				this->close();
		}
	}

	void onRepaint(Graph& g) {
		char str[32];
		int gW = g.getW();
		int gH = g.getH();
		graph_t* img = gW > (img_big->w*2) ? img_big: img_small;

		int x = random_to(gW);
		int y = random_to(gH);
		int w = random_to(gW/4);
		int h = random_to(gH/4);
		int c = random();

		w = w < 8 ? 8 : w;
		h = h < 8 ? 8 : h;

		if((count++ % 100) == 0) {
			g.fill(0, 0, gW, gH, 0xff000000);
			imgX = x;
			imgY = y;
			if(imgX > (gW - img->w))
				imgX = gW - img->w;
			if(imgY > (gH - img->h - font->h))
				imgY = gH - img->h - font->h;
		}

		if(circle) {
			g.fillCircle(x, y, w, c);
			g.circle(x, y, w+10, c);
		}
		else {
			g.fill(x+5, y+5, w-10, h-10, c);
			g.box(x, y, w, h, c);
		}

		snprintf(str, 31, "EwokOS %d", count);
		get_text_size(str, font, (int32_t*)&w, NULL);
		g.fill(imgX, imgY+img->h+2, img->w, font->h, 0xffffffff);
		g.drawText(imgX+4, imgY+img->h+2, str, font, 0xff000000);
		drawImage(g, img);

		circle = !circle;	
	}
};

static void loop(void* p) {
	XWin* xwin = (XWin*)p;
	xwin->repaint();
	usleep(10000);
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	xscreen_t scr;

	X x;
	x.screenInfo(scr);

	TestX xwin;
	x.open(&xwin, random()%320, random()%240, scr.size.w/2-20, scr.size.h/2-20, "gtest", X_STYLE_NORMAL);

	xwin.setVisible(true);

	x.run(loop, &xwin);
	return 0;
} 
