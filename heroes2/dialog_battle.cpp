#include "view.h"

using namespace draw;
using namespace battle;

static heroi*			attacker;
static heroi*			defender;
static unsigned char	hexagon_color;
static res_s			back, frng;
static unsigned short	hilite_index;

static void prepare_background(landscape_s area, bool trees) {
	auto light = true;
	res_s back, frng;
	switch(area) {
	case Desert:
		back = CBKGDSRT;
		frng = FRNG0004;
		break;
	case Snow:
		back = trees ? CBKGSNTR : CBKGSNMT;
		light = false;
		frng = trees ? FRNG0006 : FRNG0007;
		break;
	case Swamp:
		back = CBKGSWMP;
		frng = FRNG0008;
		break;
	case Waste:
		back = CBKGCRCK;
		light = false;
		frng = FRNG0003;
		break;
	case Beach:
		back = CBKGBEAC;
		light = false;
		frng = FRNG0002;
		break;
	case Lava:
		back = CBKGLAVA;
		frng = FRNG0005;
		break;
	case Dirt:
		back = trees ? CBKGDITR : CBKGDIMT;
		frng = trees ? FRNG0010 : FRNG0009;
		break;
	case Sea:
		back = CBKGWATR;
		frng = FRNG0013;
		break;
	default:
		back = trees ? CBKGGRTR : CBKGGRMT;
		frng = trees ? FRNG0011 : FRNG0009;
		break;
	}
	if(false) {
		back = CBKGGRAV;
		light = true;
		frng = FRNG0001;
	}
	hexagon_color = light ? 0xE0 : 0xE5;
}


static void paint_grid(const squadi* squad) {
	// Shadow movement indecies
	if(setting::movement) {
		draw::state push;
		font = SMALFONT;
		unsigned radius = squad->get(Speed) + 2;
		for(auto i = 0; i < awd*ahd; i++) {
			auto m = getcost(i);
			if(m) {
				auto pt = i2h(i);
				if(m <= radius)
					draw::hexagonf(pt.x, pt.y, 0);
				if(m < Blocked && setting::distance) {
					char temp[32]; zprint(temp, "%1i", m - 1);
					text(pt.x - draw::textw(temp) / 2, pt.y - 5, temp);
				}
			}
		}
	}
	// Shadow cursor index
	if(setting::cursor) {
		if(hilite_index != -1) {
			auto pt = i2h(hilite_index);
			hexagonf(pt.x, pt.y, 0);
		}
	}
	// Show grid
	if(setting::grid) {
		for(int i = 0; i < awd*ahd; i++) {
			auto pt = i2h(i);
			hexagon(pt.x, pt.y, hexagon_color);
		}
	}
	// Show index (only debug)
	if(setting::index) {
		draw::state push;
		font = SMALFONT;
		for(auto i = 0; i < awd*ahd; i++) {
			auto pt = i2h(i);
			char temp[8]; zprint(temp, "%1i", i);
			draw::text(pt.x - 4, pt.y - 4, temp);
		}
	}
}

void heroi::setup_battle(heroi* enemy) {
	attacker = this;
	defender = enemy;
	auto pos = defender->getpos();
	prepare_background(Dirt, true);
}

static void paint_screen(squadi* squad) {

}

void heroi::battlemove() {

}