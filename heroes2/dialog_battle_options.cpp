#include "view.h"

using namespace draw;

static void change_bool() {
	auto p = (bool*)hot::param;
	*p = !(*p);
}

static int value_max;
static void change_int() {
	auto p = (int*)hot::param;
	auto n = value_max;
	*p = (*p) + 1;
	if(n)
		*p = *p % n;
}

static void checkbox(int x, int y, res_s icn, bool& value, int off, int on, const char* text) {
	auto i = value ? on : off;
	image(x, y, icn, i);
	if(mousein({x, y, x + getwidth(icn, off), y + getheight(icn, off)})) {
		if(hot::key == MouseLeft && hot::pressed)
			execute(change_bool, (int)&value);
		status(text);
	}
}

static void radiobox(int x, int y, res_s icn, int& value, int min, int max, const char* text) {
	if(draw::mousein({x, y, x + getwidth(icn, min), y + getheight(icn, min)})) {
		if(hot::key == MouseLeft && hot::pressed) {
			value_max = max - min + 1;
			execute(change_int, (int)&value);
		}
		draw::status(text);
	}
	int i = min + value;
	draw::image(x, y, icn, i);
}

void battlei::show() {
	state push;
	screenshoot surface;
	font = SMALFONT;
	auto icn = isevil(CSPANBKE, CSPANBKG);
	auto h8 = getheight(TEXTBAR, 8);
	auto h9 = getheight(TEXTBAR, 9);
	auto w3 = getwidth(TEXTBAR, 0);
	auto x1 = (width - getwidth(icn, 0)) / 2 + 16;
	auto y1 = (height - getheight(icn, 0)) / 2;
	while(ismodal()) {
		surface.restore();
		image(x1 - 16, y1, icn, 1);
		image(x1 - 16, y1, icn, 0);
		status({32, height - h8 - h9 + 1, width - w3, height - 1});
		radiobox(x1 + 36, y1 + 47, CSPANEL, speed, 0, 2, "Скорость анимации");
		radiobox(x1 + 128, y1 + 47, CSPANEL, info, 3, 5, "Уровень информации об отряде");
		checkbox(x1 + 220, y1 + 47, CSPANEL, usespells, 6, 7, "Использовать заклинания в автоматическом бою");
		checkbox(x1 + 36, y1 + 157, CSPANEL, grid, 8, 9, "Показать/Спрятать сетку");
		checkbox(x1 + 128, y1 + 157, CSPANEL, movement, 10, 11, "Показать/Спрятать движение");
		checkbox(x1 + 220, y1 + 157, CSPANEL, cursor, 12, 13, "Подсвечивать текущий гекс");
		button(x1 + 113, y1 + 252, isevil(CSPANBTE, CSPANBTN), buttonok, {0, 0, 1}, KeyEnter);
		domodal();
	}
}