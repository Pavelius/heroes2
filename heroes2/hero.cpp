#include "main.h"

static struct heroin {
	const char*		name;
	kind_s			kind;
} hero_data[] = {{"Лорд Килбурн", Knight},
{"Сэр Галлант", Knight},
{"Эктор", Knight},
{"Гвиннет", Knight},
{"Тиро", Knight},
{"Амброуз", Knight},
{"Руби", Knight},
{"Максимус", Knight},
{"Димитрий", Knight},
//
{"Громотопор", Barbarian},
{"Прекраснейший", Barbarian},
{"Джоджош", Barbarian},
{"Крэг Хек", Barbarian},
{"Жезебель", Barbarian},
{"Жаклин", Barbarian},
{"Эргон", Barbarian},
{"Тсабу", Barbarian},
{"Атлас", Barbarian},
//
{"Астра", Sorcerer},
{"Наташа", Sorcerer},
{"Троян", Sorcerer},
{"Ваттавна", Sorcerer},
{"Ребека", Sorcerer},
{"Джем", Sorcerer},
{"Ариэль", Sorcerer},
{"Карлавн", Sorcerer},
{"Луна", Sorcerer},
//
{"Ари", Warlock},
{"Аламар", Warlock},
{"Веспер", Warlock},
{"Кродо", Warlock},
{"Барок", Warlock},
{"Кастор", Warlock},
{"Агар", Warlock},
{"Фалагар", Warlock},
{"Врасмонт", Warlock},
//
{"Мира", Wizard},
{"Флинт", Wizard},
{"Давн", Wizard},
{"Халон", Wizard},
{"Мирини", Wizard},
{"Вилфрей", Wizard},
{"Саракин", Wizard},
{"Калиндра", Wizard},
{"Мандигал", Wizard},
//
{"Зом", Necromancer},
{"Дарлана", Necromancer},
{"Зам", Necromancer},
{"Ранлу", Necromancer},
{"Чарити", Necromancer},
{"Риалдо", Necromancer},
{"Роксана", Necromancer},
{"Сандро", Necromancer},
{"Целиа", Necromancer},
//
{"Роланд", Wizard},
{"Корлагон", Knight},
{"Элиза", Sorcerer},
{"Арчибальд", Warlock},
{"Халтон", Knight},
{"Бакс", Necromancer},
//
{"Капитан", Barbarian},
{"Капитан", Knight},
{"Капитан", Necromancer},
{"Капитан", Sorcerer},
{"Капитан", Warlock},
{"Капитан", Wizard},
//
{"Случайные", RandomKind},
};
heroi bsmeta<heroi>::elements[RandomHero + 1];
const char*	skill_level_name[] = {"нет", "Базово", "Продвинуто", "Эксперт"};
const costi	heroi::cost = {{2500}};

static unsigned level_experience[41] = {0,
0, 1000, 2000, 3200, 4500, 6000, 7700, 9000, 11000, 13200,
15500, 18500, 22100, 26420, 31604, 37824, 45288, 54244, 64991, 77887,
93362, 111932, 134216, 160956, 193044, 231549, 277755, 333202, 399738, 479581,
575392, 690365, 828332, 993892, 1192564, 1430970, 1717057, 2060361, 2472325, 2966681
};

void heroi::initialize() {
	for(auto& e : bsmeta<heroi>::elements)
		e.clear();
}

void heroi::clear() {
	auto& d = hero_data[getid()];
	memset(this, 0, sizeof(*this));
	setname(d.name);
	player = RandomPlayer;
	kind = d.kind;
	level = 1;
	for(auto& e : artifacts)
		e = NoArtifact;
	for(unsigned i = 0; i < 4; i++)
		abilities[i] = bsmeta<kindi>::elements[kind].abilities[i];
	for(auto e : bsmeta<kindi>::elements[kind].skills)
		skills[e]++;
	for(auto e : bsmeta<kindi>::elements[kind].units) {
		auto n = bsmeta<monsteri>::elements[e].grown;
		add(e, xrand(n, n * 2));
	}
	if(bsmeta<kindi>::elements[kind].spell) {
		add(MagicBook);
		set(bsmeta<kindi>::elements[kind].spell);
	}
	player = RandomPlayer;
	portrait = getid();
	index = Blocked;
	index_move = Blocked;
	abilities[SpellPoints] = getspmax();
	abilities[MovePoints] = getmpmax();
}

playeri* heroi::getplayer() const {
	if(player == RandomPlayer)
		return 0;
	return bsmeta<playeri>::elements + player;
}

