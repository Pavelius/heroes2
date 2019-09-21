#include "view.h"

using namespace draw;

const int				awd = 11, ahd = 9;
static heroi*			attacker;
static heroi*			defender;
static battleimage*		current_unit;
static unsigned short	hilite_index;
static battleimage*		hilite_unit;
battlei					battle;

static unsigned short	attack_index;
static direction_s		attack_direction;
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
static battleimage*		drawables[20];
static battleimage		attacker_image, defender_image;
static unsigned			drawables_count;
static short unsigned	position_wide[2][5] = {{0, 22, 44, 66, 88}, {10, 32, 54, 76, 98}};

static unsigned getanimationspeed() {
	switch(battle.speed) {
	case 1: return 90;
	case 2: return 50;
	default: return 180;
	}
}

static unsigned getmovespeed() {
	return getanimationspeed() / 2;
}

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
	if(index == Blocked)
		return 0;
	return path[index];
}

static void gnext(short unsigned index, unsigned& level, short unsigned& pos) {
	if(index == Blocked)
		return;
	auto nlevel = path[index];
	if(!nlevel)
		return;
	if(nlevel < level) {
		level = nlevel;
		pos = index;
	}
}

static void route_path(short unsigned goal) {
	path_push = 0;
	path_goal = Blocked;
	auto pos = goal;
	unsigned level = BlockedPath;
	path_stack[path_push++] = goal;
	while(pos != path_start) {
		auto n = pos;
		gnext(uniti::to(pos, Left), level, n);
		gnext(uniti::to(pos, Right), level, n);
		gnext(uniti::to(pos, LeftDown), level, n);
		gnext(uniti::to(pos, LeftUp), level, n);
		gnext(uniti::to(pos, RightDown), level, n);
		gnext(uniti::to(pos, RightUp), level, n);
		if(pos == n)
			return;
		pos = n;
		path_stack[path_push++] = n;
		level = path[pos];
	}
	path_goal = goal;
}

bool uniti::isattacker(const heroi* hero) {
	return hero == attacker;
}

static int getside(const heroi* leader) {
	return (attacker == leader) ? 0 : 1;
}

static battleimage* find_image(const uniti* p) {
	auto pe = (battleimage*)p;
	if(pe >= units && pe < units + sizeof(units) / sizeof(units[0]))
		return pe;
	return 0;
}

static void add_drawable(battleimage* p) {
	if(drawables_count >= sizeof(drawables) / sizeof(drawables[0]))
		return;
	if(find_image(p))
		return;
	drawables[drawables_count++] = p;
}

