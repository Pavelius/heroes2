#include "main.h"

struct actioncase {
	int			chance;
	variantcol	variants[2];
};
static actioncase treasure_chest[] = {{20, {{Gold, 1000}, {Experience, 500}}},
{30, {{Gold, 1500}, {Experience, 1000}}},
{20, {{Gold, 2000}, {Experience, 1500}}},
{17, {{ThunderMace}}},
{5, {{TaxLien}}},
{4, {{HideousMask}}},
{4, {{FizbinMesfortune}}},
};
static actioncase artifact_take[] = {{1, {{Paladin, 1}}},
{1, {{Crusader, 1}}},
{1, {{Genie, 1}}},
{1, {{GreenDragon, 1}}},
{1, {{RedDragon, 1}}},
{1, {{Cyclop, 1}}},
{1, {{Titan, 1}}},
{1, {{Giant, 1}}},
{5, {{Wisdow}}},
{5, {{Leadership}}},
{20, {{ArtifactObject}}},
{2, {{Gold, 3000}}},
{2, {{Gold, 1500}, {Gems, 3}}},
};

static unsigned char random(const aref<actioncase>& source) {
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
	if(e.type==Object) {
		switch(e.object) {
		case TreasureChest: return random(treasure_chest);
		case ArtifactObject: return random(artifact_take);
		default: return 0;
		}
	}
	return 0;
}

static monster_s getdwelve(object_s e) {
	switch(e) {
	case GoblinHut: return Goblin;
	case ArcherHouse: return Archer;
	case DwarfCottage: return Dwarf;
	default: return RandomMonster;
	}
}

bool heroi::interact(moveablei& object) {
	string str;
	auto player = getplayer();
	int choose;
	const actioncase* pa;
	switch(object.element.type) {
	case Resource:
		add(variantcol{object.element, object.value});
		if(player) {
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
		switch(object.element.object) {
		case TreasureChest:
			pa = treasure_chest + object.value2;
			switch(pa->variants[0].element.type) {
			case Resource:
				str.add("Исследуя окресности вы наткнулись на древний ларец. Золото можно оставить сее или раздать крестьянам в омен на опыт. Вы останите золото себе?");
				choose = ask(str, pa->variants);
				choose = choose ? 0 : 1;
				add(pa->variants[choose]);
				break;
			}
			break;
		case GoblinHut:
			str.add("Группа %1 в поисках славы желает примкнуть к вашему войску. Согласны ли вы их принять?",
				bsmeta<monsteri>::elements[getdwelve(object.element.object)].multiname);
			if(ask(str, 0)) {

			}
			break;
		case Mines:
			str.add("Вы стали хозяином %1 шахты.");
			message(str);
			break;
		}
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