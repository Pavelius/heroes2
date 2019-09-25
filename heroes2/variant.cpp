#include "main.h"

const char* variant::getname() const {
	switch(type) {
	case Ability: return getstr(ability);
	case Artifact: return getstr(artifact);
	case Landscape: return getstr(landscape);
	case Skill: return getstr(skill);
	case Spell: return getstr(spell);
	case Monster: return getstr(monster);
	default: return "";
	}
}

variant::variant(const moveablei* v) : type(Moveable), value(v - bsmeta<moveablei>::elements) {}

variant::variant(const castlei* v) : type(CastleVar), value(v - bsmeta<castlei>::elements) {}

variant::variant(const heroi* v) : type(Hero), value(v ? v->getid() : RandomHero) {}

castlei* variant::getcastle() const {
	return bsmeta<castlei>::elements + value;
}

heroi* variant::gethero() const {
	if(hero == RandomHero)
		return 0;
	return bsmeta<heroi>::elements + hero;
}

moveablei* variant::getmoveable() const {
	return bsmeta<moveablei>::elements + value;
}