#include "view.h"

using namespace draw;
using namespace battle;

static heroi*			attacker;
static heroi*			defender;
static unsigned char	hexagon_color;
static res_s			back, frng;
static unsigned short	hilite_index;

static battleimage		units[32];
static unsigned			units_count;
static battleimage		attacker_image, defender_image;

inline int sin_a(int a) {
	return a * 38 / 43;
}

inline int cos_a(int a) {
	return a * 22 / 43;
}

point battle::i2h(short unsigned index) {
	int x = 20 + 88 - ((index / awd) % 2 ? cell_wd / 2 : 0) + (cell_wd - 1) * (index % awd);
	int y = 20 + 85 + ((cell_hd / 4) * 3 - 1) * (index / awd);
	return{(short)x, (short)y};
}

static void prepare_background(landscape_s area, bool trees) {
	auto light = true;
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

static void prepare_leader(battleimage& e, heroi* hero, bool defender) {
	e.clear();
	e = hero->getid();
	e.set(Wait);
	if(defender) {
		e.pos.x = 606;
		e.pos.y = 156;
		e.flags = AFMirror;
	} else {
		e.pos.x = 32;
		e.pos.y = 186;
	}
}

static void paint_grid(const squadi* squad) {
	// Shadow movement indecies
	if(setting::movement) {
		state push;
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
		if(hilite_index != Blocked) {
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
		state push;
		font = SMALFONT;
		for(auto i = 0; i < awd*ahd; i++) {
			auto pt = i2h(i);
			char temp[8]; zprint(temp, "%1i", i);
			text(pt.x - 4, pt.y - 4, temp);
		}
	}
}

void heroi::setup_battle(heroi* enemy) {
	attacker = this;
	defender = enemy;
	auto pos = defender->getpos();
	prepare_background(Dirt, true);
	prepare_leader(attacker_image, attacker, false);
	prepare_leader(defender_image, defender, true);
}

static void skip_turn() {

}

static void open_setting() {

}

static void start_autocombat() {

}

static void hittest_grid() {
	for(int i = 0; i < awd*ahd; i++) {
		auto pt = i2h(i);
		rect rc = {pt.x - cell_wd / 2, pt.y - cell_hr, pt.x + cell_wd / 2, pt.y + cell_hr};
		point cooru[] =
		{
			{(short)(pt.x - cell_wd / 2), (short)(pt.y - cell_hr)},
		{pt.x, (short)(pt.y - cell_hd / 2)},
		{(short)(pt.x + cell_wd / 2), (short)(pt.y - cell_hr)},
		};
		point coord[] =
		{
			{(short)(pt.x + cell_wd / 2), (short)(pt.y + cell_hr)},
		{(short)pt.x, (short)(pt.y + cell_hd / 2)},
		{(short)(pt.x - cell_wd / 2), (short)(pt.y + cell_hr)},
		};
		if(mousein(rc)
			|| hot::mouse.in(cooru[0], cooru[1], cooru[2])
			|| hot::mouse.in(coord[0], coord[1], coord[2])) {
			hilite_index = i;
			return;
		}
	}
}

static direction_s hex_direction(int x1, int y1, point pt) {
	const int INFL = 12;
	point coord[7] = {{(short)x1, (short)y1},
	{(short)x1, (short)(y1 - cell_hd * INFL / 2)}, // u
	{(short)(x1 + cell_wd * INFL / 2), (short)(y1 - cell_hr * INFL)}, // ru
	{(short)(x1 + cell_wd * INFL / 2), (short)(y1 + cell_hr * INFL)}, // rd
	{(short)x1, (short)(y1 + cell_hd * INFL / 2)}, // d
	{(short)(x1 - cell_wd * INFL / 2), (short)(y1 + cell_hr * INFL)}, // ld
	{(short)(x1 - cell_wd * INFL / 2), (short)(y1 - cell_hr * INFL)}, // lu
	};
	if(pt == coord[0])
		return Up;
	else if(pt.in(coord[0], coord[1], coord[2]))
		return RightUp;
	else if(pt.in(coord[0], coord[2], coord[3]))
		return Right;
	else if(pt.in(coord[0], coord[3], coord[4]))
		return RightDown;
	else if(pt.in(coord[0], coord[4], coord[5]))
		return LeftDown;
	else if(pt.in(coord[0], coord[5], coord[6]))
		return Left;
	else if(pt.in(coord[0], coord[1], coord[6]))
		return LeftUp;
	return Up;
}

static void paint_field(squadi* squad) {
	auto h1 = getheight(TEXTBAR, 4);
	auto h2 = getheight(TEXTBAR, 6);
	auto h3 = getheight(TEXTBAR, 0);
	auto h8 = getheight(TEXTBAR, 8);
	auto h9 = getheight(TEXTBAR, 9);
	auto w3 = getwidth(TEXTBAR, 0);
	image(0, 0, back, 0);
	auto x = getwidth(TEXTBAR, 4);
	image(x, height - h8 - h9, TEXTBAR, 8);
	image(x, height - h9, TEXTBAR, 9);
	status({x + 32, draw::height - h8 - h9 + 1, draw::width - w3, draw::height - 1});
	button(0, height - h1 - h2, TEXTBAR, start_autocombat, {4, 4, 5}, Alpha + 'A', "Запустить автоматический бой");
	button(0, height - h2, TEXTBAR, open_setting, {6, 6, 7}, KeyEscape, "Открыть настройки");
	button(width - w3, height - h3, TEXTBAR, skip_turn, {0, 0, 1}, KeySpace, "Пропустить текущий ход");
	if(frng != NoRes)
		image(0, 0, frng, 0);
}

static unsigned select_drawables(battleimage** source, unsigned count) {
	auto pb = source;
	auto pe = source + count;
	*pb++ = &attacker_image;
	*pb++ = &defender_image;
	return pb - source;
}

static void update_drawables() {
	if(hot::key == InputTimer) {
		for(unsigned i = 0; i < units_count; i++)
			units[i].update();
	}
}

static void paint_drawables(battleimage** source, unsigned count) {
	for(unsigned i = 0; i < count; i++)
		source[i]->paint();
}

static void paint_screen(squadi* squad) {
	battleimage* source[32];
	auto count = select_drawables(source, sizeof(source)/ sizeof(source[0]));
	hilite_index = Blocked;
	paint_field(squad);
	hittest_grid();
	paint_grid(squad);
	paint_drawables(source, count);
}

void heroi::battlemove() {
	auto p = getslowest();
	while(ismodal()) {
		paint_screen(p);
		domodal();
		update_drawables();
	}
}