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

bool heroi::interact(interact_s type, const variantcol* variants, const char* text) {
	string str;
	artifact_s artifact;
	int choose;
	switch(type) {
	case TreasureCase:
		str.add(text);
		choose = ask(str, variants) ? 0 : 1;
		add(variants[choose]);
		break;
	case TreasureArtifact:
		artifact = variants[0].element.artifact;
		if(artifact == ThunderMace)
			artifact = generator::any_artifact(1);
		str.add(text, getstr(artifact));
		str.addsep();
		str.addi(artifact);
		message(str);
		add(artifact);
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

bool moveablei::isonetime() const {
	switch(element.type) {
	case Artifact: return true;
	case Resource: return true;
	case Object:
		switch(element.object) {
		case TreasureChest: return true;
		case CampFire: return true;
		case WaterChest: return true;
		}
		break;
	}
	return false;
}

void heroi::gainmine(const char* text, resource_s mine) {
	string str;
	str.add(text, mine_names_of[mine]);
	str.addsep();
	str.add("\n");
	str.addi(mine, playeri::getmineincome(mine), 1);
	message(str);
}

void heroi::gain(resource_s type, unsigned short count) {
	add(variantcol{type, count});
	auto player = getplayer();
	if(player) {
		string str;
		costi cost;
		cost.clear();
		cost.add(type, count);
		str.add("Вы нашли ресурс\n(%-1)", getstr(type));
		str.addsep();
		str.addi(type, count);
		player->quickmessage(str);
	}
}

void heroi::add(const costi& v) {
	auto player = getplayer();
	if(player)
		player->getresources() += v;
}

void heroi::add(const variantcol& v) {
	costi cost;
	switch(v.element.type) {
	case Resource:
		cost.clear();
		cost.add(v.element.resource, v.count);
		add(cost);
		break;
	case Artifact:
		add(v.element.artifact);
		break;
	case Monster:
		armyi::add(v.element.monster, v.count);
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
	case Spell:
		set(v.element.spell);
		break;
	}
}

bool heroi::isvisited(const moveablei& object) const {
	if(object.element.type != Object)
		return false;
	switch(object.element.object) {
	case MagicWell:
		return get(SpellPoints) >= getspmax();
	case Shrine1:
	case Shrine2:
	case Shrine3:
		return is(object.getspell());
	default:
		return false;
	}
}

bool heroi::interact(moveablei& object, object_s type, const char* text, const char* text_fail) {
	costi cost;
	string str;
	monster_s monster;
	bool allok;
	switch(type) {
	case GoblinHut:
	case DwarfCottage:
	case ArcherHouse:
	case SpriteHouse:
	case PeasantHut:
	case HalflingHole:
		monster = getmonster(type);
		if(!object.value2) {
			str.add(text_fail, bsmeta<monsteri>::elements[monster].multiname);
			message(str);
		} else {
			str.add(text, bsmeta<monsteri>::elements[monster].multiname);
			if(ask(str)) {
				variantcol e;
				e.element = monster;
				e.count = object.value2;
				add(e);
				object.value2 = 0;
			}
		}
		break;
	case Mines:
		gainmine(text, object.getresource());
		object.player = getplayer()->getid();
		break;
	case SawMill:
		gainmine(text, Wood);
		object.player = getplayer()->getid();
		break;
	case AlchemyLab:
		gainmine(text, Mercury);
		object.player = getplayer()->getid();
		break;
	case CampFire:
		cost.clear();
		cost.add(Gold, xrand(5, 10) * 100);
		cost.add(resource_s(xrand(Wood, Gems)), xrand(3, 6));
		str.add(text);
		str.addsep();
		str.addi(cost);
		message(str);
		add(cost);
		break;
	case Shrine1:
	case Shrine2:
	case Shrine3:
		allok = false;
		str.add(text, getstr(object.getspell()));
		if(!is(MagicBook))
			str.adds("К сожелению у вас нету Волшебной книги, чтобы записать заклинание в нее.");
		else if(isvisited(object))
			str.adds("Однако, это заклиание вы уже и так знаете, поэтому их помощь вам была не нужна.");
		else {
			allok = true;
			str.addsep();
			str.addi(object.getspell());
		}
		message(str);
		add(variantcol{object.getspell(), 1});
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
		gain(object.element.resource, object.value);
		break;
	case Artifact:
		break;
	case Object:
		po = bsmeta<objecti>::elements + object.element.object;
		if(po->actions) {
			auto& e = po->actions.data[object.value2];
			return interact(e.type, e.variants, e.text ? e.text : po->text);
		} else
			return interact(object, object.element.object, po->text, po->text_fail);
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

bool heroi::buymagicbook() {
	if(is(MagicBook))
		return true;
	auto player = getplayer();
	if(!player)
		return false;
	costi cost;
	cost.clear();
	cost.add(Gold, gamei::cost_magic_book);
	string str;
	str.add("Чтобы направлять заклинания, прежде следует купить волшебную книгу за %1i золотых.",
		cost.get(Gold));
	if(player->getresources() >= cost) {
		str.adds("Желаете приобрести ее?");
		str.addsep();
		str.addi(MagicBook);
		if(!ask(str))
			return false;
	} else {
		str.adds("К сожелению у вас не хватает денег.");
		message(str);
		return false;
	}
	player->getresources() -= cost;
	add(MagicBook);
	return true;
}