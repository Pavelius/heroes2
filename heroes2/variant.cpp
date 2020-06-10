#include "main.h"

const char* variant::getname() const {
	switch(type) {
	case Ability: return bsmeta<abilityi>::elements[value].name;
	case Artifact: return bsmeta<artifacti>::elements[value].name;
	case Castle: return bsmeta<castlei>::elements[value].getname();
	case Hero: return bsmeta<heroi>::elements[value].getname();
	case Landscape: return bsmeta<landscapei>::elements[value].name;
	//case Moveable: return getmoveable()->getname();
	case Skill: return bsmeta<skilli>::elements[value].name;
	case Spell: return bsmeta<spelli>::elements[value].name;
	case Monster: return bsmeta<monsteri>::elements[value].name;
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
	if(value == RandomHero)
		return 0;
	return bsmeta<heroi>::elements + value;
}

moveablei* variant::getmoveable() const {
	return bsmeta<moveablei>::elements + value;
}