int heroi::getbonus(ability_s v) const {
	auto r = 0;
	for(auto e : artifacts) {
		if(e == NoArtifact)
			continue;
		auto i = getpower(e);
		auto t = bsmeta<artifacti>::elements[e].type;
		if(t.type == Ability) {
			if(t.ability == v)
				r += i;
			else if(t.ability == SpellPowerKnowledge && (v == SpellPower || v == Knowledge))
				r += i;
			else if(t.ability == AttackDefence && (v == Attack || v == Defence))
				r += i;
			else if(t.ability == AllAbilities && (v == Attack || v == Defence || v == SpellPower || v == Knowledge))
				r += i;
		}
	}
	return r;
}

int heroi::get(ability_s v) const {
	int r;
	switch(v) {
	case Attack:
	case Defence:
	case SpellPower:
	case Knowledge:
		return abilities[v] + getbonus(v);
	case SpellPoints:
	case MovePoints:
		return abilities[v];
	case MoraleStat:
		r = skills[Leadership];
		if(armyi::is(Undead))
			r--;
		return r;
	case LuckStat:
		return skills[Luck];
	default:
		return 0;
	}
}

int	heroi::getspmax() const {
	return get(Knowledge) * 10;
}

int	heroi::getmpmax() const {
	unsigned r = 1500;
	auto sw = getslowest();
	if(sw) {
		switch(bsmeta<monsteri>::elements[sw->unit].speed) {
		case UltraFastSpeed: r = 1500; break;
		case VeryFastSpeed: r = 1400; break;
		case FastSpeed: r = 1300; break;
		case AverageSpeed: r = 1200; break;
		case SlowSpeed: r = 1100; break;
		default: r = 1000; break;
		}
	}
	auto s = get(Logistics);
	if(s)
		r = (r * (100 + s * 10)) / 100;
	s += getbonus(MovePoints) * 100;
	return r;
}

void heroi::add(artifact_s id) {
	for(auto& e : artifacts) {
		if(e != NoArtifact)
			continue;
		e = id;
		break;
	}
}

int	heroi::getcost(spell_s v) const {
	auto r = bsmeta<spelli>::elements[v].points;
	switch(v) {
	case Bless:
	case MassBless:
		if(is(SnakeRing))
			r /= 2;
		break;
	case SummonAirElemental:
	case SummonEarthElemental:
	case SummonFireElemental:
	case SummonWaterElemental:
	case SetAirGuadrdian:
	case SetEartGuadrdian:
	case SetFireGuadrdian:
	case SetWaterGuadrdian:
		if(is(ElementalRing))
			r /= 2;
		break;
	case Curse:
	case MassCurse:
		if(is(EvilEye))
			r /= 2;
		break;
	}
	if(r < 1)
		r = 1;
	return r;
}

kind_s heroi::getkind() const {
	return kind;
}

int playeri::getheroes() const {
	auto result = 0;
	for(auto& e : bsmeta<heroi>::elements) {
		if(e.getplayer() != this)
			continue;
		result++;
	}
	return result;
}

unsigned heroi::select(heroi** result, heroi** result_maximum, const playeri* player, kind_s kind, kind_s kind_exclude, bool include_special) {
	auto p = result;
	for(auto i = FirstHero; i <= LastGameHero; i = (hero_s)(i + 1)) {
		auto& e = bsmeta<heroi>::elements[i];
		if(e.getplayer() != player)
			continue;
		if(kind != RandomKind && e.getkind() != kind)
			continue;
		if(kind_exclude != RandomKind && e.getkind() == kind_exclude)
			continue;
		if(p < result_maximum)
			*p++ = &e;
	}
	return p - result;
}

heroi* heroi::find(short unsigned index) {
	if(index == Blocked)
		return 0;
	for(auto& e : bsmeta<heroi>::elements) {
		if(e.index == index)
			return &e;
	}
	return 0;
}

unsigned heroi::getcost(short unsigned from, short unsigned to) const {
	return map::getcost(from, to, get(Pathfinding));
}

void heroi::set(ability_s id, int v) {
	if(id >= sizeof(abilities) / sizeof(abilities[0]))
		return;
	abilities[id] = v;
}

int	heroi::getsprefresh() const {
	return get(Mysticism) + 1;
}

unsigned heroi::getlearn(variantcol* result, unsigned count) const {
	auto n = 0;
	cflags<skill_s> exclude;
	for(auto i = FirstSkill; i <= LastSkill; i = skill_s(i + 1)) {
		if(skills[i] >= 3)
			exclude.add(i);
	}
	for(unsigned i = 0; i < count; i++) {
		if(getskillscount() >= 8) {
			// После того как появилось 8 навыков, мы не можем брать новые
			for(auto i = FirstSkill; i <= LastSkill; i = skill_s(i + 1)) {
				if(skills[i] == 0)
					exclude.add(i);
			}
		}
		auto v = bsmeta<kindi>::elements[kind].getrandomskill(exclude);
		if(!v)
			break;
		*((variant*)(result + n)) = v;
		result[n].count = skills[v.skill] + 1;
		exclude.add(v.skill);
		n++;
	}
	return n;
}

