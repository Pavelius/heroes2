#include "main.h"

kindi bsmeta<kindi>::elements[] = {{"������", {3, 1, 1, 1}, {Pathfinding, Pathfinding}, {Goblin, Orc}},
{"������", {2, 2, 1, 1}, {Leadership, Ballistics}, {Peasant, Archer}},
{"���������", {1, 0, 3, 2}, {Necromancy, WisdomSkill}, {Skeleton, Zombie}, Haste},
{"����������", {0, 0, 3, 4}, {Navigation, WisdomSkill}, {Sprite, Dwarf}, Bless},
{"������", {0, 0, 4, 3}, {WisdomSkill, WisdomSkill}, {Centaur, Gargoyle}, Curse},
{"�������", {0, 1, 3, 3}, {Scounting, WisdomSkill}, {Halfling, Boar}, StoneSkin},
{"�������", {2, 2, 0, 0}, {Leadership, Ballistics}, {Rogue, Nomand}},
};
assert_enum(kind, Hirelings);