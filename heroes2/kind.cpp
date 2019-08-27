#include "main.h"

kindi bsmeta<kindi>::elements[] = {{"Варвар", "Варвар", {3, 1, 1, 1}, {Pathfinding, Pathfinding}, {Goblin, Orc}},
{"Рыцарь", "Рыцарь", {2, 2, 1, 1}, {Leadership, Ballistics}, {Peasant, Archer}},
{"Некромант", "Некром.", {1, 0, 3, 2}, {Necromancy, WisdomSkill}, {Skeleton, Zombie}, Haste},
{"Волшебница", "Волшеб.", {0, 0, 3, 4}, {Navigation, WisdomSkill}, {Sprite, Dwarf}, Bless},
{"Колдун", "Колдун", {0, 0, 4, 3}, {WisdomSkill, WisdomSkill}, {Centaur, Gargoyle}, Curse},
{"Чародей", "Чародей", {0, 1, 3, 3}, {Scounting, WisdomSkill}, {Halfling, Boar}, StoneSkin},
{"Случайно", "Случ.", {}, {}, {}},
};
assert_enum(kind, RandomKind);