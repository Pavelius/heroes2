#include "main.h"

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

void heroi::gainmine(const char* text, resource_s mine) {
	static const char* mine_names_of[] = {"Золотой", "Деревянной", "Ртутной", "Рудной", "Серной", "Кристальной", "Изумрудной"};
	static const char* res_names_of[] = {"1000 золотых", "два дерева", "одну ртуть", "две руды", "одну серу", "один кристалл", "один самоцвет"};
	auto count = playeri::getmineincome(mine);
	string str;
	str.add(text, mine_names_of[mine], maptbl(res_names_of, mine));
	str.addsep();
	str.add("\n");
	str.addi(mine, count, 1);
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
	switch(v.type) {
	case Resource:
		cost.clear();
		cost.add(v.resource, v.count);
		add(cost);
		break;
	case Artifact:
		add(v.artifact);
		break;
	case Monster:
		armyi::add(v.monster, v.count);
		break;
	case Ability:
		switch(v.ability) {
		case Attack:
		case Defence:
		case Wisdow:
		case Knowledge:
			abilities[v.ability]++;
			break;
		case Experience:
			addexperience(v.count);
			break;
		}
		break;
	case Spell:
		set(v.spell);
		break;
	case Skill:
		set(v.skill, v.count);
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
	case WitchHut:
		return is(object.getskill());
	}
	if(bsmeta<objecti>::elements[object.gettype()].isvisitable()) {
		auto i = map::getvisit(object.index);
		if(i == Blocked)
			return true;
		return (visited[i / 8] & (1 << (i & 7))) != 0;
	}
	return false;
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

bool heroi::interact(moveablei& object, interact_s type, variantcol v1, variantcol v2, const char* text, const char* fail) {
	string str;
	auto player = getplayer();
	switch(type) {
	case TreasureCase:
		str.add(text);
		if(true) {
			variantcol variants[2] = {v1, v2};
			add(variants[ask(str, variants) ? 0 : 1]);
		}
		break;
	case QuestArtifact:
		if(bsmeta<artifacti>::elements[object.getartifact()].text)
			text = bsmeta<artifacti>::elements[object.getartifact()].text;
		str.add(text, getstr(object.getartifact()));
		str.addsep();
		str.addi(object.getartifact());
		message(str);
		add(object.getartifact());
		break;
	case TreasureArtifact:
		str.add(text, getstr(object.getartifact()));
		str.addsep();
		str.addi(object.getartifact());
		message(str);
		add(object.getartifact());
		break;
	case TreasureCost:
		str.add(text);
		str.addsep();
		str.addi(v1);
		str.addi(v2);
		message(str);
		add(v1);
		add(v2);
		break;
	case JoinDwelling:
		if(!object.getcount()) {
			str.add(fail, bsmeta<monsteri>::elements[v1.monster].multiname);
			message(str);
		} else {
			str.add(text, bsmeta<monsteri>::elements[v1.monster].multiname);
			if(ask(str)) {
				variantcol e(v1);
				e.count = object.getcount();
				object.setcount(0);
				add(e);
			}
		}
		break;
	case CaptureObject:
		if(v1.type == Object && v1.object == ResourceObject)
			gainmine(text, object.getresource());
		else if(v1.type == Resource)
			gainmine(text, v1.resource);
		else {
			str.add(text);
			message(str);
		}
		object.setowner(getplayer()->getid());
		break;
	case LookAround:
		message(text);
		break;
	case IncreaseAbility:
		if(v1.type == Ability && v1.ability == Experience)
			v1.count = 1000;
		if(isvisited(object)) {
			str.add(fail);
			message(str);
			return false;
		} else {
			str.add(text);
			str.addsep();
			str.addi(v1);
			message(str);
			add(v1);
			setvisit(object.index);
		}
		break;
	case LearnAbility:
		switch(v1.type) {
		case Spell:
			v1.spell = object.getspell();
			break;
		case Skill:
			v1.skill = object.getskill();
			v1.count = 1;
			break;
		}
		object.set(player->getid());
		if(v1.type==Spell && !is(MagicBook)) {
			str.add(text, v1.getname());
			str.adds("К сожелению у вас нету Волшебной книги, чтобы записать заклинание в нее.");
			message(str);
			return false;
		} else if(isvisited(object)) {
			str.add(fail ? fail : text, v1.getname());
			if(v1.type == Spell)
				str.adds("Однако, вы уже и так знаете это заклинание, поэтому ничего нового вы не узнали.");
			message(str);
			return false;
		} else {
			str.add(text, v1.getname());
			str.add("\n\n");
			str.addi(v1);
			message(str);
			add(v1);
		}
		break;
	case BuyArtifact:
		if(player) {
			auto a = object.getartifact();
			costi cost;
			cost.clear();
			cost.add(v1.resource, v1.count);
			cost.add(v2.resource, v2.count);
			string rstr; rstr.addt(cost);
			str.add(text, getstr(a), rstr.begin());
			str.addsep();
			str.addi(a);
			str.addi(v1);
			str.addi(v2);
			if(player->getresources() >= cost) {
				if(ask(str)) {
					player->getresources() -= cost;
					add(a);
					return true;
				}
			}
		}
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
		if(e.type != NoCase) {
			auto text = bsmeta<interacti>::elements[e.type].text;
			auto fail = bsmeta<interacti>::elements[e.type].fail;
			if(po->text)
				text = po->text;
			if(po->fail)
				text = po->fail;
			if(e.text)
				text = e.text;
			return interact(object, e.type, e.variants[0], e.variants[1], text, fail);
		}
	}
	auto text = bsmeta<interacti>::elements[object.getinteract()].text;
	auto fail = bsmeta<interacti>::elements[object.getinteract()].fail;
	if(po->text)
		text = po->text;
	if(po->fail)
		fail = po->fail;
	return interact(object, object.getinteract(), po->param, {}, text, fail);
}

bool heroi::battle(moveablei& enemy) {
	string str;
	auto unit_total = enemy.getcount();
	auto unit_type = enemy.getmonster();
	auto& opponent = bsmeta<heroi>::elements[RandomHero];
	opponent.clear();
	opponent.add(enemy.getmonster(), unit_total);
	auto pf = getsquad(unit_type);
	auto s1 = getstrenght();
	auto s2 = opponent.getstrenght();
	auto ra = enemy.getreaction();
	auto player = getplayer();
	if(!s2 || !s1)
		return false;
	auto kf = s1 / s2;
	if(!is(HideousMask)) {
		if(ra >= ReactionIndifferent) {
			if(ra == ReactionFriendly || kf >= 2) {
				str.add("%1 впечатены вашей армией и хотят присоединиться к вашему войску. Согласны ли вы их принять?",
					bsmeta<monsteri>::elements[unit_type].multiname);
				if(ask(str)) {
					pf->unit = unit_type;
					pf->count += unit_total;
					enemy.clear();
					return true;
				}
			}
		}
		if(get(Diplomacy) && unit_total > 1) {
			auto gold_cost = bsmeta<monsteri>::elements[enemy.getmonster()].cost.get(Gold) * unit_total;
			auto unit_join = 0;
			switch(get(Diplomacy)) {
			case 1: unit_join = unit_total * 25 / 100; break;
			case 2: unit_join = unit_total * 50 / 100; break;
			case 3: unit_join = unit_total * 75 / 100; break;
			}
			if(unit_join < 1)
				unit_join = 1;
			if(player->getresources().get(Gold) >= gold_cost) {
				str.add("Воины покорены вашим шармом и выдвинули предложение:\n\n");
				str.adds("%1i из %2i %-3 вступят в ряды вашей армии, а оставшиеся оставят вас в покое за %4i золотых.",
					unit_join, unit_total, bsmeta<monsteri>::elements[unit_type].multiname, gold_cost);
				str.adds("Вы согласны?");
				if(ask(str)) {
					pf->unit = unit_type;
					pf->count += unit_join;
					player->getresources().data[Gold] -= gold_cost;
					enemy.clear();
				}
			}
		}
	}
	if(kf >= 5) {
		str.add("%1 испуганы силой вашей армии и пытаются бежать. Будете ли вы их приследовать?");
		if(!ask(str)) {
			enemy.clear();
			return false;
		}
	}
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