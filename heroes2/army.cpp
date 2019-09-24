#include "main.h"

int	squadi::get(ability_s v) const {
	auto& e = getmonster();
	switch(v) {
	case Attack: return e.attack;
	case Defence: return e.defence;
	case DamageMax: return e.damageMax;
	case DamageMin: return e.damageMin;
	case HitPoints: return e.hp;
	case Shoots: return e.shoots;
	case Speed: return e.speed;
	case MoraleStat: return 3;
	case LuckStat: return 3;
	default: return 0;
	}
}

int	squadi::get(ability_s v, const heroi* hero) const {
	auto n = get(v);
	if(hero)
		n += hero->get(v);
	switch(v) {
	case MoraleStat:
		if(is(Undead))
			return NormalMorale;
		if(n < 0)
			n = 0;
		if(n > BloodMorale)
			n = BloodMorale;
		break;
	case LuckStat:
		if(n < 0)
			n = 0;
		if(n > IrishLuck)
			n = IrishLuck;
		break;
	}
	return n;
}

bool squadi::is(tag_s v) const {
	return bsmeta<monsteri>::elements[unit].tags.is(v);
}

void squadi::clear() {
	memset(this, 0, sizeof(*this));
}

monster_s squadi::getupgrade() const {
	return bsmeta<monsteri>::elements[unit].upgrade;
}

bool armyi::is(tag_s v) const {
	for(auto& e : units) {
		if(!e)
			continue;
		if(e.is(v))
			return true;
	}
	return false;
}

const squadi* armyi::find(monster_s v) const {
	for(auto& e : units) {
		if(e.unit == v && e.count > 0)
			return &e;
	}
	return 0;
}

squadi* armyi::add(monster_s v, short unsigned count) {
	auto p = (squadi*)find(v);
	if(!p) {
		for(auto& e : units) {
			if(!e.count) {
				p = &e;
				p->unit = v;
				break;
			}
		}
	}
	if(!p)
		return p;
	p->unit = v;
	p->count += count;
	return p;
}

squadi*	armyi::getslowest() const {
	auto speed = UltraFastSpeed;
	const squadi* ps = 0;
	for(auto& s : units) {
		if(!s)
			continue;
		if(bsmeta<monsteri>::elements[s.unit].speed <= speed)
			ps = &s;
	}
	return const_cast<squadi*>(ps);
}

unsigned armyi::getstrenght() const {
	unsigned r = 0;
	for(auto& s : units) {
		if(!s)
			continue;
		r += s.getstrenght();
	}
	return r;
}

squadi* armyi::getsquad(monster_s v) const {
	auto p = find(v);
	if(p)
		return const_cast<squadi*>(p);
	for(auto& s : units) {
		if(!s)
			return const_cast<squadi*>(&s);
	}
	return 0;
}

void armyi::clear() {
	memset(this, 0, sizeof(*this));
}

bool squadi::isdrainlife() const {
	switch(unit) {
	case Vampire:
	case VampireLord:
		return true;
	default:
		return false;
	}
}

bool squadi::isdragon() const {
	switch(unit) {
	case GreenDragon:
	case RedDragon:
	case BlackDragon:
	case BoneDragon:
	case Phoenix:
		return true;
	default:
		return false;
	}
}

//unsigned squadi::getstrenght() const {
//	unsigned res = ((getmonster().damageMin + getmonster().damageMax) >> 1)*count;
//	// increase strength
//	if(is(Fly))
//		res += res / 2;
//	if(getmonster().isarcher())
//		res += res / 2;
//	if(is(Twice))
//		res += res / 2;
//	if(is(Stealth))
//		res += res / 2;
//	// slowly: decrease strength
//	if((!is(Fly) && !getmonster().isarcher()) && getmonster().speed<AverageSpeed)
//		res -= res * 0.5;
//	switch(unit) {
//	case Ghost: res *= 2; break;
//	default: break;
//	}
//	return res;
//}

unsigned squadi::getstrenght() const {
	return bsmeta<monsteri>::elements[unit].rating * count;
}