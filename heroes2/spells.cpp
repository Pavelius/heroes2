#include "main.h"

spelli bsmeta<spelli>::elements[] = {{"�������� ���", 9, 3, 8, 1, {Hostile, Area, Damage, Fire}, CombatSpell, "���������� �������� ��� ���� ������� � ��������� �������, ������� ���� ������� ������."},
{"�������� �����", 15, 4, 9, 15, {Hostile, LargeArea, Damage, Fire}, CombatSpell, "���������� ������ ��������� ���� �������� ��� ������ ������ ����� ��������� ������ �����. ������� %1i ����� �� ������ ������� ���� �����."},
{"������", 7, 2, 4, 25, {Hostile, Damage, Lighting}, CombatSpell, "������ ������������� ������� �������� ��������� �������� ������ %1i ����� �� ������� ���� �����."},
{"���� ������", 15, 4, 5, 40, {Hostile, Damage, Lighting}, CombatSpell, "������������� ������ �������� ��������� ��������, ����� �������� �������� �������� � ��������� �����, ����� ��������� �������� �������� � ��������� ����� � �.�. �������� - ��� ���������� ����� �������� ������������� �������."},
{"��������", 9, 3, 10, 0, {Friendly}, CombatSpell, "���������� ��������, ������� �� ������� � ����� �������� ������� �� ���� �����."},
{"�������", 6, 1, 6, 5, {Friendly}, CombatSpell, "������� ��� ���������� ���������� � ������ ������ ����� � ��������������� ������ �� %1i ����� �� ������� ���� �����."},
{"����� �������", 15, 3, 2, 5, {Friendly, Mass}, CombatSpell, "������� ��� ���������� ���������� � ��������������� ������ �� %1i ����� �� ������� ���� ����� � ���� ����� �������."},
{"���������", 12, 4, 13, 50, {Friendly}, CombatSpell, "�������� ������������ ��� ������� �������� �� ����� ���."},
{"�������� ���������", 15, 5, 12, 50, {Friendly}, CombatSpell, "�������� �������� ������������ ��� ������� ��������."},
{"���������", 3, 1, 14, 2, {Friendly, Enchantment}, CombatSpell, "���������� �������� �������� �� %1i."},
{"����� ���������", 10, 3, 14, 2, {Friendly, Mass, Enchantment}, CombatSpell, "���������� �������� ���� ����� �������� �� %1i."},
{"����������", 3, 1, 1, 0, {Hostile, Enchantment}, CombatSpell, "��������� ���� �� �������� �� ��������."},
{"����� ����������", 15, 4, 1, 0, {Hostile, Mass, Enchantment}, CombatSpell, "��������� ���� ������ �� �������� �� ��������."},
//
{"����������", 6, 2, 21, 0, {Hostile, Enchantment}, CombatSpell, "���, ���������� ����� ���������� ������ ����������� �� ������."},
{"�������������", 3, 1, 7, 0, {Friendly, Enchantment}, CombatSpell, "��������� ����� ������� ������������ �����������."},
{"����� �������������", 12, 3, 7, 0, {Friendly, Mass, Enchantment}, CombatSpell, "��� ���� ������� ����� �������� ������������ ����."},
{"�������� ����", 3, 1, 31, 3, {Friendly, Enchantment}, CombatSpell, "�������� ������ ���������� �������� �� %1i."},
{"�������� ����", 6, 2, 30, 5, {Friendly, Enchantment}, CombatSpell, "�������� ������ ���������� �������� �� % 1i. ��� ���������� ������ �������� ����."},
{"���������", 3, 1, 3, 0, {Hostile, Enchantment}, CombatSpell, "��������� ��������� ����� ������� ����������� �����������."},
{"����� ���������", 12, 3, 3, 0, {Hostile, Mass, Enchantment}, CombatSpell, "��� ��������� ������ ������� ����������� �����������."},
{"������ �����", 9, 3, 22, 10, {Hostile, Mass, Damage}, CombatSpell, "������� ����������� ���� ��������� � �����. ���� ����� %1i �� ������ ������� ���� �����."},
{"������ ����", 12, 4, 23, 20, {Hostile, Mass, Damage}, CombatSpell, "������� ����������� ���� ��������� � �����. ���� ����� %1i �� ������ ������� ���� �����. ��� ���������� ������ ������� �����."},
{"���������", 7, 3, 17, 0, {Friendly, Hostile, Enchantment}, CombatSpell, "�������� ��������� �������� �� ���� ����������."},
{"�������� �����", 5, 1, 18, 0, {Friendly}, CombatSpell, "������� ��� ���������� � ��������� ����."},
{"����� �����������", 12, 3, 18, 0, {Friendly, Hostile, Mass}, CombatSpell, "������� ��� ���������� �� ���� �������."},
{"��������� ������", 3, 1, 38, 10, {Hostile, Damage}, CombatSpell, "��������� ������ ������� ���� ���������� ��������. ���� ����� %1i �� ������ ������� ���� �����."},
{"���������", 12, 3, 19, 0, {Hostile}, CombatSpell, "���������� �������� ��������� ��� ������."},
{"���������", 20, 5, 16, 50, {Damage, Hostile, Mass, Fire}, CombatSpell, "������� ����� �������� ��� ���� ����� ������ �������� ����������� ��� �������������. ���� ����� %1i �� ������ ������� ���� �����."},
{"���� ���������", 15, 4, 11, 25, {Damage, Hostile, Mass}, CombatSpell, "�������� ������ ������������ �� ���� �����, ������ %1i ����� �� ������ ������� ���� ����� ������� ��������."},
{"����������� �����", 15, 4, 24, 25, {Hostile, Area, Damage, Earth}, CombatSpell, "����������� ����� ������������ � ���� � ������� ���� �������� ��������� %1i ����� �� ������ ������� ���� �����."},
{"�������", 9, 3, 20, 0, {Hostile}, CombatSpell, "��������� �������� ������������ � ������ ����������� ��������� � �������� �� �����."},
{"������", 15, 5, 37, 50, {Hostile}, CombatSpell, "��������� ���� �� ���� ����� ��� ��������� ��� ��� ��������, ���� ���������� ��� ����� ����� ��� %1i �� ������� ���� �����."},
{"��� ������", 6, 2, 36, 20, {Damage, Hostile, Cold}, CombatSpell, "���������� ��������� ����� � ���������� ������. ������� %1i ����� �� ������ ������� ���� �����."},
{"������ ������", 9, 3, 35, 10, {Damage, Hostile, Cold}, CombatSpell, "���������� ��������� ����� �� ���� ������� ������ ������, �� �� ������� �����. ������� %1i ����� �� ������ ������� ���� �����."},
{"��� ����������", 7, 2, 34, 3, {Enchantment, Hostile}, CombatSpell, "������� ������ ���������� ������ �� %1i."},
{"����� ������", 6, 2, 28, 5, {Damage, Hostile}, CombatSpell, "������� %1i �� ������ ������� ���� ����� ����� ���� ����� (�� ���������) �� ���� ���."},
{"����� ������", 10, 3, 29, 10, {Damage, Hostile}, CombatSpell, "������� %1i �� ������ ������� ���� ����� ����� ���� ����� (�� ���������) �� ���� ���. ��� ���������� ������ ���������� ����� ������."},
{"������ ��������", 6, 2, 32, 6, {Enchantment, Friendly}, CombatSpell, "�������� �� %1i ����� ���������� ������ ������ ��������."},
{"����� �����", 3, 1, 27, 3, {Enchantment, Friendly}, CombatSpell, "����������� ����� ������ �� %1i."},
{"������� �������", 10, 3, 25, 50, {Friendly}, CombatSpell, "�������� ������ � ��� ���������."},
{"�������", 25, 5, 26, 0, {Friendly}, CombatSpell, "Creates an illusionary unit that duplicates one of your existing units.  This illusionary unit does the same damages as the original, but will vanish if it takes any damage."},
{"���", 3, 1, 15, 2, {Enchantment, Friendly}, CombatSpell, "������� �������� ����� �� ������������� ����, ������������ � ��������� �����."},
{"����� ���", 7, 4, 15, 0, {Enchantment, Friendly}, CombatSpell, "Halves damage received from ranged attacks for all of your units."},
{"����� ���������� �����", 30, 5, 56, 3, {Friendly, Summon, Earth}, CombatSpell, "Summons Earth Elementals to fight for your army."},
{"����� ���������� �������", 30, 5, 57, 3, {Friendly, Summon, Air}, CombatSpell, "Summons Air Elementals to fight for your army."},
{"����� ���������� ����", 30, 5, 58, 3, {Friendly, Summon, Fire}, CombatSpell, "Summons Fire Elementals to fight for your army."},
{"����� ���������� ����", 30, 5, 59, 3, {Friendly, Summon, Water}, CombatSpell, "Summons Water Elementals to fight for your army."},
{"�������������", 15, 4, 33, 0, {Hostile, Mass, Earth}, CombatSpell, "Damages castle walls."},
// Adventure spells
{"�������� �����", 1, 1, 39, 0, {}, AdventureSpell, "Causes all mines across the land to become visible."},
{"�������� �������", 1, 2, 40, 0, {}, AdventureSpell, "Causes all res::tokens across the land to become visible."},
{"�������� ���������", 2, 2, 41, 0, {}, AdventureSpell, "Causes all artifacts across the land to become visible."},
{"�������� ������", 2, 3, 42, 0, {}, AdventureSpell, "Causes all towns and castles across the land to become visible."},
{"�������� ������", 2, 3, 43, 0, {}, AdventureSpell, "Causes all Heroes across the land to become visible."},
{"�������� ���", 3, 4, 44, 0, {}, AdventureSpell, "��� ����� ������ ���������� �����."},
{"�������� �����", 3, 4, 45, 0, {}, AdventureSpell, "Allows the caster to view detailed information on enemy Heroes."},
{"������� �������", 5, 2, 46, 0, {}, AdventureSpell, "Summons the nearest unoccupied, friendly boat to an adjacent shore location.  A friendly boat is one which you just built or were the most recent player to occupy."},
{"������", 10, 5, 47, 0, {}, AdventureSpell, "Allows the caster to magically transport to a nearby location."},
{"��������� �����", 10, 4, 48, 0, {}, AdventureSpell, "Returns the caster to any town or castle currently owned."},
{"��������� ������", 20, 5, 49, 0, {}, AdventureSpell, "Returns the hero to the town or castle of choice, provided it is controlled by you."},
{"�������", 6, 2, 50, 3, {}, AdventureSpell, "Visions predicts the likely outcome of an encounter with a neutral army camp."},
{"����������", 8, 2, 51, 4, {}, AdventureSpell, "Haunts a mine you control with Ghosts.  This mine stops producing res::tokens.  (If I can't keep it, nobody will!)"},
{"������ �����", 15, 4, 52, 4, {Earth}, AdventureSpell, "Sets Earth Elementals to guard a mine against enemy armies."},
{"������ �������", 15, 4, 53, 4, {Air}, AdventureSpell, "Sets Air Elementals to guard a mine against enemy armies."},
{"������ ����", 15, 4, 54, 4, {Fire}, AdventureSpell, "Sets Fire Elementals to guard a mine against enemy armies."},
{"������ ����", 15, 4, 55, 4, {Water}, AdventureSpell, "������������� ������ � ������� ����������� ��� ������ �����."},
{"����������", 0, 0, 0, 0, {Hostile}, AdventureSpell, "Stone spell from Medusa."},
};

//int game::getframe(int rec) {
//	if(rec >= FirstSpell && rec <= LastSpell) {
//		switch(rec) {
//		case SpellBloodLust: return 9;
//		case SpellBless: return 3;
//		case SpellHaste: return 0;
//		case SpellShield: return 10;
//		case SpellStoneSkin: return 13;
//		case SpellDragonSlayer: return 8;
//		case SpellSteelSkin: return 14;
//		case SpellAntimagic: return 12;
//		case SpellCurse: return 4;
//		case SpellSlow: return 1;
//		case SpellBerserker: return 5;
//		case SpellHypnotize: return 7;
//		case SpellBlind: return 2;
//		case SpellParalyze: return 6;
//		case SpellStone: return 11;
//		default: return 100;
//		}
//	}
//	return 0;
//}