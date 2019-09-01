#include "main.h"

static struct heroin {
	const char*		name;
	kind_s			kind;
} hero_data[] = {{"���� �������", Knight},
{"��� �������", Knight},
{"�����", Knight},
{"�������", Knight},
{"����", Knight},
{"�������", Knight},
{"����", Knight},
{"��������", Knight},
{"��������", Knight},
//
{"����������", Barbarian},
{"�������������", Barbarian},
{"�������", Barbarian},
{"���� ���", Barbarian},
{"��������", Barbarian},
{"������", Barbarian},
{"�����", Barbarian},
{"�����", Barbarian},
{"�����", Barbarian},
//
{"�����", Sorcerer},
{"������", Sorcerer},
{"�����", Sorcerer},
{"��������", Sorcerer},
{"������", Sorcerer},
{"����", Sorcerer},
{"������", Sorcerer},
{"�������", Sorcerer},
{"����", Sorcerer},
//
{"���", Warlock},
{"������", Warlock},
{"������", Warlock},
{"�����", Warlock},
{"�����", Warlock},
{"������", Warlock},
{"����", Warlock},
{"�������", Warlock},
{"��������", Warlock},
//
{"����", Wizard},
{"�����", Wizard},
{"����", Wizard},
{"�����", Wizard},
{"������", Wizard},
{"�������", Wizard},
{"�������", Wizard},
{"��������", Wizard},
{"��������", Wizard},
//
{"���", Necromancer},
{"�������", Necromancer},
{"���", Necromancer},
{"�����", Necromancer},
{"������", Necromancer},
{"������", Necromancer},
{"�������", Necromancer},
{"������", Necromancer},
{"�����", Necromancer},
//
{"������", Wizard},
{"��������", Knight},
{"�����", Sorcerer},
{"���������", Warlock},
{"������", Knight},
{"����", Necromancer},
//
{"�������", Barbarian},
{"�������", Knight},
{"�������", Necromancer},
{"�������", Sorcerer},
{"�������", Warlock},
{"�������", Wizard},
};
heroi bsmeta<heroi>::elements[CaptainWizard + 1];

const costi	heroi::cost = {{2500}};

void heroi::initialize() {
	for(auto i = 0; i <= CaptainWizard; i++)
		bsmeta<heroi>::elements[i].clear();
}

void heroi::refresh() {
	abilities[SpellPoints] = getmaximum(SpellPoints);
	abilities[MovePoints] = getmaximum(MovePoints);
}

void heroi::clear() {
	auto& d = hero_data[getid()];
	memset(this, 0, sizeof(*this));
	zcpy(name, d.name);
	player = RandomPlayer;
	kind = d.kind;
	level = 1;
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
	refresh();
}

playeri* heroi::getplayer() const {
	if(player == RandomPlayer)
		return 0;
	return bsmeta<playeri>::elements + player;
}

int heroi::get(ability_s v) const {
	int r;
	switch(v) {
	case Attack:
	case Defence:
	case SpellPower:
	case Knowledge:
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

int	heroi::getmaximum(ability_s v) const {
	switch(v) {
	case SpellPoints: return get(Knowledge) * 10;
	case MovePoints: return 1000;
	default: return 0;
	}
}

void heroi::add(artifact_s id) {
	for(auto& e : artifacts) {
		if(e)
			continue;
		e = id;
		break;
	}
}

int	heroi::getcost(spell_s v) const {
	return bsmeta<spelli>::elements[v].points;
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
		if(e.getplayer()!=player)
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