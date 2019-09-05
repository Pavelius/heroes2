#include "main.h"

static const char* mine_names_of[] = {"Золотой", "Деревянной", "Ртутной", "Рудной", "Серной", "Кристальной", "Изумрудной"};

static unsigned char random(const aref<casei>& source) {
	auto total_weight = 0;
	for(auto& e : source)
		total_weight += e.chance;
	if(!total_weight)
		return 0;
	auto r = rand() % total_weight;
	auto cw = 0;
	for(auto& e : source) {
		cw += e.chance;
		if(cw > r)
			return &e - source.data;
	}
	return 0;
}

unsigned char gamei::getrandom(variant e) {
	if(e.type == Object) {
		auto& m = bsmeta<objecti>::elements[e.object];
		if(m.actions)
			return random(m.actions);
	}
	return 0;
}

bool heroi::interact(moveablei& object, interact_s type, const variantcol* variants, const char* text) {
	string str;
	switch(type) {
	case TreasureCase:
		str.add(text);
		auto choose = ask(str, variants) ? 0 : 1;
		add(variants[choose]);
		break;
	}
	return true;
}

monster_s getmonster(object_s type) {
	switch(type) {
	case GoblinHut: return Goblin;
	case DwarfCottage: return Dwarf;
	case ArcherHouse: return Archer;
	case SpriteHouse: return Sprite;
	case PeasantHut: return Peasant;
	case HalflingHole: return Halfling;
	default: return RandomMonster;
	}
}

bool heroi::interact(moveablei& object, object_s type, const char* text) {
	string str;
	switch(type) {
	case GoblinHut:
	case DwarfCottage:
	case ArcherHouse:
	case SpriteHouse:
	case PeasantHut:
	case HalflingHole:
		str.add(text, bsmeta<monsteri>::elements[getmonster(type)].multiname);
		if(ask(str)) {
		}
		break;
	case Mines:
		str.add(text, mine_names_of[object.value2]);
		message(str);
		break;
	default:
		return false;
	}
	return true;
}

bool heroi::interact(moveablei& object) {
	const objecti* po;
	switch(object.element.type) {
	case Resource:
		add(variantcol{object.element, object.value});
		if(getplayer()) {
			auto player = getplayer();
			string str;
			costi cost;
			cost.clear();
			cost.add(object.element.resource, object.value);
			str.add("Вы нашли ресурс\n(%-1)", getstr(object.element.resource));
			str.addsep();
			str.addi(object.element.resource, object.value);
			player->quickmessage(str);
		}
		break;
	case Artifact:
		break;
	case Object:
		po = bsmeta<objecti>::elements + object.element.object;
		if(po->actions) {
			auto& e = po->actions.data[object.value2];
			return interact(object, e.type, e.variants, po->text);
		} else
			return interact(object, object.element.object, po->text);
	}
	return true;
}

bool heroi::interact(short unsigned index, const pvar& object) {
	switch(object.type) {
	case Hero: break;
	case CastleVar: break;
	case Moveable: return interact(*object.moveable);
	}
	return true;
}

void heroi::add(const variantcol& v) {
	costi cost;
	auto player = getplayer();
	switch(v.element.type) {
	case Resource:
		cost.clear();
		cost.add(v.element.resource, v.count);
		if(player)
			player->getresources() += cost;
		break;
	case Artifact:
		add(v.element.artifact);
		break;
	case Ability:
		switch(v.element.ability) {
		case Attack:
		case Defence:
		case Wisdow:
		case Knowledge:
			abilities[v.element.ability]++;
			break;
		case Experience:
			addexperience(v.count);
			break;
		}
		break;
	}
}