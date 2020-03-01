#include "view.h"

using namespace draw;

const int				awd = 11, ahd = 9;
static heroi*			attacker;
static heroi*			defender;
static bool				defender_can_escape;
static unitai*		current_unit;
static unsigned short	hilite_index;
static unitai*		hilite_unit;
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

static adat<unitai*, 32>	drawables;
static unitai				attacker_image, defender_image;
static short unsigned		position_wide[2][5] = {{0, 22, 44, 66, 88}, {10, 32, 54, 76, 98}};

variant::variant(const uniti* v) : type(Unit), value((unitai*)v - bsmeta<unitai>::elements) {}

uniti* variant::getunit() const {
	return bsmeta<unitai>::elements + value;
}

static unsigned getanimationspeed() {
	switch(battle.speed) {
	case 1: return 70;
	case 2: return 30;
	default: return 140;
	}
}

static point i2h(short unsigned index) {
	int x = 20 + 88 - ((index / awd) % 2 ? cell_wd / 2 : 0) + (cell_wd - 1) * (index % awd);
	int y = 20 + 85 + ((cell_hd / 4) * 3 - 1) * (index / awd);
	return{(short)x, (short)y};
}

void unitai::setpos(short unsigned v) {
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
		gnext(uniti::to(pos, LeftDown), level, n);
		gnext(uniti::to(pos, LeftUp), level, n);
		gnext(uniti::to(pos, RightDown), level, n);
		gnext(uniti::to(pos, RightUp), level, n);
		gnext(uniti::to(pos, Left), level, n);
		gnext(uniti::to(pos, Right), level, n);
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

static void add_squad(short unsigned index, squadi& squad, heroi* leader) {
	unitai* p = 0;
	for(auto& e : bsmeta<unitai>()) {
		if(e)
			continue;
		p = &e;
		break;
	}
	if(!p)
		p = bsmeta<unitai>::add();
	if(!p)
		return;
	p->clear();
	p->setup(squad, leader);
	p->type = Monster;
	p->monster = p->unit;
	p->flags = uniti::isattacker(leader) ? 0 : AFMirror;
	p->source = &squad;
	p->leader = leader;
	p->setpos(index);
	p->set(Wait);
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
	drawables.clear();
	for(auto& e : bsmeta<unitai>()) {
		if(!e)
			continue;
		drawables.add(&e);
	}
	drawables.add(&attacker_image);
	drawables.add(&defender_image);
}

static bool isend() {
	heroi* leader = 0;
	for(auto& e : bsmeta<unitai>()) {
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

static void prepare_leader(unitai& e, heroi* hero, bool defender) {
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

static void paint_grid(const unitai* current_unit) {
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

static void clear_board() {
	bsmeta<unitai>::count = 0;
	drawables.clear();
}

void heroi::setup_battle(heroi* enemy) {
	clear_board();
	attacker = this;
	defender = enemy;
	add_squad(*this, this);
	add_squad(*enemy, enemy);
	prepare_background(Dirt, true);
	prepare_leader(attacker_image, attacker, false);
	prepare_leader(defender_image, defender, true);
}

static void end_turn() {
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

static void command_move() {
	current_unit->move(hot::param);
	end_turn();
}

static void command_shoot() {
	auto& attacker = *current_unit;
	auto& defender = *hilite_unit;
	attacker.shoot(defender);
	end_turn();
}

static void command_attack() {
	auto& attacker = *current_unit;
	auto& defender = *hilite_unit;
	attacker.move(attack_index);
	attacker.melee(defender, attack_direction);
	end_turn();
}

static void show_info() {
	auto p = (uniti*)hot::param;
	p->show(p->leader, false, false, false, p);
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
	auto p1 = *((unitai**)v1);
	auto p2 = *((unitai**)v2);
	auto y1 = p1->getz();
	auto y2 = p2->getz();
	if(y1 != y2)
		return y1 - y2;
	if(p1->pos.x != p2->pos.x)
		return p1->pos.x - p2->pos.x;
	return (p1 < p2) ? -1 : 1;
}

static void normalize_drawables() {
	qsort(drawables.data, drawables.count, sizeof(drawables.data[0]), drawable_compare);
}

static bool update_drawables(unitai* p, bool update_wait) {
	for(auto pe : drawables) {
		if(pe != p) {
			if(!update_wait && pe->iswait())
				continue;
			pe->update();
		}
	}
	if(p)
		return p->increment();
	return false;
}

static void paint_drawables(const unitai* current) {
	for(auto pe : drawables) {
		pe->paint();
		if(pe == current)
			pe->stroke();
	}
}

static void hittest_drawable() {
	hilite_unit = 0;
	for(auto p : drawables) {
		if(hilite_index != Blocked && p->index == hilite_index)
			hilite_unit = p;
		else if(p->type == Hero) {
			if(mousein({p->pos.x - 32, p->pos.y - 96, p->pos.x + 32, p->pos.y + 8}))
				hilite_unit = p;
		}
	}
}

static void paint_screen(const unitai* current) {
	normalize_drawables();
	hittest_grid();
	hittest_drawable();
	paint_field();
	paint_grid(current);
	paint_drawables(current);
}

static void hero_options() {
	auto p = (heroi*)hot::param;
	p->battlemenu((p == attacker) || defender_can_escape);
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

static void cast_spells() {
	auto p = (heroi*)hot::param;
	p->castcombatspell();
}

static void creature_info(uniti* p) {
	auto fev = (hot::key == MouseLeft && hot::pressed);
	setcursor(CMSECO, 5, {-7, -7});
	status("%1i %2", p->count, bsmeta<monsteri>::elements[p->unit].multiname);
	if(fev)
		execute(show_info, (int)p);
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
					execute(command_shoot, (int)p);
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
						execute(command_attack, (int)p);
					}
				} else
					creature_info(p);
			} else
				creature_info(p);
		}
	} else if(hilite_index != Blocked && current_unit) {
		int a = getcost(hilite_index) - 1;
		if(a <= m) {
			if(current_unit->is(Fly))
				setcursor(CMSECO, 2, {-7, -14});
			else
				setcursor(CMSECO, 1, {-7, -14});
			if(fev)
				execute(command_move, hilite_index);
		} else
			setcursor(CMSECO, 0, {-7, -7});
	}
	switch(hot::key) {
	case Alpha + 'C': execute(cast_spells, (int)current_unit->leader); break;
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
	for(auto& e : bsmeta<unitai>()) {
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

static void battlemove(unitai& e) {
	current_unit = &e;
	while(ismodal()) {
		paint_screen(current_unit);
		standart_input();
		domodal();
		if(hot::key == InputTimer)
			update_drawables(0, true);
	}
}

static void makebattle() {
	static speed_s speeds[] = {UltraFastSpeed, VeryFastSpeed, FastSpeed, AverageSpeed, SlowSpeed, VerySlowSpeed, CrawlingSpeed};
	while(!isend()) {
		for(auto s : speeds) {
			for(auto& e : bsmeta<unitai>()) {
				if(!e.isalive())
					continue;
				if(e.isparalized())
					continue;
				if(e.is(Moved))
					continue;
				if(e.get(Speed) < s)
					continue;
				prepare_drawables();
				wave(e.getpos(), e.is(Fly));
				if(e.testmorale() == -1)
					e.show_morale(false);
				else {
					if(e.ishuman())
						battlemove(e);
					else
						e.automove();
				}
				e.set(Moved);
				if(isend())
					return;
			}
		}
		for(auto& e : bsmeta<unitai>()) {
			if(!e.isalive())
				continue;
			e.refresh();
		}
		uniti::exhausespells();
		attacker->remove(Moved);
		defender->remove(Moved);
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
	for(auto& e : bsmeta<unitai>()) {
		if(!e.isalive())
			continue;
		if(e.getpos() == index)
			return &e;
	}
	return 0;
}

void unitai::animate(int frames) {
	if(iswait())
		return;
	if(start == 0 && animation::count == 0)
		return;
	if(frames == -1)
		frames = animation::count;
	frames += start;
	if(frame >= frames)
		return;
	auto speed = getanimationspeed();
	while(frame < frames) {
		paint_screen(0);
		updatescreen();
		sleep(speed);
		if(update_drawables(this, false))
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

static int getdistance(point p1, point p2) {
	auto dx = p1.x - p2.x;
	auto dy = p1.y - p2.y;
	return isqrt(dx*dx + dy * dy);
}

void unitai::animate(point goal, int velocity) {
	if(type == Monster && (!isalive() || iswait()))
		return;
	auto frames = start + animation::count;
	if(frame >= frames)
		return;
	auto p1 = pos;
	auto distance_maximum = getdistance(p1, goal);
	if(distance_maximum <= 0)
		return;
	auto distance = 0;
	distance += velocity;
	while(distance < distance_maximum) {
		pos.x = p1.x + (goal.x - p1.x)*distance / distance_maximum;
		pos.y = p1.y + (goal.y - p1.y)*distance / distance_maximum;
		paint_screen(0);
		updatescreen();
		sleep(getanimationspeed());
		if(update_drawables(this, false))
			frame = start;
		distance += velocity;
	}
}

static int missile9(int dx, int dy) {
	if(0 == dx)
		return dy > 0 ? 0 : 8;
	int tan = iabs(1000 * dy / dx);
	if(tan <= 60)
		return 4;
	else if(tan <= 577) // tan 30: 0 - 30
		return dy > 0 ? 3 : 5;
	else if(tan >= 1732) // tan 60: 90 - 60
		return dy > 0 ? 1 : 7;
	// tan 45: 30 - 60
	return dy > 0 ? 2 : 6;
}

static int missile7(int dx, int dy) {
	if(0 == dx)
		return dy > 0 ? 0 : 6;
	else if(0 == dy)
		return 3;
	int tan = iabs(1000 * dy / dx);
	// tan 45: 0 - 45
	if(1000 >= tan)
		return dy > 0 ? 2 : 4;
	// 45 - 90
	return dy > 0 ? 1 : 5;
}

static int get_missile_index(res_s icn, int dx, int dy) {
	switch(getframecount(icn)) {
	case 9: return missile9(dx, dy);
	case 7: return missile7(dx, dy);
	default: return 0;
	}
}

void uniti::show_shoot(uniti& enemy) const {
	if(!enemy.isalive() || !isalive())
		return;
	auto pa = (unitai*)this;
	auto pe = (unitai*)&enemy;
	const auto d = getdirection(getpos(), enemy.getpos());
	pa->set(d);
	pa->set(Shoot, pa->getparam(d));
	auto long_animation = (pa->start != 0);
	auto shoot_finish = -2;
	switch(unit) {
	case WarTroll: case Troll:
		shoot_finish = -1;
		break;
	}
	if(long_animation) {
		pa->set(Shoot);
		pa->animate();
		pa->set(Shoot, pa->getparam(d));
		pa->animate(pa->animation::count - shoot_finish);
	} else {
		pa->set(Shoot);
		pa->animate(pa->animation::count - shoot_finish);
	}
	// Выпускание снаряда
	const int shoot_height = 50;
	unitai arrow; arrow.clear();
	arrow.pos = pa->getbreast();//pa->getlaunch(pa->monster, d);
	point target = pe->getbreast();
	arrow.res = pa->getmissile(pa->monster);
	arrow.frame = get_missile_index(arrow.res, arrow.pos.x - target.x, arrow.pos.y - target.y);
	arrow.start = arrow.frame;
	arrow.flags = pa->flags;
	arrow.animation::count = 1;
	drawables.add(&arrow);
	arrow.animate(target, 44);
	// Оставшаяся анимация
	pa->animate();
	prepare_drawables();
}

void heroi::show_cast(bool mass) const {
	auto pa = (attacker == this) ? &attacker_image : &defender_image;
	pa->set(mass ? Throw : Cast);
	pa->animate();
	pa->freezy(4);
}

void uniti::show_morale(bool good) const {
	auto pa = (unitai*)this;
	unitai e; e.clear();
	e.animation::set(good ? MORALEG : MORALEB, 0, pa->pos);
	e.pos.y -= 32;
	e.animation::count = getframecount(e.res);
	drawables.add(&e);
	e.animate();
	prepare_drawables();
}

void uniti::show_effect(spell_s v) const {
	auto pa = (unitai*)this;
	unitai e; e.clear();
	e.type = Spell; e.spell = v;
	switch(v) {
	case Bless: e.set(BLESS, pa->gethead()); break;
	case Blind: e.set(BLIND, pa->gethead()); break;
	case Cure: e.set(MAGIC01, pa->pos + point{0, -getheight(MAGIC01, 0) / 3}); break;
	case Curse: e.set(CURSE, pa->pos); break;
	case DragonSlayer: e.set(DRAGSLAY, pa->gethead()); break;
	case Haste: e.set(HASTE, pa->pos + getcenter(HASTE, 0)); break;
	case Paralyze: e.set(PARALYZE, pa->gethead()); break;
	case SteelSkin: e.set(STELSKIN, pa->pos); break;
	case StoneSkin: e.set(STONSKIN, pa->pos); break;
	}
	if(e.res != NoRes)
		drawables.add(&e);
	e.animate();
	prepare_drawables();
}

void uniti::show_attack(const uniti& enemy, direction_s d) const {
	if(!enemy.isalive() || !isalive())
		return;
	auto pa = (unitai*)this;
	pa->set(d);
	pa->set(AttackAction);
	pa->animate();
	pa->set(AttackAction, pa->getparam(d));
	pa->animate(pa->animation::count - 2);
}

void uniti::show_damage() const {
	auto pa = (unitai*)this;
	if(count <= 0)
		pa->set(Killed);
	else
		pa->set(Damaged);
	pa->animate();
}

short unsigned uniti::getnextmove(short unsigned index, short unsigned points) const {
	route_path(index);
	if(!path_push)
		return Blocked;
	auto i = getpos();
	path_push--;
	while(path_push != 0) {
		path_push--;
		auto i1 = path_stack[path_push];
		if(i1 == Blocked)
			break;
		auto cost = getcost(i1);
		if(cost > points)
			break;
		i = i1;
	}
	return i;
}

void uniti::show_move(short unsigned index) const {
	route_path(index);
	if(!path_push)
		return;
	auto pa = (unitai*)this;
	auto i = getpos();
	pa->set(Move);
	path_push--;
	int distance = 0;
	auto speed = getanimationspeed();
	speed = speed * 2 / 3;
	while(path_push != 0) {
		path_push--;
		auto i1 = path_stack[path_push];
		if(i1 == Blocked)
			break;
		auto d = uniti::getdirection(i, i1);
		pa->set(d);
		pa->flags |= AFMoving;
		auto p0 = i2h(i);
		auto p1 = i2h(i1);
		auto md = 0;
		auto sf = 0;
		if(d != Left && d != Right) {
			auto pf = draw::get(pa->res, pa->frame);
			pa->flags |= AFNoOffset;
			md = pa->start + pa->animation::count - pa->frame;
			sf = pa->frame;
		}
		while(true) {
			if(md) {
				auto pf = draw::get(pa->res, pa->frame);
				pa->pos.x = p0.x + (pa->frame - sf) * (p1.x - p0.x) / md;
				if(pa->flags&AFMirror)
					pa->pos.x += pf->width / 2;
				else
					pa->pos.x -= pf->width / 2;
				pa->pos.y = p0.y + (pa->frame - sf) * (p1.y - p0.y) / md + pf->y;
			}
			paint_screen(0);
			updatescreen();
			sleep(speed);
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
	auto pa = (unitai*)this;
	auto p2 = i2h(goal);
	pa->set(getdirection(index, goal));
	pa->flags |= AFMoving;
	pa->set(FlyAction, 0);
	pa->animate();
	pa->set(FlyAction, 1);
	pa->animate(p2, 16);
	pa->set(FlyAction, 2);
	pa->animate();
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

static int getframe(spell_s id) {
	switch(id) {
	case Slow: return 1;
	case Curse: return 3;
	case LightingBolt: return 4;
	case ChainLighting: return 5;
	case Cure: return 6;
	case Bless: return 7;
	case FireBall: return 8;
	case FireBlast: return 9;
	case Teleport: return 10;
	case ElementalStorm: return 0x0B;
	case RessurectTrue: return 0x0C;
	case Ressurect: return 0x0D;
	case Haste: return 0x0E;
	case Shield: return 0x0F;
	case Armagedon: return 0x10;
	case Antimagic: return 0x11;
	case Dispel: return 0x12;
	case Berserker: return 0x13;
	case Paralyze: return 0x14;
	case Blind: return 0x15;
	case HolyWord: return 0x16;
	case HolyShout: return 0x17;
	case MeteorShower: return 0x18;
	case AnimateDead: return 0x19;
	case MirrorImage: return 0x1A;
	case BloodLust: return 0x1B;
	case DeathRipple: return 0x1C;
	case DeathWave: return 0x1D;
	case SteelSkin: return 0x1E;
	case StoneSkin: return 0x1F;
	case DragonSlayer: return 0x20;
	case Earthquake: return 0x21;
	case DisruptingRay: return 0x22;
	case ColdRing: return 0x23;
	case ColdRay: return 0x24;
	case Hypnotize: return 0x25;
	case MagicArrow: return 0x26;
	default: return 0;
	}
}

bool heroi::choose(spell_s id, variant& result) const {
	result.clear();
	prepare_drawables();
	while(ismodal()) {
		paint_screen(current_unit);
		setcursor(SPELLS, 0);
		result.clear();
		if(hilite_unit)
			result = static_cast<uniti*>(hilite_unit);
		if(result &&  cast(id, result, false)) {
			auto fr = getframe(id);
			setcursor(SPELLS, getframe(id));
		}
		domodal();
		switch(hot::key) {
		case InputTimer:
			update_drawables(0, true);
			break;
		case MouseLeft:
			if(hot::pressed)
				breakmodal((result.type != NoVariant) ? 1 : 0);
			break;
		}
	}
	hot::key = 0;
	hot::param = 0;
	return getresult() != 0;
}

void heroi::castcombatspell() {
	if(is(Moved)) {
		message("Заклинания можно использовать только один раз за ход.");
		return;
	}
	spell_s spell;
	if(showbook(CombatSpell, &spell)) {
		variant target;
		if(choose(spell, target)) {
			if(cast(spell, target, true))
				set(SpellPoints, get(SpellPoints) - getcost(spell));
			set(Moved);
		}
	}
}

void uniti::addmorale(const heroi* leader, int value) {
	for(auto& e : bsmeta<unitai>()) {
		if(!e)
			continue;
		if(e.leader == leader)
			e.morale += value;
	}
}

unsigned uniti::getscore(short unsigned index) {
	if(index == Blocked)
		return 0;
	if(getcost(index) == Blocked)
		return 0;
	unsigned cs = 0;
	for(auto d : all_around) {
		auto i1 = to(index, d);
		auto pu = find(i1);
		if(!pu)
			continue;
		if(pu->isarcher())
			cs++;
	}
	return cs;
}

direction_s uniti::getattackpos(short unsigned index) {
	short unsigned i1 = Blocked;
	unsigned s1 = 0;
	direction_s d1 = Up;
	for(auto e : all_around) {
		auto i = to(index, e);
		if(i == Blocked)
			continue;
		auto s = getscore(i);
		if(!s)
			continue;
		if(i1 == Blocked || s1 < s) {
			i1 = i;
			s1 = s;
			d1 = e;
		}
	}
	return d1;
}

bool uniti::isclose(short unsigned index) const {
	auto i = getpos();
	for(auto d : all_around) {
		auto i1 = to(i, d);
		if(i1 == index)
			return true;
	}
	return false;
}

//direction_s uniti::getdirection(short unsigned index) const {
//	auto i = getpos();
//	for(auto d : all_around) {
//		auto i1 = to(i, d);
//		if(i1 == index)
//			return true;
//	}
//	return false;
//}