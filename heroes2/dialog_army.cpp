#include "view.h"

using namespace draw;

const int				ox = 10;
const int				oy = 18;
static squadi*			current_unit;
static const heroi*		current_hero;

int index_by_type(kind_s id) {
	switch(id) {
	case Necromancer: return 9;
	case Wizard: return 8;
	case Warlock: return 7;
	case Sorcerer: return 6;
	case Barbarian: return 5;
	case Knight: return 4;
	default: return 10;
	}
}

static void unit_info() {
	((squadi*)hot::param)->information(current_hero);
}

void unit_info_monster() {
	squadi sq;
	sq.unit = (monster_s)hot::param;
	sq.count = 0;
	sq.information(0);
}

static void unit_edit() {
	auto p = current_unit;
	current_unit = 0;
	p->show(current_hero, false, true, true);
}

static void choose_unit() {
	auto s1 = *((squadi*)hot::param);
	if(current_unit) {
		*((squadi*)hot::param) = *current_unit;
		*current_unit = s1;
		current_unit = 0;
	} else {
		if(hot::param && ((squadi*)hot::param)->count)
			current_unit = (squadi*)hot::param;
	}
}

static void upgrade_unit() {
	current_unit = (squadi*)hot::param;
}

static void dismiss_unit() {
	auto u = (squadi*)hot::param;
	char temp[260];
	zprint(temp, "Вы действительно хотите распустить %1i %2?",
		u->count, bsmeta<monsteri>::elements[u->unit].multiname);
	if(playeri::confirm(temp)) {
		u->clear();
		breakmodal(0);
	}
}

void squadi::paint(int x, int y, const heroi* hero, bool allow_change) const {
	auto key = hot::key;
	auto pressed = hot::pressed;
	auto w = getwidth(STRIP, 2);
	auto h = getheight(STRIP, 2);
	if(!*this)
		image(x, y, STRIP, 2);
	else {
		char temp[260];
		image(x, y, STRIP, index_by_type(bsmeta<monsteri>::elements[unit].type));
		image(x, y, res_s(MONH0000 + unit - FirstMonster), 0);
		zprint(temp, "%1i", count);
		draw::state push;
		font = SMALFONT;
		auto w1 = draw::textw(temp);
		auto x1 = x + w - 4 - w1;
		text(x1, y + h - texth(), temp);
	}
	if(current_unit == this)
		image(x, y, STRIP, 1);
	if(mousein({x, y, x + w, y + h})) {
		if(allow_change) {
			switch(key) {
			case MouseRight:
				if(hot::pressed) {
					current_hero = hero;
					execute(unit_info, (int)this);
				}
				break;
			case MouseLeftDBL:
				current_unit = (squadi*)this;
				current_hero = hero;
				execute(unit_edit);
				break;
			case MouseLeft:
				if(pressed) {
					current_hero = hero;
					execute(choose_unit, (int)this);
				}
				break;
			}
		}
		if(*this)
			status("%1i %2", count, bsmeta<monsteri>::elements[unit].multiname);
	}
}

static void army_input_update() {
	if(hot::key == MouseLeft && hot::pressed)
		current_unit = 0;
}

void armyi::paint(int x, int y, const heroi* hero, bool allow_change, bool clean_current_unit) const {
	int w = getwidth(STRIP, 2);
	int h = getwidth(STRIP, 2);
	for(int i = 0; i < 5; i++) {
		units[i].paint(x, y, hero, allow_change);
		x += 88;
	}
	if(clean_current_unit)
		army_input_update();
}

static int field(int x, int y, const char* name, const char* v1) {
	char temp[260]; zprint(temp, "%1:", name);
	text(x - textw(temp), y, temp);
	zprint(temp, "%1", v1);
	text(x + ox, y, temp);
	return oy;
}

static int field(int x, int y, const char* name, int v1, int v2) {
	char temp[260]; zprint(temp, "%1:", name);
	text(x - textw(temp), y, temp);
	if(v1==v2)
		zprint(temp, "%1i", v1);
	else
		zprint(temp, "%2i (%1i)", v1, v2);
	text(x + ox, y, temp);
	return oy;
}

static int fiedm(int x, int y, const char* name, int v1, int v2) {
	if(v1 == v2)
		return field(x, y, name, v1, v1);
	char temp[260]; zprint(temp, "%1:", name);
	text(x - textw(temp), y, temp);
	zprint(temp, "%1i-%2i", v1, v2);
	text(x + ox, y, temp);
	return oy;
}

