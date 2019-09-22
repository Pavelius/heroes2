#include "view.h"

using namespace draw;

static void use_spells() {
}

static void run_away() {}

static void make_offer() {}

void heroi::battlemenu(bool friendly, bool surrender) const {
	state push;
	screenshoot surface;
	font = SMALFONT;
	auto back = isevil(VGENBKGE, VGENBKG);
	auto w1 = getwidth(back, 0);
	auto h1 = getheight(back, 0);
	auto x = (width - w1) / 2;
	auto y = (height - h1) / 2 - 32;
	while(ismodal()) {
		surface.restore();
		image(x, y, back, 1);
		image(x, y, back, 0);
		image(x + 27, y + 42, getid(), true);
		auto col = 1;
		image(x + 148, y + 36, VIEWGEN, col);
		string sb;
		sb.add("%1 - %2", getname(), getstr(kind));
		text((draw::width - draw::textw(sb)) / 2, y + 12, sb);
		auto y1 = y + 40 + 8;
		sb.clear();
		sb.addo(this);
		textf(x + 148, y1, 110, sb);
		button(x + 30, y + 148, VIEWGEN, use_spells, {9, 9, 10}, 0);
		button(x + 89, y + 148, VIEWGEN, run_away, {11, 11, 12}, 0);
		button(x + 148, y + 148, VIEWGEN, make_offer, {13, 13, 14}, 0);
		button(x + 207, y + 148, VIEWGEN, buttoncancel, {15, 15, 16}, KeyEscape);
		domodal();
	}
}