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

static bool isonetime(object_s id) {
	switch(id) {
	case CampFire:
	case TreasureChest:
	case ResourceObject:
	case WaterChest:
		return true;
	default:
		return false;
	}
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

bool heroi::interact(moveablei& object) {
	return true;
}

bool heroi::interact(short unsigned index, const pvar& object) {
	string str;
	const actioncase* pa;
	auto player = getplayer();
	int choose;
	switch(object.type) {
	case Hero:
		break;
	case CastleVar:
		break;
	case Moveable:
		switch(object.moveable->element.type) {
		case Resource:
			add(variantcol{object.moveable->element, object.moveable->value});
			if(player) {
				costi cost;
				cost.clear();
				cost.add(object.moveable->element.resource, object.moveable->value);
				str.add("¬ы нашли ресурс\n(%-1)", getstr(object.moveable->element.resource));
				str.addsep();
				str.addi(object.moveable->element.resource, object.moveable->value);
				player->quickmessage(str);
			}
			return true;
		case Artifact:
			return true;
		case Object:
			switch(object.moveable->element.object) {
			case TreasureChest:
				pa = treasure_chest + object.moveable->value2;
				switch(pa->variants[0].element.type) {
				case Resource:
					str.add("»сследу€ окресности вы наткнулись на древний ларец. «олото можно оставить сее или раздать кресть€нам в омен на опыт. ¬ы останите золото себе?");
					choose = ask(str, pa->variants);
					choose = choose ? 0 : 1;
					add(pa->variants[choose]);
					break;
				}
				return true;
			default: return isonetime(object.moveable->element.object);
			}
		}
		break;
	}
	return false;
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
	}
}