extern const char* skill_level_name[];

void heroi::levelup(bool interactive) {
	variantcol raise_skills[2];
	unsigned raise_count = getlearn(raise_skills, 2);
	ability_s raise = bsmeta<kindi>::elements[kind].getrandomability(level);
	auto choose = -1;
	if(interactive) {
		string str;
		str.add("%1 получает уровень опыта.", getname());
		str.add("\n\n%1 +1\n", getstr(raise));
		if(raise_count >= 1) {
			str.addn("\nВы также можете выучить %-1 %2",
				skill_level_name[raise_skills[0].count], getstr(raise_skills[0].skill));
		}
		if(raise_count >= 2) {
			str.adds("или %-1 %2",
				skill_level_name[raise_skills[1].count], getstr(raise_skills[1].skill));
		}
		str.add(".");
		choose = learn(str, raise_skills, raise_count);
	} else {
		if(raise_count)
			choose = rand() % raise_count;
	}
	abilities[raise]++;
	if(choose != -1)
		skills[raise_skills[choose].skill]++;
	level++;
}

void heroi::addexperience(unsigned count, bool interactive) {
	experience += count;
	while(level_experience[level] < experience)
		levelup(interactive);
}

int	heroi::getskillscount() const {
	auto n = 0;
	for(auto i = FirstSkill; i <= LastSkill; i = skill_s(i + 1)) {
		if(skills[i] > 0)
			n++;
	}
	return n;
}

void heroi::refresh() {
	auto v0 = get(SpellPoints);
	auto v1 = getspmax();
	if(v0 < v1)
		set(SpellPoints, imax(0, imin(v0 + getsprefresh(), v1)));
	set(MovePoints, getmpmax());
}

unsigned heroi::get(artifact_s* source, unsigned count_maximum) {
	auto pe = source + count_maximum;
	auto pb = source;
	for(auto e : artifacts) {
		if(e == NoArtifact)
			continue;
		if(pb < pe)
			*pb++ = e;
	}
	return pb - source;
}

bool heroi::is(artifact_s v) const {
	for(auto e : artifacts) {
		if(e == NoArtifact)
			continue;
		if(e == v)
			return true;
	}
	return false;
}

void heroi::setvisit(unsigned short index) {
	auto i = map::getvisit(index);
	if(i == Blocked)
		return;
	visited[i / 8] |= (1 << (i & 7));
}

unsigned heroi::getdamage(spell_s id, uniti& enemy) const {
	auto value = bsmeta<spelli>::elements[id].power * get(SpellPower);
	auto leader = enemy.leader;
	switch(enemy.unit) {
	case IronGolem:
	case SteelGolem:
		switch(id) {
			// 50% damage
		case ColdRay:
		case ColdRing:
		case FireBall:
		case FireBlast:
		case LightingBolt:
		case ChainLighting:
		case ElementalStorm:
		case Armagedon:
			value /= 2;
			break;
		}
		break;
	case WaterElement:
		switch(id) {
			// 200% damage
		case FireBall:
		case FireBlast:
			value *= 2;
			break;
		}
		break;
	case AirElement:
		switch(id) {
			// 200% damage
		case ElementalStorm:
		case LightingBolt:
		case ChainLighting:
			value *= 2;
			break;
		}
		break;
	case FireElement:
		switch(id) {
			// 200% damage
		case ColdRay:
		case ColdRing:
			value *= 2;
			break;
		}
		break;
	default: break;
	}

	// check artifact
	switch(id) {
	case ColdRay:
	case ColdRing:
		// +50%
		if(is(EvercoldIcicle))
			value += value * getpower(EvercoldIcicle) / 100;
		// -50%
		if(leader && leader->is(IceCloack))
			value -= value * getpower(IceCloack) / 100;
		break;
	case FireBall:
	case FireBlast:
		if(is(EvercoldIcicle))
			value += value * getpower(EverhotLavaRock) / 100;
		if(leader && leader->is(FireCloack))
			value -= value * getpower(IceCloack) / 100;
		break;
	case LightingBolt:
		if(is(LightingRod))
			value += value * getpower(LightingRod) / 100;
		if(is(LightingHelm))
			value -= value * getpower(LightingHelm) / 100;
		break;
	case ElementalStorm:
	case Armagedon:
		break;
	}
	return value;
}

void heroi::cast(spell_s id, const pvar& target) {
	auto& spell = bsmeta<spelli>::elements[id];
	auto power = get(SpellPower);
	if(spell.tags.is(Enchantment)) {
		if(target.type == Unit)
			target.unit->enchant(id, power, this);
	}
}