#include "view.h"

const int es = 16;

using namespace draw;

void dialog_palette() {
	while(ismodal()) {
		image(0, 0, STONEBAK, 0);
		auto x0 = 20;
		auto y0 = 20;
		for(auto y = 0; y < 16; y++) {
			for(auto x = 0; x < 16; x++) {
				auto x1 = x0 + x * es;
				auto y1 = y0 + y * es;
				auto n = y * 16 + x;
				rectf({x1, y1, x1 + es - 2, y1 + es - 2}, n);
			}
		}
		auto x = (hot::mouse.x - x0) / es;
		auto y = (hot::mouse.y - y0) / es;
		char temp[128]; zprint(temp, "Color %3i (%1i, %2i)", x, y, y * 16 + x);
		auto x1 = x0 + 18 * es;
		auto y1 = y0;
		text(x1, y1, temp);
		cursor(ADVMCO, 0);
		domodal();
	}
}