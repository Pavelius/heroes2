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

bool heroi::interact(interact_s type, const variantcol* variants, const char* text, unsigned char param) {
	string str;
	switch(type) {
	case TreasureCase:
		str.add(text);
		add(variants[ask(str, variants) ? 0 : 1]);
		break;
	case TreasureArtifact:
		str.add(text, getstr(variants[0].element.artifact));
		str.addsep();
		str.addi(variants[0].element.artifact);
		message(str);
		add(variants[0].element.artifact);
		break;
	case TreasureCost:
		str.add(text);
		str.addsep();
		str.addi(variants[0]);
		str.addi(variants[1]);
		message(str);
		add(variants[0]);
		add(variants[1]);
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

bool moveablei::is(object_use_s v) const {
	return bsmeta<objecti>::elements[type].use==v;
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
	switch(object.gettype()) {
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

void moveablei::setup(generator& generate) {
	auto& m = bsmeta<objecti>::elements[type];
	if(m.actions)
		count = random(m.actions);
	switch(type) {
	case ResourceObject:
		switch(subtype) {
		case Ore: case Wood: count = xrand(5, 10); break;
		case Gold: count = 100 * xrand(3, 9); break;
		default: count = xrand(3, 6); break;
		}
		break;
	case MonsterObject:
		switch(bsmeta<monsteri>::elements[subtype].level) {
		case 2: count = xrand(8, 16); break;
		case 3: count = xrand(4, 7); break;
		case 4: count = xrand(1, 3); break;
		default: count = xrand(17, 30); break;
		}
		break;
	case GoblinHut: case ThatchedHut: case HalflingHole: case SpriteHouse: count = 20; break;
	case ArcherHouse: case DwarfCottage: count = 10; break;
	case Shrine1: set(generate.spell(1)); break;
	case Shrine2: set(generate.spell(2)); break;
	case Shrine3: set(generate.spell(3)); break;
	case WitchHut: set(generate.skill()); break;
	}
}

bool heroi::interact(moveablei& object, object_s type, const char* text, const char* text_fail) {
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
		if(!object.getcount()) {
			str.add(text_fail, bsmeta<monsteri>::elements[monster].multiname);
			message(str);
		} else {
			str.add(text, bsmeta<monsteri>::elements[monster].multiname);
			if(ask(str)) {
				variantcol e;
				e.element = monster;
				e.count = object.getcount();
				object.setcount(0);
				add(e);
			}
		}
		break;
	case Mines:
		gainmine(text, object.getresource());
		object.setowner(getplayer()->getid());
		break;
	case SawMill:
		gainmine(text, Wood);
		object.setowner(getplayer()->getid());
		break;
	case AlchemyLab:
		gainmine(text, Mercury);
		object.setowner(getplayer()->getid());
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
			str.add("\n\n");
			str.addi(object.getspell());
		}
		message(str);
		add(variantcol{object.getspell(), 1});
		break;
	case WatchTower:
		str.add(text);
		message(str);
		break;
	case ArtifactObject:
		if(true) {
			auto a = object.getartifact();
			auto p = bsmeta<artifacti>::elements[a].text;
			if(!p)
				p = text;
			str.add(p, getstr(a));
			str.addsep();
			str.addi(a);
			message(str);
			add(object.getartifact());
		}
		break;
	default:
		return false;
	}
	return true;
}

bool heroi::interact(moveablei& object) {
	if(object.is(ResourceObject)) {
		gain(object.getresource(), object.getcount());
		return true;
	}
	const objecti* po = bsmeta<objecti>::elements + object.gettype();
	if(po->actions) {
		auto& e = po->actions.data[object.getcount()];
		if(e.type != NoCase)
			return interact(e.type, e.variants, e.text ? e.text : po->text);
	}
	return interact(object, object.gettype(), po->text, po->text_fail);
}

bool heroi::battle(moveablei& enemy) {
	enemy.clear();
	return true;
}

bool heroi::interact(short unsigned index, const pvar& object) {
	switch(object.type) {
	case Hero: break;
	case CastleVar: break;
	case Moveable:
		if(object.moveable->is(MonsterObject))
			return battle(*object.moveable);
		return interact(*object.moveable);
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