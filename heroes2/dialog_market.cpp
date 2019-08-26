#include "view.h"

using namespace draw;

static resource_s resf = Wood;
static resource_s rest = Gold;
static int count;
static int market_count;

static void select_source() {
	resf = (resource_s)hot::param;
}

static void select_target() {
	rest = (resource_s)hot::param;
}

static void make_trade() {
	((playeri*)hot::param)->trade(resf, rest, count, market_count);
	buttonok();
}

static void paint_resource(int x, int y, res_s res, int frame, resource_s id, costi& resources, resource_s select, resource_s resf, int markets) {
	char temp[32];
	image(x, y, res, frame);
	if(markets == 0) {
		auto value = resources.data[id - FirstResource];
		zprint(temp, "%1i", value);
	} else {
		auto value = playeri::getrate(resf, id, markets);
		if(value)
			zprint(temp, "1/%1i", value);
		else
			zprint(temp, "н/д");
	}
	text(x + (34 - draw::textw(temp)) / 2, y + 23, temp);
	if(select == id)
		image(x - 2, y - 2, res, 14);
	if(mousein({x - 2, y - 2, x + 37, y + 37})) {
		if(hot::key == MouseLeft && hot::pressed) {
			if(markets)
				execute(select_target, id);
			else
				execute(select_source, id);
		}
	}
}

static void paint_resources(int x, int y, res_s res, costi& resources, resource_s select, resource_s resf, int markets) {
	draw::state push;
	font = SMALFONT;
	paint_resource(x, y, res, 7, Wood, resources, select, resf, markets);
	paint_resource(x + 37, y, res, 8, Mercury, resources, select, resf, markets);
	paint_resource(x + 74, y, res, 9, Ore, resources, select, resf, markets);
	paint_resource(x, y + 37, res, 10, Sulfur, resources, select, resf, markets);
	paint_resource(x + 37, y + 37, res, 11, Crystal, resources, select, resf, markets);
	paint_resource(x + 74, y + 37, res, 12, Gems, resources, select, resf, markets);
	paint_resource(x + 37, y + 74, res, 13, Gold, resources, select, resf, markets);
}

void playeri::marketplace() {
	draw::screenshoot surface;
	const int width = 230;
	const int height = 300;
	market_count = getmarket();
	if(!market_count)
		return;
	auto icn = isevil(TRADPOSE, TRADPOST);
	auto x = (draw::width - width) / 2;
	auto ratf = 1;
	auto ratt = 1;
	char temp[260];
	while(ismodal()) {
		surface.restore();
		int y = dialog(height);
		int rate = playeri::getrate(resf, rest, market_count);
		if(rate) {
			if(rest == Gold) {
				zprint(temp, "На этом рынке за %-1 можно выторговать %3i %-2.",
					getstr(resf),
					bsmeta<resourcei>::elements[rest].nameof,
					rate);
			} else {
				zprint(temp, "На этом рынке за %3i %-1 можно получить %-2.",
					bsmeta<resourcei>::elements[resf].nameof,
					getstr(rest),
					rate);
			}
			textf(x, y, width, temp);
			int x2 = x;
			int x3 = x + width - 84;
			int maximum;
			if(rest == Gold)
				maximum = resources.data[resf - Gold];
			else
				maximum = resources.data[resf - Gold] / rate;
			splitter(x - 4, y + 96, icn, count, 0, maximum);
			image(x + width / 2, y + 96 - 32, icn, 0, AFCenter);
			image({x2, y + 51, x2 + 84, y + 51 + 42}, resf, rest == Gold ? count : count * rate, 0);
			image({x3, y + 51, x3 + 84, y + 51 + 42}, rest, rest == Gold ? count * rate : count, 0);
			if(true) {
				auto pf = font;
				font = SMALFONT;
				text(x, y + 96 - texth() - 2, width, AlignCenter, "Количество");
				font = pf;
			}
			button(x + (width - getwidth(icn, 15)) / 2, y + 120, icn, cmd(make_trade, (int)this), {15, 15, 16}, KeyEnter);
		} else
			textf(x, y, width, "Этот обмен не возможен.");
		paint_resources(x, y + 150, icn, resources, resf, Gold, 0);
		paint_resources(x + 120, y + 150, icn, resources, rest, resf, market_count);
		button(x + (width - getwidth(icn, 17)) / 2, y + height - getheight(icn, 17), icn, buttoncancel, {17, 17, 18}, KeyEscape);
		cursor(ADVMCO, 0);
		domodal();
	}
}