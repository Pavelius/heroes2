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
	auto& s = *((squadi*)hot::param);
	if(current_unit) {
		if(s.unit == current_unit->unit) {
			s.count += current_unit->count;
			current_unit->clear();
		} else {
			auto s2 = *current_unit;
			*current_unit = s;
			s = s2;
		}
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

static void field(string& str, const char* name, const char* v1) {
	str.addn("%1: %2", name, v1);
}

static void field(string& str, const char* name, int v1) {
	str.addn("%1: %2i", name, v1);
}

static void field(string& str, const char* name, int v1, int v2, const char* format = "%1: %3i (%2i)") {
	if(v1 == v2)
		field(str, name, v1);
	else
		str.addn(format, name, v1, v2);
}

static void field(string& str, const char* name, ability_s e, const squadi* ps, const heroi* ph, const uniti* pu, const char* format = "%1: %3i (%2i)") {
	auto v1 = ps->get(e);
	if(v1<=0)
		return;
	auto v2 = v1;
	if(pu)
		v2 = pu->get(e);
	else if(ph)
		v2 = ps->get(e, ph);
	field(str, name, v1, v2, format);
}

static int getstat(ability_s a, int min, int max, const uniti* pu, const heroi* ph, const squadi* sq) {
	int m;
	if(pu)
		m = pu->get(a);
	else
		m = sq->get(a, ph);
	if(m < min)
		m = min;
	else if(m > max)
		m = max;
	return m;
}

static int status(int x, int y, int width, const squadi* squad, const heroi* hero, const uniti* pu) {
	string str;
	field(str, getstr(Attack), Attack, squad, hero, pu);
	field(str, getstr(Defence), Defence, squad, hero, pu);
	field(str, "Урон", squad->get(DamageMin), squad->get(DamageMax), "%1: %2i-%3i");
	field(str, "Выстрелов", Shoots, squad, hero, pu, "%1: %3i из %2i");
	field(str, "Жизнь", HitPoints, squad, hero, pu, "%1: %3i из %2i");
	field(str, "Скорость", getstr((speed_s)getstat(Speed, 0, UltraFastSpeed, pu, hero, squad)));
	if(hero) {
		field(str, "Мораль", getstr((morale_s)getstat(MoraleStat, 0, 7, pu, hero, squad)));
		field(str, "Удача", getstr((luck_s)getstat(LuckStat, 0, 7, pu, hero, squad)));
	}
	return textf(x, y, width, str);
}

static int geteffectframe(spell_s v) {
	switch(v) {
	case Haste: return 0;
	case Slow: case MassSlow: return 1;
	case Blind: return 2;
	case Bless: case MassBless: return 3;
	case Curse: case MassCurse: return 4;
	case Berserker: return 5;
	case Paralyze: return 6;
	case Hypnotize: return 7;
	case DragonSlayer: return 8;
	case BloodLust: return 9;
	case Shield: return 10;
	case MirrorImage: return 11;
	case Antimagic: return 12;
	case StoneSkin: return 13;
	case SteelSkin: return 14;
	default: return -1;
	}
}

static void effects(int x, int y, const uniti* p) {
	if(!p)
		return;
	char temp[32];
	draw::state push;
	draw::font = SMALFONT;
	int dx = 0;
	for(auto& e : bsmeta<enchantmenti>()) {
		if(e.object != p)
			continue;
		int f = geteffectframe(e.id);
		if(f==-1)
			continue;
		dx += getwidth(SPELLINF, f);
	}
	x = x - dx / 2;
	for(auto& e : bsmeta<enchantmenti>()) {
		if(e.object != p)
			continue;
		int f = geteffectframe(e.id);
		if(f == -1)
			continue;
		zprint(temp, "%1i", e.count);
		image(x, y, SPELLINF, f, AFNoOffset);
		text(x + (getwidth(SPELLINF, f) - textw(temp)) / 2, y + getheight(SPELLINF, f) + 2, temp);
		x += getwidth(SPELLINF, f);
	}
}

void squadi::show(const heroi* hero, bool info_mode, bool allow_dismiss, bool allow_upgrade, const uniti* pu) {
	draw::screenshoot surface;
	auto back = isevil(VIEWARME, VIEWARMY);
	int w1 = getwidth(back, 0);
	int h1 = getheight(back, 0);
	int x = (width - w1) / 2 - 16;
	int y = (height - h1) / 2;
	unitai mon;
	mon.clear();
	mon.type = Monster;
	mon.monster = this->unit;
	mon.unit = this->unit;
	mon.uniti::count = 0;
	if(is(Fly))
		mon.set(FlyAction, 1);
	else
		mon.set(Wait);
	mon.pos.x = x + 24 + 146 - (is(Wide) ? cell_wd / 2 : 0);
	mon.pos.y = y + 170;
	while(ismodal()) {
		surface.restore();
		image(x, y, back, 7);
		image(x, y, back, 0);
		int x1 = x + 24;
		int y1 = y;
		status(x1 + 286, y1 + 40, 220, this, hero, pu);
		effects(x1 + 140, y1 + 188, pu);
		// name
		const char* p = getstr(unit);
		draw::text(x1 + 140 - draw::textw(p) / 2, y1 + 40, p);
		// avatar
		mon.paint();
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
			if(mon.increment()) {
				if(is(Fly))
					mon.set(FlyAction, 1);
				else
					mon.set(Wait);
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