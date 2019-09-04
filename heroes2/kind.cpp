#include "main.h"

kindi bsmeta<kindi>::elements[] = {{"Варвар", "Варвар", {3, 1, 1, 1},
{Pathfinding, Pathfinding}, {Goblin, Orc},
{55, 35, 5, 5}, {35, 25, 20, 20}, {4, 3, 3, 4, 2, 3, 3, 1, 1, 2, 3, 1, 0, 2}},
{"Рыцарь", "Рыцарь", {2, 2, 1, 1},
{Leadership, Ballistics}, {Peasant, Archer},
{35, 50, 5, 10}, {25, 35, 20, 20}, {4, 2, 3, 4, 3, 2, 5, 1, 1, 1, 4, 1, 0, 3}},
{"Некромант", "Некром.", {1, 0, 3, 2},
{Necromancy, Wisdow}, {Skeleton, Zombie},
{15, 15, 35, 35}, {25, 25, 25, 25}, {2, 1, 2, 1, 2, 2, 0, 4, 3, 1, 3, 3, 5, 2}, Haste},
{"Волшебница", "Волшеб.", {0, 0, 3, 4},
{Navigation, Wisdow}, {Sprite, Dwarf},
{10, 10, 30, 50}, {55, 35, 5, 5}, {2, 3, 2, 1, 2, 4, 1, 4, 3, 3, 3, 2, 0, 2}, Bless},
{"Колдун", "Колдун", {0, 0, 4, 3},
{Wisdow, Wisdow}, {Centaur, Gargoyle},
{10, 10, 50, 30}, {20, 20, 30, 30}, {2, 1, 2, 4, 2, 2, 1, 4, 3, 1, 3, 3, 1, 2}, Curse},
{"Чародей", "Чародей", {0, 1, 3, 3},
{Scounting, Wisdow}, {Halfling, Boar},
{10, 10, 40, 40}, {20, 20, 30, 30}, {2, 1, 2, 2, 2, 2, 2, 5, 4, 2, 3, 3, 0, 2}, StoneSkin},
{"Случайно", "Случ."},
};
assert_enum(kind, RandomKind);

ability_s kindi::getrandomability(int level) const {
	auto pc = (level >= 10) ? ability_chance_high : ability_chance;
	char weight[Knowledge + 1];
	auto n = 0;
	for(auto i = Attack; i <= Knowledge; i = ability_s(i + 1)) {
		weight[n] = pc[i];
		n++;
	}
	auto r = rand() % 100;
	auto cw = 0;
	for(auto i = Attack; i <= Knowledge; i=ability_s(i+1)) {
		cw += weight[i];
		if(cw > r)
			return i;
	}
	return Attack;
}

variant kindi::getrandomskill(cflags<skill_s> exclude) const {
	skill_s ids[LastSkill + 1];
	char weight[LastSkill + 1] = {};
	auto total_weight = 0;
	auto n = 0;
	for(auto i = FirstSkill; i <= LastSkill; i = skill_s(i + 1)) {
		if(exclude.is(i))
			continue;
		ids[n] = i;
		weight[n] = skill_chance[i];
		total_weight += weight[n];
		n++;
	}
	if(!total_weight)
		return variant();
	auto r = rand() % total_weight;
	auto cw = 0;
	for(auto i = 0; i < n; i++) {
		cw += weight[i];
		if(cw > r)
			return ids[i];
	}
	return variant();
}