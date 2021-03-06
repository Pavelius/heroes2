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

enchantmenti* bsmeta<enchantmenti>::add() {
	for(unsigned i = 0; i < bsmeta<enchantmenti>::count; i++) {
		auto& e = bsmeta<enchantmenti>::elements[i];
		if(!e)
			return &e;
	}
	if(bsmeta<enchantmenti>::count < bsmeta<enchantmenti>::count_maximum)
		return &bsmeta<enchantmenti>::elements[bsmeta<enchantmenti>::count++];
	return &bsmeta<enchantmenti>::elements[0];
}

void uniti::exhausespells() {
	for(unsigned i = 0; i < bsmeta<enchantmenti>::count; i++) {
		auto& e = bsmeta<enchantmenti>::elements[i];
		if(!e)
			continue;
		if(e.count > 0)
			e.count--;
		if(!e.count)
			e.object = 0;
	}
}

void uniti::setspell(spell_s v, int count) {
	auto p = find_enchantment(this, v);
	if(!p && count > 0)
		p = bsmeta<enchantmenti>::add();
	if(p) {
		if(count <= 0)
			memset(p, 0, sizeof(enchantmenti));
		else {
			p->id = v;
			p->object = this;
			p->count = count;
		}
	}
}

int uniti::getspell(spell_s v) const {
	auto p = find_enchantment(this, v);
	return p ? p->count : 0;
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
	remove(HalfStrenght);
	// Normalize morale
	if(morale < 0)
		morale++;
	else if(morale > 0)
		morale--;
	// Normalize luck
	if(luck < 0)
		luck++;
	else if(luck > 0)
		luck--;
}