static int status(int x, int y, const squadi* squad, const heroi* hero) {
	auto y1 = y;
	// Главные статусы
	y += field(x, y, getstr(Attack), squad->get(Attack), squad->get(Attack, hero));
	y += field(x, y, getstr(Defence), squad->get(Defence), squad->get(Defence, hero));
	// shoots
	//if(bsget(rec, Shoots))
	//	y += field(x, y, Shoots, rec, side, szt("Shoots Left", "Выстрелов"));
	// damage
	y += fiedm(x, y, "Урон", squad->get(DamageMin), squad->get(DamageMax));
	// hits
	y += field(x, y, "Жизнь", squad->get(HitPointsMax), squad->get(HitPointsMax, hero));
	//y += field(x, y, HitPoints, rec, side, szt("Hits Left", "Жизнь ост."));
	// speed
	y += field(x, y, "Скорость", getstr((speed_s)squad->get(Speed, hero)));
	if(hero) {
		y += field(x, y, "Мораль", getstr((morale_s)squad->get(MoraleStat, hero)));
		y += field(x, y, "Удача", getstr((luck_s)squad->get(LuckStat, hero)));
	}
	return y - y1;
}

static void effects(int x, int y, const squadi* p) {
	static const int modes[] = {
		BloodLust, Bless, Haste, Shield, StoneSkin,
		DragonSlayer, SteelSkin, Antimagic, Curse, Slow,
		Berserker, Hypnotize, Blind, Paralyze, Stone
	};
	draw::state push;
	draw::font = SMALFONT;
	int dx = 0;
	for(auto sid : modes) {
//		if(!bsget(rec, sid))
//			continue;
//		int f = bsget(sid, Frame);
//		dx += getwidth(SPELLINF, f);
	}
	x = x - dx / 2;
	for(auto sid : modes) {
//		int value = bsget(rec, sid);
//		if(!value)
//			continue;
//		sznum(temp, value);
//		int f = bsget(sid, Frame);
//		draw::image(x, y, res::SPELLINF, f, AFNoOffset);
//		draw::text(x + (res::width(res::SPELLINF, f) - draw::textw(temp)) / 2,
//			y + res::height(res::SPELLINF, f) + 2,
//			temp);
//		x += res::width(res::SPELLINF, f);
	}
}

void squadi::show(const heroi* hero, bool info_mode, bool allow_dismiss, bool allow_upgrade) {
	draw::screenshoot surface;
	auto back = isevil(VIEWARME, VIEWARMY);
	int w1 = getwidth(back, 0);
	int h1 = getheight(back, 0);
	int x = (width - w1) / 2 - 16;
	int y = (height - h1) / 2;
	animation mon;
	if(is(Fly))
		mon.set(unit, FlyAction, 1);
	else
		mon.set(unit, (d100()<30) ? Warn : Wait);
	while(ismodal()) {
		surface.restore();
		image(x, y, back, 7);
		image(x, y, back, 0);
		int x1 = x + 24;
		int y1 = y;
		status(x1 + 386, y1 + 40, this, hero);
		effects(x1 + 140, y1 + 188, this);
		// name
		const char* p = getstr(unit);
		draw::text(x1 + 140 - draw::textw(p) / 2, y1 + 40, p);
		// avatar
		mon.paint(x1 + 146 - (is(Wide) ? cell_wd / 2 : 0), y1 + 170);
		if(count) {
			char temp[32]; zprint(temp, "%1i", count);
			text(x1 + 140 - textw(temp) / 2, y1 + 227, temp);
		}
		if(allow_upgrade && canupgrade()) {
			if(button(x + 435, y + 192, back, {5, 5, 6}))
				execute(upgrade_unit, (int)this);
		}
		if(allow_dismiss) {
			if(button(x + 310, y + 221, back, {1, 1, 2}))
				execute(dismiss_unit, (int)this);
		}
		if(!info_mode)
			button(x + 435, y + 221, back, buttoncancel, {3, 3, 4});
		else
			setcursor(NoRes, 0);
		domodal();
		if(hot::key == InputTimer) {
			if(mon.update()) {
				if(is(Fly))
					mon.set(unit, FlyAction, 1);
				else
					mon.set(unit, (d100()<30) ? Warn : Wait);
			}
		}
		if(info_mode) {
			if(hot::key == MouseLeft || hot::key == MouseRight)
				breakmodal(0);
		}
	}
}

