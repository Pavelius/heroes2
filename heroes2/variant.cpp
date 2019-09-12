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

heroi* variant::gethero() const {
	if(hero == RandomHero)
		return 0;
	return bsmeta<heroi>::elements + hero;
}