#include "view.h"

using namespace draw;

const int				awd = 11, ahd = 9;
static heroi*			attacker;
static heroi*			defender;
static battleimage*		current_unit;
static unsigned short	hilite_index;
static battleimage*		hilite_unit;
battlei					battle;

static unsigned char	hexagon_color;
static res_s			back, frng;
static unsigned short	path[awd * ahd];
static unsigned short	path_stack[256 * 256];
static unsigned short	path_push;
static unsigned short	path_pop;
static unsigned short	path_goal;
static unsigned short	path_start;
static const direction_s all_around[] = {Left, Right, LeftUp, LeftDown, RightUp, RightDown};

static battleimage		units[20];
static battleimage		attacker_image, defender_image;
static short unsigned position_wide[2][5] = {{0, 22, 44, 66, 88}, {10, 32, 54, 76, 98}};

static point i2h(short unsigned index) {
	int x = 20 + 88 - ((index / awd) % 2 ? cell_wd / 2 : 0) + (cell_wd - 1) * (index % awd);
	int y = 20 + 85 + ((cell_hd / 4) * 3 - 1) * (index / awd);
	return{(short)x, (short)y};
}

void battleimage::setpos(short unsigned v) {
	positioni::setpos(v);
	pos = i2h(v);
}

static unsigned getcost(short unsigned index) {
	return path[index];
}

static bool isattacker(const heroi* hero) {
	return hero == attacker;
}

static int getside(const heroi* leader) {
	return (attacker == leader) ? 0 : 1;
}