int	uniti::get(ability_s v) const {
	int i;
	switch(v) {
	case Shoots: return shoots;
	case HitPoints: return hits;
	case MoraleStat:
		if(is(Undead))
			return NormalMorale;
		return morale + squadi::get(v, leader);
	case LuckStat:
		return luck + squadi::get(v, leader);
	case Attack:
		i = squadi::get(v, leader);
		if(is(BloodLust))
			i += getpower(BloodLust);
		return i;
	case Defence:
		i = squadi::get(v, leader);
		if(is(StoneSkin))
			i += getpower(StoneSkin);
		if(is(SteelSkin))
			i += getpower(SteelSkin);
		return i;
	case Speed:
		i = squadi::get(v, leader);
		if(is(Haste))
			i += getpower(Haste);
		return i;
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

bool uniti::ishandfight() const {
	auto i = getpos();
	for(auto d : all_around) {
		if(!isvalidindex(to(i, d), leader))
			return true;
	}
	return false;
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

unsigned uniti::getdamage(const uniti& enemy) {
	if(!isalive() || !enemy.isalive())
		return 0;
	auto d = getdamage();
	auto at = get(Attack);
	if(is(DragonSlayer) && enemy.is(Dragon))
		at += 6;
	auto df = enemy.get(Defence);
	if(is(Dragon) && enemy.is(DragonSlayer))
		df += 6;
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
	if(isarcher()) {
		if(!canshoot()) {
			if(!is(MeleeArcher))
				d /= 2;
		} else {
			if(leader)
				d += d * leader->get(Archery) * 10 / 100;
			if(enemy.is(Shield))
				d /= 2;
		}
	}
	if(enemy.is(Stone))
		d /= 2;
	if(unit == Crusader && enemy.is(Undead))
		d *= 2;
	if(d <= 0)
		d = 1;
	return d;
}

unsigned uniti::shoot(uniti& enemy) {
	auto d = getdamage(enemy);
	show_shoot(enemy);
	enemy.damage(d);
	enemy.show_damage();
	if(is(Twice)) {
		auto d = getdamage(enemy);
		show_shoot(enemy);
		enemy.damage(d);
		enemy.show_damage();
	}
	shoots--;
	return d;
}

void uniti::move(short unsigned index) {
	if(is(Fly))
		show_fly(index);
	else
		show_move(index);
	setpos(index);
}

void uniti::attack(uniti& enemy, direction_s dir) {
	auto d = getdamage(enemy);
	if(is(HalfStrenght)) {
		remove(HalfStrenght);
		d = d / 2;
	}
	show_attack(enemy, dir);
	enemy.damage(d);
	enemy.show_damage();
}

void uniti::melee(uniti& enemy, direction_s dir) {
	if(dir == Up)
		dir = getdirection(getpos(), enemy.getpos());
	attack(enemy, dir);
	if(!is(Stealth) && !is(CounterAttacked) && enemy.isalive()) {
		enemy.attack(*this, to(dir, Down));
		add(CounterAttacked);
	}
	if(is(Twice) && isalive() && !isarcher())
		attack(enemy, dir);
	if(isalive() && enemy.isalive() && !is(MoraleBoosted) && testmorale() > 0) {
		add(MoraleBoosted);
		show_morale(true);
		melee(enemy, dir);
	}
}

void uniti::heal(unsigned v) {
	auto mhp = getmonster().hp;
	hits += v;
	if(hits > mhp)
		hits = mhp;
}

void uniti::damage(unsigned v) {
	if(!count)
		return;
	sethits(gethits() - v);
	if(is(Blind)) {
		add(HalfStrenght);
		setspell(Blind, 0);
	}
	if(is(Paralyze)) {
		add(CounterAttacked);
		setspell(Paralyze, 0);
	}
	if(!count) {
		// Unit is killed
		addmorale(leader, -1);
	}
}

int uniti::gethits() const {
	if(count < 1)
		return 0;
	auto mhp = getmonster().hp;
	if(!mhp)
		return 0;
	return (count - 1) * mhp + hits;
}

int	uniti::getkilled(int d) const {
	return count - getcount(gethits() - d);
}

int	uniti::getcount(int hits) const {
	if(hits <= 0)
		return 0;
	auto mhp = getmonster().hp;
	if(!mhp)
		return 0;
	return 1 + (hits / mhp);
}

void uniti::sethits(int value) {
	auto mhp = getmonster().hp;
	if(!mhp)
		return;
	if(value <= 0) {
		count = 0;
		hits = 0;
	} else {
		count = (value + mhp - 1) / mhp;
		hits = value % mhp;
		if(hits == 0)
			hits = mhp;
	}
}

bool uniti::isdamaged() const {
	return hits < getmonster().hp;
}

unsigned uniti::getscore(const uniti& defender) const {
	auto& attacker = *this;
	// initial value: (hitpoints)
	auto damage = (attacker.getmonster().damageMin + attacker.getmonster().damageMax) / 2;
	auto kills = defender.getkilled(attacker.is(Twice) ? damage * 2 : damage);
	double res = kills * defender.getmonster().hp;
	// attacker
	auto noscale = false;
	switch(attacker.unit) {
	case Ghost:
		// priority: from killed only
		noscale = true;
		break;
	case VampireLord:
		if(attacker.isdamaged()) {
			// alive priority
			if(defender.is(Elemental) || defender.is(Undead))
				res /= 2;
		}
		break;
	default: break;
	}
	// scale on ability
	if(!noscale) {
		if(defender.isarcher())
			res += res * 0.7;
		if(defender.is(Fly))
			res += res * 0.6;
		if(defender.is(Stealth))
			res += res * 0.5;
		if(defender.is(Twice))
			res += res * 0.4;
		if(defender.isdrainlife())
			res += res * 0.3;
		if(defender.is(Area))
			res += res * 0.3;
		if(defender.is(AlwaysResponse))
			res -= res * 0.5;
	}
	// extra
	if(defender.is(MirrorImage))
		res += res * 0.7;
	if(!attacker.isarcher()) {
		if(defender.is(CounterAttacked))
			res += res * 0.3;
	}
	return (res > 1) ? (unsigned)res : 1;
}

int	uniti::getresist(spell_s id, int spell_power) const {
	//if(spell.isMindInfluence() &&
	//	(isUndead() || isElemental() ||
	//		GetID() == Monster::GIANT || GetID() == Monster::TITAN)) return 100;
	auto& spell = bsmeta<spelli>::elements[id].tags;
	if(spell.is(Alive) && is(Undead))
		return 100;
	if(spell.is(Undead) && !is(Undead))
		return 100;
	switch(unit) {
	case ArchMage:
		if(spell.is(Hostile))
			return 20;
		break;
	case Dwarf:
	case BattleDwarf:
		if(spell.is(Hostile))
			return 25;
		break;
	case GreenDragon:
	case RedDragon:
	case BlackDragon:
		return 100;
	case Phoenix:
		switch(id) {
		case ColdRay:
		case ColdRing:
		case FireBall:
		case FireBlast:
		case LightingBolt:
		case ChainLighting:
		case ElementalStorm:
			return 100;
		}
		break;
	case Crusader:
		switch(id) {
		case Curse:
		case MassCurse:
			return 100;
		}
		break;
	case EarthElement:
		switch(id) {
		case MeteorShower:
		case LightingBolt:
		case ChainLighting:
			return 100;
		default: break;
		}
		break;
	case AirElement:
		switch(id) {
		case MeteorShower:
			return 100;
		}
		break;
	case FireElement:
		switch(id) {
		case FireBall:
		case FireBlast:
			return 100;
		}
		break;
	case WaterElement:
		switch(id) {
		case ColdRay:
		case ColdRing:
			return 100;
		}
		break;
	}
	switch(id) {
	case Cure:
	case MassCure:
		if(!isdamaged())
			return 100;
		break;
	case Ressurect:
	case RessurectTrue:
	case AnimateDead:
		if(source && count >= source->count)
			return 100;
		break;
	case Hypnotize:
		if((getpower(id) * spell_power) < gethits())
			return 100;
		break;
	default: break;
	}
	return 0;
}

spell_s uniti::getbattlemagic(int chance) const {
	switch(unit) {
	case Unicorn:
		if(chance < 20)
			return Blind;
		break;
	case Cyclop:
		if(chance < 20)
			return Paralyze;
		break;
	case Mummy:
		if(chance << 20)
			return Curse;
		break;
	case RoyalMummy:
		if(chance < 30)
			return Curse;
		break;
	case Medusa:
		if(chance < 20)
			return Stone;
		break;
	}
	return ViewMines;
}

void uniti::dispell(bool only_hostile) {
	for(auto& e : bsmeta<enchantmenti>()) {
		if(!e)
			continue;
		if(only_hostile && !bsmeta<spelli>::elements[e.id].tags.is(Hostile))
			continue;
		if(e.object == this)
			e.count = 0;
	}
}

int	uniti::teststat(ability_s a) const {
	auto m = get(a) - 3;
	if(m == 0)
		return 0;
	if(m > 6)
		m = 6;
	else if(m < -6)
		m = -6;
	auto r = rand() % 10;
	if(m > 0)
		return (r < m) ? 1 : 0;
	else
		return (r < -m) ? -1 : 0;
}

bool uniti::ishuman() const {
	if(!leader)
		return false;
	auto player = leader->getplayer();
	if(!player)
		return false;
	return true;
}