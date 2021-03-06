#include "main.h"

monsteri bsmeta<monsteri>::elements[] = {{Knight, 1, 0, 1, 1, 1, 1, 1, 1, 12, 0, {}, "����������", "��������", {20, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 33},
{Knight, 1, 0, 5, 3, 2, 3, 10, 1, 8, 12, {}, "������", "��������", {150, 0, 0, 0, 0, 0, 0}, Dwelving2, Ranger, 312},
{Knight, 2, 0, 5, 3, 2, 3, 10, 3, 8, 24, {Twice}, "��������", "����������", {200, 0, 0, 0, 0, 0, 0}, Dwelving2u, Peasant, 463},
{Knight, 2, 0, 5, 9, 3, 4, 15, 3, 5, 0, {}, "��������", "����������", {200, 0, 0, 0, 0, 0, 0}, Dwelving3, VeteranPikeman, 639},
{Knight, 2, 0, 5, 9, 3, 4, 20, 4, 5, 0, {}, "������� ��������", "��������� ����������", {250, 0, 0, 0, 0, 0, 0}, Dwelving3u, Peasant, 824},
{Knight, 3, 0, 7, 9, 4, 6, 25, 3, 4, 0, {}, "������", "��������", {250, 0, 0, 0, 0, 0, 0}, Dwelving4, MasterSwordsman, 1130},
{Knight, 3, 0, 7, 9, 4, 6, 30, 4, 4, 0, {}, "������ ������", "�������� ��������", {300, 0, 0, 0, 0, 0, 0}, Dwelving4u, Peasant, 1350},
{Knight, 3, 0, 10, 9, 5, 10, 30, 5, 3, 0, {Wide}, "����������", "���������", {300, 0, 0, 0, 0, 0, 0}, Dwelving5, Champion, 1830},
{Knight, 3, 0, 10, 9, 5, 10, 40, 6, 3, 0, {Wide}, "�������", "���������", {375, 0, 0, 0, 0, 0, 0}, Dwelving5u, Peasant, 2273},
{Knight, 4, 0, 11, 12, 10, 20, 50, 4, 2, 0, {Twice}, "�������", "���������", {600, 0, 0, 0, 0, 0, 0}, Dwelving6, Crusader, 4704},
{Knight, 4, 0, 11, 12, 10, 20, 65, 5, 2, 0, {Twice}, "�����������", "������������", {1000, 0, 0, 0, 0, 0, 0}, Dwelving6u, Peasant, 5822},
//
{Barbarian, 1, 0, 3, 1, 1, 2, 3, 3, 10, 0, {}, "������", "��������", {40, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 109},
{Barbarian, 1, 0, 3, 4, 2, 3, 10, 1, 8, 8, {}, "���", "�����", {140, 0, 0, 0, 0, 0, 0}, Dwelving2, OrcChief, 299},
{Barbarian, 2, 0, 3, 4, 3, 4, 15, 2, 8, 16, {}, "������ ���", "������ �����", {175, 0, 0, 0, 0, 0, 0}, Dwelving2u, Peasant, 512},
{Barbarian, 2, 0, 6, 2, 3, 5, 20, 5, 5, 0, {Twice, Wide}, "����", "������", {200, 0, 0, 0, 0, 0, 0}, Dwelving3, Peasant, 865},
{Barbarian, 3, 0, 9, 5, 4, 6, 40, 1, 4, 0, {}, "���", "�����", {300, 0, 0, 0, 0, 0, 0}, Dwelving4, OgreLord, 1065},
{Barbarian, 3, 0, 9, 5, 5, 7, 60, 3, 4, 0, {}, "������� ���", "������� �����", {500, 0, 0, 0, 0, 0, 0}, Dwelving4u, Peasant, 2070},
{Barbarian, 3, 0, 10, 5, 5, 7, 40, 3, 3, 8, {}, "������", "�������", {600, 0, 0, 0, 0, 0, 0}, Dwelving5, WarTroll, 1921},
{Barbarian, 3, 0, 10, 5, 7, 9, 40, 4, 3, 16, {}, "������ ������", "������ �������", {700, 0, 0, 0, 0, 0, 0}, Dwelving5u, Peasant, 2337},
{Barbarian, 4, 0, 12, 9, 12, 24, 80, 4, 2, 0, {Area}, "������", "��������", {750, 0, 0, 0, 0, 1, 0}, Dwelving6, Peasant, 6074},
//
{Sorcerer, 1, 0, 4, 2, 1, 2, 2, 3, 8, 0, {Fly, Stealth}, "���", "���", {50, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 129},
{Sorcerer, 2, 0, 6, 5, 2, 4, 20, 1, 6, 0, {}, "�����", "�������", {200, 0, 0, 0, 0, 0, 0}, Dwelving2, BattleDwarf, 500},
{Sorcerer, 2, 0, 6, 6, 2, 4, 20, 3, 6, 0, {}, "������ �����", "������ �������", {250, 0, 0, 0, 0, 0, 0}, Dwelving2u, Peasant, 716},
{Sorcerer, 2, 0, 4, 3, 2, 3, 15, 3, 4, 24, {Twice}, "����", "������", {250, 0, 0, 0, 0, 0, 0}, Dwelving3, GrandElf, 554},
{Sorcerer, 2, 0, 5, 5, 2, 3, 15, 5, 4, 24, {Twice}, "������� ����", "������� ������", {300, 0, 0, 0, 0, 0, 0}, Dwelving3u, Peasant, 658},
{Sorcerer, 3, 0, 7, 5, 5, 8, 25, 4, 3, 8, {}, "�����", "�������", {350, 0, 0, 0, 0, 0, 0}, Dwelving4, GreaterDruid, 1290},
{Sorcerer, 3, 0, 7, 7, 5, 8, 25, 5, 3, 16, {}, "������� �����", "������� �������", {400, 0, 0, 0, 0, 0, 0}, Dwelving4u, Peasant, 1428},
{Sorcerer, 4, 0, 10, 9, 7, 14, 40, 4, 2, 0, {Wide}, "��������", "����������", {500, 0, 0, 0, 0, 0, 0}, Dwelving5, Peasant, 2702},
{Sorcerer, 4, 0, 12, 10, 20, 40, 100, 6, 1, 0, {Area, Dragon, Fly, Wide}, "������", "��������", {1500, 0, 1, 0, 0, 0, 0}, Dwelving6, Peasant, 10114},
//
{Warlock, 1, 0, 3, 1, 1, 2, 5, 3, 8, 8, {Wide}, "�������", "���������", {60, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 154},
{Warlock, 2, 0, 4, 7, 2, 3, 15, 5, 6, 0, {Fly}, "��������", "��������", {200, 0, 0, 0, 0, 0, 0}, Dwelving2, Peasant, 579},
{Warlock, 3, 0, 6, 6, 3, 5, 25, 3, 4, 0, {Fly, Wide}, "������", "��������", {300, 0, 0, 0, 0, 0, 0}, Dwelving3, Peasant, 1101},
{Warlock, 3, 0, 9, 8, 5, 10, 35, 3, 3, 0, {}, "��������", "����������", {400, 0, 0, 0, 0, 0, 0}, Dwelving4, MinotaurKing, 1751},
{Warlock, 3, 0, 9, 8, 5, 10, 45, 5, 3, 0, {}, "������ ��������", "������ ����������", {500, 0, 0, 0, 0, 0, 0}, Dwelving4u, Peasant, 2252},
{Warlock, 4, 0, 8, 9, 6, 12, 75, 1, 2, 0, {LargeArea, Stealth, Wide}, "�����", "����", {800, 0, 0, 0, 0, 0, 0}, Dwelving5, Peasant, 2878},
{Warlock, 4, 0, 12, 12, 25, 50, 200, 3, 1, 0, {Dragon, Area, Fly, Wide}, "������� ������", "������� ��������", {3000, 0, 0, 0, 1, 0, 0}, Dwelving6, RedDragon, 18153},
{Warlock, 4, 0, 13, 13, 25, 50, 250, 4, 1, 0, {Dragon, Area, Fly, Wide}, "������� ������", "������� ��������", {3500, 0, 0, 0, 1, 0, 0}, Dwelving6u, BlackDragon, 22962},
{Warlock, 4, 0, 14, 14, 25, 50, 300, 5, 1, 0, {Dragon, Area, Fly, Wide}, "������ ������", "������ ��������", {4000, 0, 0, 0, 2, 0, 0}, Dwelving6u2, Peasant, 28144},
//
{Wizard, 1, 0, 2, 1, 1, 3, 3, 2, 8, 12, {}, "����������", "������������", {50, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 134},
{Wizard, 2, 0, 5, 4, 2, 3, 15, 5, 6, 0, {Wide}, "�����", "�������", {150, 0, 0, 0, 0, 0, 0}, Dwelving2, Peasant, 493},
{Wizard, 2, 0, 5, 10, 4, 5, 30, 1, 4, 0, {}, "�������� �����", "�������� �������", {300, 0, 0, 0, 0, 0, 0}, Dwelving3, SteelGolem, 951},
{Wizard, 3, 0, 7, 10, 4, 5, 35, 2, 4, 0, {}, "�������� �����", "�������� �������", {350, 0, 0, 0, 0, 0, 0}, Dwelving3u, Peasant, 1324},
{Wizard, 3, 0, 7, 7, 4, 8, 40, 3, 3, 0, {Fly, Wide}, "����", "�����", {400, 0, 0, 0, 0, 0, 0}, Dwelving4, Peasant, 1739},
{Wizard, 3, 0, 11, 7, 7, 9, 30, 4, 2, 12, {MeleeArcher}, "���", "�����", {600, 0, 0, 0, 0, 0, 0}, Dwelving5, ArchMage, 1935},
{Wizard, 3, 0, 12, 8, 7, 9, 35, 5, 2, 24, {MeleeArcher}, "�������", "���������", {700, 0, 0, 0, 0, 0, 0}, Dwelving5u, Peasant, 2469},
{Wizard, 4, 0, 13, 10, 20, 30, 150, 3, 1, 0, {}, "������", "��������", {2000, 0, 0, 0, 0, 0, 1}, Dwelving6, Titan, 9589},
{Wizard, 4, 0, 15, 15, 20, 30, 300, 5, 1, 24, {}, "�����", "�������", {5000, 0, 0, 0, 0, 0, 2}, Dwelving6u, Peasant, 22933},
//
{Necromancer, 1, 0, 4, 3, 2, 3, 4, 3, 8, 0, {Undead}, "������", "��������", {75, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 203},
{Necromancer, 1, 1, 5, 2, 2, 3, 15, 1, 6, 0, {Undead}, "�����", "�����", {150, 0, 0, 0, 0, 0, 0}, Dwelving2, MutantZombie, 310},
{Necromancer, 2, 1, 5, 2, 2, 3, 25, 3, 6, 0, {Undead}, "����� ������", "����� ��������", {200, 0, 0, 0, 0, 0, 0}, Dwelving2u, Peasant, 506},
{Necromancer, 2, 0, 6, 6, 3, 4, 25, 3, 4, 0, {Undead}, "�����", "�����", {250, 0, 0, 0, 0, 0, 0}, Dwelving3, RoyalMummy, 868},
{Necromancer, 3, 0, 6, 6, 3, 4, 30, 4, 4, 0, {Undead}, "���������� �����", "����������� �����", {300, 0, 0, 0, 0, 0, 0}, Dwelving3u, Peasant, 1056},
{Necromancer, 3, 0, 8, 6, 5, 7, 30, 3, 3, 0, {Undead, Fly, Stealth}, "������", "��������", {500, 0, 0, 0, 0, 0, 0}, Dwelving4, VampireLord, 1685},
{Necromancer, 3, 0, 8, 6, 5, 7, 40, 4, 3, 0, {Undead, Fly, Stealth}, "������-����", "������ ��������", {650, 0, 0, 0, 0, 0, 0}, Dwelving4u, Peasant, 2461},
{Necromancer, 3, 0, 7, 12, 8, 10, 25, 4, 2, 12, {Undead, MeleeArcher}, "���", "�����", {750, 0, 0, 0, 0, 0, 0}, Dwelving5, PowerLich, 2069},
{Necromancer, 4, 0, 7, 13, 8, 10, 35, 5, 2, 24, {Undead, MeleeArcher}, "������� ���", "������� �����", {900, 0, 0, 0, 0, 0, 0}, Dwelving5u, Peasant, 2625},
{Necromancer, 4, 0, 11, 9, 25, 45, 150, 3, 1, 0, {Dragon, Undead, Fly, Wide}, "�������� ������", "�������� ��������", {1500, 0, 0, 0, 0, 0, 0}, Dwelving6, Peasant, 11744},
//
{RandomKind, 1, 0, 6, 1, 1, 2, 4, 4, 4, 0, {Stealth}, "������", "��������", {50, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 177},
{RandomKind, 2, 0, 7, 6, 2, 5, 20, 5, 4, 0, {Wide}, "��������", "����������", {200, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 805},
{RandomKind, 3, 0, 8, 7, 4, 6, 20, 4, 4, 0, {Fly}, "�������", "���������", {1000, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 1545},
{RandomKind, 4, 0, 10, 9, 20, 30, 50, 5, 4, 0, {Fly}, "����", "������", {650, 0, 0, 0, 0, 0, 1}, Dwelving1, Peasant, 5692},
{RandomKind, 3, 0, 8, 9, 6, 10, 35, 3, 4, 0, {Wide}, "������", "�����", {500, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 1975},
{RandomKind, 3, 0, 8, 8, 4, 5, 50, 2, 4, 0, {Elemental}, "�������� ����������", "�������� �����������", {500, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 1732},
{RandomKind, 3, 0, 7, 7, 2, 8, 35, 5, 4, 0, {Elemental, Lighting}, "��������� ����������", "��������� �����������", {500, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 1412},
{RandomKind, 3, 0, 8, 6, 4, 6, 40, 4, 4, 0, {Elemental, Fire}, "�������� ����������", "�������� �����������", {500, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 1501},
{RandomKind, 3, 0, 6, 8, 3, 7, 45, 3, 4, 0, {Elemental, Cold}, "������ ����������", "������ �����������", {500, 0, 0, 0, 0, 0, 0}, Dwelving1, Peasant, 1690},
//
{RandomKind, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {}, "Random Monster", "Random Monsters", {0, 0, 0, 0, 0, 0, 0}},
{RandomKind, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {}, "Random Monster 1", "Random Monsters 1", {0, 0, 0, 0, 0, 0, 0}},
{RandomKind, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {}, "Random Monster 2", "Random Monsters 2", {0, 0, 0, 0, 0, 0, 0}},
{RandomKind, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {}, "Random Monster 3", "Random Monsters 3", {0, 0, 0, 0, 0, 0, 0}},
{RandomKind, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, {}, "Random Monster 4", "Random Monsters 4", {0, 0, 0, 0, 0, 0, 0}},
};

monster_s castlei::getmonster(building_s building, kind_s kind) {
	for(auto& e : bsmeta<monsteri>::elements) {
		if(e.type == kind && e.building == building)
			return e.getid();
	}
	return RandomMonster;
}

monster_s monsteri::getid() const {
	return (monster_s)(this - bsmeta<monsteri>::elements);
}