static void add_squad(short unsigned index, squadi& squad, heroi* leader) {
	for(auto& e : units) {
		if(e)
			continue;
		e.clear();
		e.setup(squad, leader);
		e.type = Monster;
		e.monster = e.unit;
		e.flags = isattacker(leader) ? 0 : AFMirror;
		e.source = &squad;
		e.leader = leader;
		e.setpos(index);
		e.set(Wait);
		break;
	}
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
	for(auto& e : units) {
		if(!e)
			continue;
		if(!leader)
			leader = e.leader;
		else if(e.leader != leader)
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
	e.type = Hero;
	e.hero = hero->getid();
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

static void paint_grid() {
	// Shadow movement indecies
	if(battle.movement && current_unit) {
		state push;
		font = SMALFONT;
		unsigned radius = current_unit->get(Speed) + 2;
		for(auto i = 0; i < awd*ahd; i++) {
			auto m = getcost(i);
			if(m) {
				auto pt = i2h(i);
				if(m <= radius)
					draw::hexagonf(pt.x, pt.y, 0);
				if(m < Blocked && battle.distance) {
					char temp[32]; zprint(temp, "%1i", m - 1);
					text(pt.x - draw::textw(temp) / 2, pt.y - 5, temp);
				}
			}
		}
	}
	// Shadow cursor index
	if(battle.cursor) {
		if(hilite_index != Blocked) {
			auto pt = i2h(hilite_index);
			hexagonf(pt.x, pt.y, 0);
		}
	}
	// Show grid
	if(battle.grid) {
		for(int i = 0; i < awd*ahd; i++) {
			auto pt = i2h(i);
			hexagon(pt.x, pt.y, hexagon_color);
		}
	}
	// Show index (only debug)
	if(battle.index) {
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

static void end_turn() {
	current_unit->add(Moved);
	breakmodal(0);
}

static void skip_turn() {
	current_unit->add(TotalDefence);
	end_turn();
}

static void open_setting() {

}

static void start_autocombat() {

}

static void move_unit() {
	current_unit->setpos(hot::param);
	end_turn();
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

static void paint_field() {
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
	for(auto& e : units) {
		if(!e)
			continue;
		if(pb < pe)
			*pb++ = &e;
	}
	return pb - source;
}

static void update_drawables() {
	if(hot::key == InputTimer) {
		for(auto& e : units) {
			if(e)
				e.update();
		}
		attacker_image.update();
		defender_image.update();
	}
}

static void paint_drawables(battleimage** source, unsigned count) {
	for(unsigned i = 0; i < count; i++) {
		source[i]->paint();
		if(source[i] == current_unit)
			source[i]->stroke();
	}
}

static void hittest_drawable(battleimage** source, unsigned count) {
	for(unsigned i = 0; i < count; i++) {
		auto p = source[i];
		if(hilite_index != Blocked && p->index == hilite_index)
			hilite_unit = p;
		else if(p->type == Hero) {
			if(mousein({p->pos.x - 32, p->pos.y - 96, p->pos.x + 32, p->pos.y + 8}))
				hilite_unit = p;
		}
	}
}

static void paint_screen() {
	battleimage* source[32];
	auto count = select_drawables(source, sizeof(source) / sizeof(source[0]));
	hilite_index = Blocked;
	hilite_unit = 0;
	hittest_grid();
	hittest_drawable(source, count);
	paint_field();
	paint_grid();
	paint_drawables(source, count);
}

//// res::CMSECO
//// 0 - None
//// 1 - Move
//// 2 - Fly
//// 3 - Shoot
//// 4 - Hero (Helmet)
//// 5 - Info
//// 6 - Small cursor
//// 7 - Sword (to right up)
//// 8 - Sword (to right)
//// 9 - Sword (to right down)
//// 10 - Sword (to left down)
//// 11 - Sword (to left)
//// 12 - Sword (to left up)
//// 13 - Sword (to up)
//// 14 - Sword (to down)
//// 15 - Broken arrow
//switch(value) {
//case HexLeft:
//	icn = res::CMSECO;
//	start = 8;
//	pos.x = -25;
//	pos.y = -7;
//	break;
//case HexRight:
//	icn = res::CMSECO;
//	start = 11;
//	pos.x = -5;
//	pos.y = -7;
//	break;
//case HexLeftUp:
//	icn = res::CMSECO;
//	start = 9;
//	pos.x = -20;
//	pos.y = -20;
//	break;
//case HexLeftDown:
//	icn = res::CMSECO;
//	start = 7;
//	pos.x = -20;
//	pos.y = -5;
//	break;
//case HexRightUp:
//	icn = res::CMSECO;
//	start = 10;
//	pos.x = -5;
//	pos.y = -20;
//	break;
//case HexRightDown:
//	icn = res::CMSECO;
//	start = 12;
//	pos.x = -5;
//	pos.y = -5;
//	break;
//case Information:
//	icn = res::CMSECO;
//	start = 5;
//	pos.x = -7;
//	pos.y = -7;
//	break;
//case Fly:
//	icn = res::CMSECO;
//	start = 2;
//	pos.x = -7;
//	pos.y = -14;
//	break;
//case Shoot:
//	icn = res::CMSECO;
//	start = 3;
//	pos.x = -7;
//	pos.y = -7;
//	break;
//case Hero:
//	icn = res::CMSECO;
//	start = 4;
//	pos.x = -7;
//	pos.y = -7;
//	break;

static void standart_input() {
	if(hilite_unit) {
		if(hilite_unit->type == Hero) {
			auto p = hilite_unit->gethero();
			if(p) {
				status(p->getname());
				setcursor(CMSECO, 4, {-7, -7});
				if(hot::key == MouseLeft && hot::pressed) {

				}
			}
		} else if(hilite_unit->type == Monster) {
			uniti* p = hilite_unit;
			status("%1i %2", p->count, bsmeta<monsteri>::elements[p->unit].multiname);
		}
	} else if(hilite_index != Blocked && current_unit) {
		int a = getcost(hilite_index) - 1;
		int m = current_unit->get(Speed) + 1;
		if(a <= m) {
			setcursor(CMSECO, 1, {-7, -14});
			if(hot::key == MouseLeft && hot::pressed)
				execute(move_unit, hilite_index);
		} else
			setcursor(CMSECO, 0, {-7, -7});
	}
}

short unsigned to(short unsigned i, direction_s d) {
	if(i == Blocked)
		return Blocked;
	auto x = i % awd, y = i / awd;
	switch(d) {
	case Left:
		if(x == 0)
			return Blocked;
		return i - 1;
	case Right:
		if(x < awd - 1)
			return i + 1;
		return Blocked;
	case LeftUp:
		if(x >= (y & 1) && y > 0)
			return i - 11 - (y & 1);
		return Blocked;
	case RightUp:
		if(x >= awd - 1 + (y & 1))
			return Blocked;
		if(y == 0)
			return Blocked;
		return i - 10 - (y & 1);
	case LeftDown:
		if((x >= (y & 1)) && y < (ahd - 1))
			return i + 11 - (y & 1);
		return Blocked;
	case RightDown:
		if(x < (awd - 1 + (y & 1)) && y < (ahd - 1))
			return i + 12 - (y & 1);
		return Blocked;
	default:
		return i;
	}
}

static void snode(short unsigned from, direction_s d, short unsigned cost) {
	auto index = to(from, d);
	if(index == Blocked)
		return;
	if(path[index] >= Blocked)
		return;
	auto a = path[index];
	if(a != 0 && cost >= a)
		return;
	path_stack[path_push++] = index;
	path[index] = cost;
}

static void wave(short unsigned start) {
	path_push = 0;
	path_pop = 0;
	if(start == Blocked)
		return;
	// Clear all path map
	memset(path, 0, sizeof(path));
	// Block units
	for(auto& e : units) {
		if(!e)
			continue;
		auto i = e.getpos();
		if(i == Blocked)
			continue;
		path[i] = Blocked;
	}
	// Start wave
	path_stack[path_push++] = start;
	path[start] = 1;
	while(path_push != path_pop) {
		auto pos = path_stack[path_pop++];
		auto cost = path[pos] + 1;
		if(cost >= Blocked - 1024)
			break;
		for(auto d : all_around)
			snode(pos, d, cost);
	}
	path_pop = 0;
	path_push = 0;
	path_goal = Blocked;
	path_start = start;
}

static void battlemove(battleimage& e) {
	current_unit = &e;
	wave(e.getpos());
	while(ismodal()) {
		paint_screen();
		standart_input();
		domodal();
		update_drawables();
	}
}

static void makebattle() {
	static speed_s speeds[] = {UltraFastSpeed, VeryFastSpeed, FastSpeed, AverageSpeed, SlowSpeed, VerySlowSpeed, CrawlingSpeed};
	while(!isend()) {
		for(auto s : speeds) {
			for(auto& e : units) {
				if(!e)
					continue;
				if(e.is(Moved))
					continue;
				if(e.get(Speed) < s)
					continue;
				battlemove(e);
			}
		}
		for(auto& e : units) {
			if(!e)
				continue;
			e.refresh();
		}
	}
}

static void prepare_battle() {
	battle.grid = true;
	battle.cursor = true;
	battle.movement = true;
	battle.index = false;
	battle.distance = true;
}

void heroi::battlestart() {
	prepare_battle();
	makebattle();
}