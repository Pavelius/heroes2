#include "view.h"

using namespace draw;
using namespace battle;

const int				awd = 11, ahd = 9;
static heroi*			attacker;
static heroi*			defender;
static battleimage*		current_image;
bool					battle::setting::movement = true;
bool					battle::setting::cursor = true;
bool					battle::setting::distance = true;
bool					battle::setting::grid = true;
bool					battle::setting::index = true;

static unsigned char	hexagon_color;
static res_s			back, frng;
static unsigned short	hilite_index;

static battleimage		units[32];
static unsigned			units_count;
static battleimage		attacker_image, defender_image;
static short unsigned position_wide[2][5] = {{0, 22, 44, 66, 88}, {10, 32, 54, 76, 98}};

static point i2h(short unsigned index) {
	int x = 20 + 88 - ((index / awd) % 2 ? cell_wd / 2 : 0) + (cell_wd - 1) * (index % awd);
	int y = 20 + 85 + ((cell_hd / 4) * 3 - 1) * (index / awd);
	return{(short)x, (short)y};
}

static unsigned getcost(short unsigned index) {
	return Blocked;
}

static bool isattacker(const heroi* hero) {
	return hero == attacker;
}

static int getside(const heroi* leader) {
	return (attacker == leader) ? 0 : 1;
}

static void add_squad(short unsigned index, squadi& squad, heroi* leader) {
	if(units_count >= sizeof(units) / sizeof(units[0]))
		return;
	auto& e = units[units_count++];
	e.clear();
	e = squad.unit;
	e.pos = i2h(index);
	e.flags = isattacker(leader) ? 0 : AFMirror;
	e.squad = squad;
	e.squad_source = &squad;
	e.leader = leader;
	e.setpos(index);
	e.set(Wait);
}

static void add_squad(armyi& army, heroi* leader) {
	auto index = 0;
	auto side = getside(leader);
	for(auto& e : army.units) {
		if(!e)
			continue;
		add_squad(position_wide[side][index], e, leader);
		index++;
		if(index >= 5)
			break;
	}
}

static bool isend() {
	heroi* leader = 0;
	for(unsigned i = 0; i < units_count; i++) {
		if(!leader)
			leader = units[i].leader;
		else if(units[i].leader != leader)
			return false;
	}
	return true;
}

inline int sin_a(int a) {
	return a * 38 / 43;
}

inline int cos_a(int a) {
	return a * 22 / 43;
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
	add_squad(*this, this);
	add_squad(*enemy, enemy);
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
	for(unsigned i = 0; i < units_count; i++) {
		if(pb < pe)
			*pb++ = &units[i];
	}
	return pb - source;
}

static void update_drawables() {
	if(hot::key == InputTimer) {
		for(unsigned i = 0; i < units_count; i++)
			units[i].update();
		attacker_image.update();
		defender_image.update();
	}
}

static void paint_drawables(battleimage** source, unsigned count, const squadi* squad) {
	for(unsigned i = 0; i < count; i++) {
		source[i]->paint();
		if(&source[i]->squad == squad)
			source[i]->stroke();
	}
}

static void hittest_drawable(battleimage** source, unsigned count) {
	for(unsigned i = 0; i < count; i++) {
		auto p = source[i];
		if(hilite_index != Blocked && p->index == hilite_index)
			current_image = p;
		else if(p->type == Hero) {
			if(mousein({p->pos.x - 32, p->pos.y - 96, p->pos.x + 32, p->pos.y+ 8}))
				current_image = p;
		}
	}
}

static void paint_status() {
	if(!current_image)
		return;
	if(current_image->type == Hero) {
		auto p = current_image->gethero();
		status(p->getname());
	} else if(current_image->type == Monster) {
		auto p = &current_image->squad;
		status("%1i %2", p->count, bsmeta<monsteri>::elements[p->unit].multiname);
	}
}

static void paint_screen(squadi* squad) {
	battleimage* source[32];
	auto count = select_drawables(source, sizeof(source)/ sizeof(source[0]));
	hilite_index = Blocked;
	current_image = 0;
	hittest_grid();
	hittest_drawable(source, count);
	paint_field(squad);
	paint_grid(squad);
	paint_status();
	paint_drawables(source, count, squad);
}

void heroi::battlemove(squadi* squad) {
	while(ismodal()) {
		paint_screen(squad);
		domodal();
		update_drawables();
	}
}

void heroi::battlestart() {
	static speed_s speeds[] = {UltraFastSpeed, VeryFastSpeed, FastSpeed, AverageSpeed, SlowSpeed, VerySlowSpeed, CrawlingSpeed};
	while(!isend()) {
		for(auto s : speeds) {
			for(unsigned i = 0; i < units_count; i++) {
				if(::units[i].squad.get(Speed) != s)
					continue;
				::units[i].leader->battlemove(&::units[i].squad);
			}
		}
	}
}