static void add_squad(short unsigned index, squadi& squad, heroi* leader) {
	for(auto& e : units) {
		if(e)
			continue;
		e.clear();
		e.setup(squad, leader);
		e.type = Monster;
		e.monster = e.unit;
		e.flags = uniti::isattacker(leader) ? 0 : AFMirror;
		e.source = &squad;
		e.leader = leader;
		e.setpos(index);
		e.set(Wait);
		add_drawable(&e);
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

static void prepare_drawables() {
	auto ps = drawables;
	for(auto& e : units) {
		if(!e)
			continue;
		*ps++ = &e;
	}
	*ps++ = &attacker_image;
	*ps++ = &defender_image;
	drawables_count = ps - drawables;
}

static bool isend() {
	heroi* leader = 0;
	for(auto& e : units) {
		if(!e.isalive())
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

static void paint_grid(const battleimage* current_unit) {
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
	if(battle.cursor && current_unit) {
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
	for(auto& e : (::units))
		e.clear();
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
	battle.show();
}

static void start_autocombat() {

}

static void move_unit() {
	current_unit->move(hot::param);
	end_turn();
}

static void shoot_enemy() {
	auto& attacker = *current_unit;
	auto& defender = *hilite_unit;
	attacker.shoot(defender);
	end_turn();
}

static void attack_enemy() {
	auto& attacker = *current_unit;
	auto& defender = *hilite_unit;
	attacker.move(attack_index);
	attacker.melee(defender, attack_direction);
	end_turn();
}

static void show_info() {
	auto p = (uniti*)hot::param;
	p->show(p->leader, false, false, false);
}

static void hittest_grid() {
	hilite_index = Blocked;
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

static direction_s hex_direction(const point hp, const point pt) {
	const int INFL = 12;
	point coord[7] = {hp,
	{hp.x, hp.y - cell_hd * INFL / 2}, // u
	{(short)(hp.x + cell_wd * INFL / 2), (short)(hp.y - cell_hr * INFL)}, // ru
	{(short)(hp.x + cell_wd * INFL / 2), (short)(hp.y + cell_hr * INFL)}, // rd
	{(short)hp.x, (short)(hp.y + cell_hd * INFL / 2)}, // d
	{(short)(hp.x - cell_wd * INFL / 2), (short)(hp.y + cell_hr * INFL)}, // ld
	{(short)(hp.x - cell_wd * INFL / 2), (short)(hp.y - cell_hr * INFL)}, // lu
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

static int drawable_compare(const void* v1, const void* v2) {
	auto p1 = *((battleimage**)v1);
	auto p2 = *((battleimage**)v2);
	auto y1 = p1->getz();
	auto y2 = p2->getz();
	if(y1 != y2)
		return y1 - y2;
	if(p1->pos.x != p2->pos.x)
		return p1->pos.x - p2->pos.x;
	return (p1 < p2) ? -1 : 1;
}

static void normalize_drawables() {
	qsort(drawables, drawables_count, sizeof(drawables[0]), drawable_compare);
}

static bool update_drawables(battleimage* p) {
	for(unsigned i = 0; i < drawables_count; i++) {
		if(drawables[i] == p)
			continue;
		drawables[i]->update();
	}
	if(p)
		return p->increment();
	return false;
}

static void paint_drawables(const battleimage* current) {
	for(unsigned i = 0; i < drawables_count; i++) {
		drawables[i]->paint();
		if(drawables[i] == current)
			drawables[i]->stroke();
	}
}

static void hittest_drawable() {
	hilite_unit = 0;
	for(unsigned i = 0; i < drawables_count; i++) {
		auto p = drawables[i];
		if(hilite_index != Blocked && p->index == hilite_index)
			hilite_unit = p;
		else if(p->type == Hero) {
			if(mousein({p->pos.x - 32, p->pos.y - 96, p->pos.x + 32, p->pos.y + 8}))
				hilite_unit = p;
		}
	}
}

static void paint_screen(const battleimage* current) {
	normalize_drawables();
	hittest_grid();
	hittest_drawable();
	paint_field();
	paint_grid(current);
	paint_drawables(current);
}

//case Fly:
//	icn = res::CMSECO;
//	start = 2;
//	pos.x = -7;
//	pos.y = -14;
//	break;

static void hero_options() {
	auto p = (heroi*)hot::param;
	p->battlemenu(true, false);
}

static void setattack(direction_s d) {
	switch(d) {
	case Left: setcursor(CMSECO, 8, {-25, -7}); break;
	case Right: setcursor(CMSECO, 11, {-5, -7}); break;
	case LeftUp: setcursor(CMSECO, 9, {-20, -20}); break;
	case LeftDown: setcursor(CMSECO, 7, {-20, -5}); break;
	case RightUp: setcursor(CMSECO, 10, {-5, -20}); break;
	case RightDown: setcursor(CMSECO, 12, {-5, -5}); break;
	}
}

static void standart_input() {
	auto fev = (hot::key == MouseLeft && hot::pressed);
	int m = current_unit->get(Speed) + 1;
	auto pu = hilite_unit;
	if(pu && pu->type == Monster && !pu->isalive())
		pu = 0;
	if(pu) {
		if(pu->type == Hero) {
			auto p = pu->gethero();
			if(p) {
				status(p->getname());
				setcursor(CMSECO, 4, {-7, -7});
				if(fev)
					execute(hero_options, (int)p);
			}
		} else if(pu->type == Monster) {
			uniti* p = pu;
			if(current_unit->isenemy(p) && current_unit->canshoot()) {
				setcursor(CMSECO, 3, {-7, -7});
				status("Стрелять в %1i %2", p->count, bsmeta<monsteri>::elements[p->unit].multiname);
				if(fev)
					execute(shoot_enemy, (int)p);
			} else if(current_unit->isenemy(p)) {
				auto ad = hex_direction(i2h(hilite_index), hot::mouse);
				auto bd = uniti::to(ad, Down);
				auto bi = uniti::to(hilite_index, ad);
				int a = getcost(bi) - 1;
				if(bi != Blocked && a <= m) {
					setattack(ad);
					status("Атаковать %2", p->count, bsmeta<monsteri>::elements[p->unit].multiname);
					if(fev) {
						attack_index = bi;
						attack_direction = ad;
						execute(attack_enemy, (int)p);
					}
				} else
					setcursor(CMSECO, 0, {-7, -7});
			} else {
				setcursor(CMSECO, 5, {-7, -7});
				status("%1i %2", p->count, bsmeta<monsteri>::elements[p->unit].multiname);
				if(fev)
					execute(show_info, (int)p);
			}
		}
	} else if(hilite_index != Blocked && current_unit) {
		int a = getcost(hilite_index) - 1;
		if(a <= m) {
			setcursor(CMSECO, 1, {-7, -14});
			if(fev)
				execute(move_unit, hilite_index);
		} else
			setcursor(CMSECO, 0, {-7, -7});
	}
}

static void snode(short unsigned from, direction_s d, short unsigned cost) {
	auto index = uniti::to(from, d);
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

static void wave(short unsigned start, bool fly) {
	path_push = 0;
	path_pop = 0;
	if(start == Blocked)
		return;
	// Clear all path map
	memset(path, 0, sizeof(path));
	// Block units
	for(auto& e : units) {
		if(!e.isalive())
			continue;
		auto i = e.getpos();
		if(i == Blocked)
			continue;
		path[i] = Blocked;
	}
	if(fly) {
		for(auto& e : path) {
			if(e == Blocked)
				continue;
			e = 1;
		}
	} else {
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
	}
	path_pop = 0;
	path_push = 0;
	path_goal = Blocked;
	path_start = start;
}

static void battlemove(battleimage& e) {
	current_unit = &e;
	prepare_drawables();
	wave(e.getpos(), e.is(Fly));
	while(ismodal()) {
		paint_screen(current_unit);
		standart_input();
		domodal();
		if(hot::key == InputTimer)
			update_drawables(0);
	}
}

static void makebattle() {
	static speed_s speeds[] = {UltraFastSpeed, VeryFastSpeed, FastSpeed, AverageSpeed, SlowSpeed, VerySlowSpeed, CrawlingSpeed};
	while(!isend()) {
		for(auto s : speeds) {
			for(auto& e : units) {
				if(!e.isalive())
					continue;
				if(e.is(Moved))
					continue;
				if(e.get(Speed) < s)
					continue;
				battlemove(e);
			}
		}
		for(auto& e : units) {
			if(!e.isalive())
				continue;
			e.refresh();
		}
	}
}

static void prepare_battle() {
	battle.grid = false;
	battle.cursor = true;
	battle.movement = true;
	battle.index = false;
	battle.distance = false;
}

void heroi::battlestart() {
	prepare_battle();
	makebattle();
}

uniti* uniti::find(short unsigned index) {
	for(auto& e : units) {
		if(!e.isalive())
			continue;
		if(e.getpos() == index)
			return &e;
	}
	return 0;
}

void battleimage::animate(int frames, int speed) {
	if(!isalive() || iswait())
		return;
	if(frames == -1)
		frames = animation::count;
	frames += start;
	if(frame >= frames)
		return;
	if(speed == -1)
		speed = getanimationspeed();
	while(frame < frames) {
		paint_screen(0);
		updatescreen();
		sleep(speed);
		if(update_drawables(this))
			break;
	}
}

static int isqrt(int num) {
	int res = 0;
	int bit = 1 << 30;
	// "bit" starts at the highest power of four <= the argument.
	while(bit > num)
		bit >>= 2;
	while(bit != 0) {
		if(num >= res + bit) {
			num -= res + bit;
			res = (res >> 1) + bit;
		} else
			res >>= 1;
		bit >>= 2;
	}
	return res;
}

int getdistance(point p1, point p2) {
	auto dx = p1.x - p2.x;
	auto dy = p1.y - p2.y;
	return isqrt(dx*dx + dy * dy);
}

void battleimage::animate(point goal, int velocity) {
	if(!isalive() || iswait())
		return;
	auto frames = start + animation::count;
	if(frame >= frames)
		return;
	auto p1 = i2h(index);
	auto distance_maximum = getdistance(p1, goal);
	if(distance_maximum<=0)
		return;
	auto distance = 0;
	add_drawable(this);
	distance += velocity;
	while(distance < distance_maximum) {
		pos.x = p1.x + (goal.x - p1.x)*distance / distance_maximum;
		pos.y = p1.y + (goal.y - p1.y)*distance / distance_maximum;
		paint_screen(0);
		updatescreen();
		sleep(getmovespeed());
		if(update_drawables(this))
			frame = start;
		distance += velocity;
	}
}

void uniti::show_shoot(uniti& enemy) const {
	auto pa = (battleimage*)this;
	auto pe = (battleimage*)&enemy;
	const auto d = Right;
	pa->set(d);
	pa->set(Shoot);
	pa->animate();
	if(d == Left || d == Right)
		pa->set(Shoot, 2);
	else if(d == LeftUp || d == RightUp)
		pa->set(Shoot, 1);
	else
		pa->set(Shoot, 3);
	pa->animate();
	pa->setdefault();
}

void uniti::show_attack(uniti& enemy, direction_s d, bool destroy_enemy) const {
	if(!enemy.isalive() || !isalive())
		return;
	auto pa = (battleimage*)this;
	auto pe = find_image(&enemy);
	pa->set(d);
	pa->set(AttackAction);
	pa->animate();
	if(d == Left || d == Right)
		pa->set(AttackAction, 2);
	else if(d == LeftUp || d == RightUp)
		pa->set(AttackAction, 1);
	else
		pa->set(AttackAction, 3);
	pa->animate(pa->animation::count - 2);
	if(destroy_enemy)
		pe->set(Killed);
	else
		pe->set(Damaged);
	pa->animate(-1); pa->setdefault();
	pe->animate(-1); pe->setdefault();
}

void uniti::show_damage(bool destroy) const {
	auto pa = (battleimage*)this;
	if(destroy)
		pa->set(Killed);
	else
		pa->set(Damaged);
	pa->animate();
}

void uniti::show_move(short unsigned index) const {
	route_path(index);
	if(!path_push)
		return;
	auto pa = (battleimage*)this;
	auto i = getpos();
	pa->set(Move);
	path_push--;
	int distance = 0;
	while(path_push != 0) {
		path_push--;
		auto i1 = path_stack[path_push];
		if(i1 == Blocked)
			break;
		auto d = uniti::getdirection(i, i1);
		pa->set(d);
		pa->flags |= AFMoving;
		while(true) {
			paint_screen(0);
			updatescreen();
			sleep(getmovespeed());
			if(pa->increment()) {
				pa->frame = pa->start + 1;
				break;
			}
		}
		pa->pos = i2h(i1);
		i = i1;
	}
	pa->setdefault();
}

void uniti::show_fly(short unsigned goal) const {
	auto pa = (battleimage*)this;
	auto p2 = i2h(goal);
	pa->set(getdirection(index, goal));
	pa->flags |= AFMoving;
	pa->set(FlyAction, 0);
	pa->animate(-1, getanimationspeed()/2);
	pa->set(FlyAction, 1);
	pa->animate(p2, 16);
	pa->set(FlyAction, 2);
	pa->animate(-1, getanimationspeed() / 2);
	pa->setdefault();
	pa->setpos(goal);
}

direction_s uniti::getdirection(short unsigned from, short unsigned to) {
	int x1 = from % awd;
	int y1 = from / awd;
	int x2 = to % awd;
	int y2 = to / awd;
	if(y2 == y1) {
		if(x2 == x1)
			return Up;
		return (x2 > x1) ? Right : Left;
	}
	point p1 = i2h(from);
	point p2 = i2h(to);
	if(y2 < y1)
		return (p2.x < p1.x) ? LeftUp : RightUp;
	return (p2.x < p1.x) ? LeftDown : RightDown;
}