bool playeri::recruit(monster_s unit, int& count, int maximum) {
	char temp[260];
	draw::screenshoot surface;
	auto back = RECRBKG;
	int w1 = getwidth(back, 0);
	int h1 = getheight(back, 0);
	int x = (width - w1) / 2 - 8;
	int y = (height - h1) / 2 - 16;
	auto& monster_cost = bsmeta<monsteri>::elements[unit].cost;
	auto maximum_available = resources.mindiv(monster_cost);
	if(maximum_available > maximum)
		maximum_available = maximum;
	while(ismodal()) {
		surface.restore();
		draw::image(x, y, back, 1);
		draw::image(x, y, back, 0);
		int x1 = x + 16;
		int y1 = y;
		// name
		text(x1 + 23, y1 + 30, 273, AlignCenter, getstr(unit), -1);
		if(true) {
			auto pf = font;
			font = SMALFONT;
			// available
			text(x1 + 32, y1 + 148, 90, AlignCenter, zprint(temp, "%1: %2i", "Достпуно", maximum));
			// recruit
			text(x1 + 32, y1 + 164, 90, AlignLeft, "Нанять кол-во:");
			font = pf;
		}
		// count
		edit(x1 + 136, y1 + 163, 64, count, maximum_available, 0,
			{x1 + 205, y1 + 154, RECRUIT, 0, 0, 1},
			{x1 + 205, y1 + 169, RECRUIT, 2, 2, 3},
			{x1 + 230, y1 + 155, RECRUIT, 4, 4, 5});
		picture mon; mon.set(unit, 0);
		image(x1 + 80 - getwidth(mon.res, 0) / 2,
			y1 + 100 - getheight(mon.res, 0) / 2,
			mon.res, mon.frame, AFNoOffset);
		if(draw::mousein({x1 + 80 - 40, y1 + 100 - 50, x1 + 80 + 40, y1 + 100 + 50})) {
			if(hot::key == MouseRight && hot::pressed)
				execute(unit_info_monster, unit);
		}
		// Price
		image(x1 + 142, y1 + 78, 120, monster_cost);
		image(x1 + 33, y1 + 200, 250, monster_cost * count);
		//
		button(x1 + 34, y1 + 249, RECRUIT, buttonok, {8, 8, 9}, KeyEnter);
		button(x1 + 187, y1 + 249, RECRUIT, buttoncancel, {6, 6, 7}, KeyEscape);
		domodal();
	}
	return getresult() != 0;
}

void armyi::paintsmall(const rect& rc, bool show_count, bool show_text) const {
	const auto maximum = sizeof(units) / sizeof(units[0]);
	state		push;
	picture		sprites[maximum];
	int			values[maximum];
	unsigned	count = 0;
	font = SMALFONT;
	for(auto& e : units) {
		if(!e)
			continue;
		sprites[count].clear();
		sprites[count].res = MONS32;
		sprites[count].frame = e.unit;
		sprites[count].setsize();
		if(show_count) {
			sprites[count].size.y += texth();
			if(show_text) {
				auto p = armysizei::find(e.count)->name;
				auto s = textw(p) + 4;
				if(sprites[count].size.x < s)
					sprites[count].size.x = s;
			}
		}
		values[count] = e.count;
		count++;
	}
	auto width_total = rc.width();
	auto height_total = rc.height();
	auto index = 0;
	auto height = sprites->gettotalheight(count, width_total);
	auto y = rc.y1 + (rc.height() - height)/2;
	//rectb(rc, 214);
	//rectb({rc.x1, y, rc.x2, y + height}, 214);
	while(count>0) {
		int width = 0, height = 0;
		auto count_per_line = sprites->getsize(count, width, height, width_total);
		auto x1 = rc.x1 + (width_total - width) / 2;
		for(unsigned i = 0; i < count_per_line; i++) {
			auto& e = sprites[index];
			auto y1 = y + height - e.size.y;
			image(x1 + (e.size.x - getwidth(e.res, e.frame)) / 2, y1, e.res, e.frame, AFNoOffset);
			if(show_count) {
				char temp[32];
				const char* p = temp;
				if(show_text)
					p = armysizei::find(values[index])->name;
				else
					zprint(temp, "%1i", values[index]);
				text(x1 + (e.size.x - textw(p)) / 2, y1 + e.size.y - texth(), p);
			}
			x1 += sprites[index].size.x;
			index++;
		}
		y += sprites[index].size.y;
		count -= count_per_line;
	}
}