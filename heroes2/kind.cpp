#include "main.h"

kindi bsmeta<kindi>::elements[] = {{"Варвар", {3, 1, 1, 1}, {Pathfinding, Pathfinding}, {Goblin, Orc}},
{"Рыцарь", {2, 2, 1, 1}, {Leadership, Ballistics}, {Peasant, Archer}},
{"Некромант", {1, 0, 3, 2}, {Necromancy, WisdomSkill}, {Skeleton, Zombie}, Haste},
{"Волшебница", {0, 0, 3, 4}, {Navigation, WisdomSkill}, {Sprite, Dwarf}, Bless},
{"Колдун", {0, 0, 4, 3}, {WisdomSkill, WisdomSkill}, {Centaur, Gargoyle}, Curse},
{"Чародей", {0, 1, 3, 3}, {Scounting, WisdomSkill}, {Halfling, Boar}, StoneSkin},
{"Наемник", {2, 2, 0, 0}, {Leadership, Ballistics}, {Rogue, Nomand}},
};
assert_enum(kind, Hirelings);