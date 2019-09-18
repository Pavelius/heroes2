#include "main.h"

const int awd = 11, ahd = 9;
static const direction_s all_around[] = {Left, Right, LeftUp, LeftDown, RightUp, RightDown};

DECLBASE(enchantmenti, 64);

static enchantmenti* find_enchantment(const uniti* object, spell_s id) {
	for(unsigned i = 0; i < bsmeta<enchantmenti>::count; i++) {
		auto& e = bsmeta<enchantmenti>::elements[i];
		if(e.object == object && e.id == id)
			return &e;
	}
	return 0;
}

static enchantmenti* add_enchantment() {
	for(unsigned i = 0; i < bsmeta<enchantmenti>::count; i++) {
		auto& e = bsmeta<enchantmenti>::elements[i];
		if(!e)
			return &e;
	}
	if(bsmeta<enchantmenti>::count < bsmeta<enchantmenti>::count_maximum)
		return &bsmeta<enchantmenti>::elements[bsmeta<enchantmenti>::count++];
	return &bsmeta<enchantmenti>::elements[0];
}

void uniti::addspell(spell_s v, unsigned short rounds) {
	auto p = find_enchantment(this, v);
	if(!p)
		p = add_enchantment();
	p->object = this;
	p->count = rounds;
	p->id = v;
}

void uniti::setspell(spell_s v, unsigned short count) {
	auto p = find_enchantment(this, v);
	if(p) {
		p->count = count;
		if(!count)
			p->object = 0;
	}
}

unsigned short uniti::getspell(spell_s v) const {
	for(unsigned i = 0; i < bsmeta<enchantmenti>::count; i++) {
		auto& e = bsmeta<enchantmenti>::elements[i];
		if(e.object == this && e.id == v)
			return e.count;
	}
	return 0;
}

void uniti::setup(squadi& v, heroi* h) {
	unit = v.unit;
	count = v.count;
	source = &v;
	leader = h;
	shoots = getmonster().shoots;
	hits = getmonster().hp;
}

void uniti::refresh() {
	remove(Moved);
	remove(CounterAttacked);
	remove(TotalDefence);
}

int	uniti::get(ability_s v) const {
	switch(v) {
	case Shoots: return shoots;
	default: return squadi::get(v, leader); break;
	}
}

static bool isvalidindex(short unsigned i, const heroi* leader) {
	if(i == Blocked)
		return true;
	auto p = uniti::find(i);
	if(!p)
		return true;
	if(p->leader == leader)
		return true;
	return false;
}

bool uniti::canshoot() const {
	if(!shoots)
		return false;
	auto i = getpos();
	for(auto d : all_around) {
		if(!isvalidindex(to(i, d), leader))
			return false;
	}
	return true;
}

direction_s uniti::to(direction_s d, direction_s d1) {
	switch(d1) {
	case Down:
		switch(d) {
		case RightUp: return LeftDown;
		case LeftDown: return RightUp;
		case Left: return Right;
		case Right: return Left;
		case RightDown: return LeftUp;
		case LeftUp: return RightDown;
		default: return d;
		}
		break;
	default: return d;
	}
}

short unsigned uniti::to(short unsigned i, direction_s d) {
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

unsigned uniti::getdamage() const {
	unsigned min = bsmeta<monsteri>::elements[unit].damageMin;
	unsigned max = bsmeta<monsteri>::elements[unit].damageMax;
	if(is(Curse))
		max = min;
	else if(is(Bless))
		min = max;
	min *= count;
	max *= count;
	if(!max)
		return 0;
	return min + rand() % (max - min + 1);
}

unsigned uniti::attack(uniti& enemy) {
	if(!count || !enemy.count)
		return 0;
	auto d = getdamage();
	auto at = get(Attack);
	auto df = enemy.get(Defence);
	if(at > df) {
		auto m = at - df;
		if(m > 30)
			m = 30;
		d += (d * m) / 10;
	} else if(df > at) {
		auto m = df - at;
		if(m > 14)
			m = 14;
		d -= (d * m) / 20;
	}
	if(d <= 0)
		d = 1;
	return d;
}

void uniti::melee(uniti& enemy) {
	auto d = attack(enemy);
	enemy.damage(d);
	if(!is(Stealth) && !is(CounterAttacked)) {
		d = enemy.attack(*this);
		damage(d);
		enemy.add(CounterAttacked);
	}
	if(is(Twice)) {
		d = attack(enemy);
		enemy.damage(d);
	}
}

void uniti::damage(unsigned v) {
	if(count < 1)
		return;
	unsigned mhp = getmonster().hp;
	if(!mhp)
		return;
	unsigned chp = (count - 1) * mhp + hits;
	if(chp < v)
		count = 0;
	else {
		chp -= v;
		count = chp / mhp;
		hits = (chp % mhp) + 1;
	}
}