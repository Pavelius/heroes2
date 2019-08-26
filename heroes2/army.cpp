#include "main.h"

int	squadi::get(ability_s v) const {
	switch(v) {
	case Attack: return bsmeta<monsteri>::elements[unit].attack;
	case Defence: return bsmeta<monsteri>::elements[unit].defence;
	case DamageMax: return bsmeta<monsteri>::elements[unit].damageMax;
	case DamageMin: return bsmeta<monsteri>::elements[unit].damageMin;
	case HitPoints: return bsmeta<monsteri>::elements[unit].hp;
	case HitPointsMax: return bsmeta<monsteri>::elements[unit].hp;
	case Speed: return bsmeta<monsteri>::elements[unit].speed;
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
	default:
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