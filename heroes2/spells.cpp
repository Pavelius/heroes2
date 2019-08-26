#include "main.h"

spelli bsmeta<spelli>::elements[] = {{"Огненный шар", 9, 3, 8, 1, {Hostile, Area, Damage, Fire}, CombatSpell, "Заставляет огромный шар огня попасть в указанную область, поражая всех существ вокруг."},
{"Огненный взрыв", 15, 4, 9, 15, {Hostile, LargeArea, Damage, Fire}, CombatSpell, "Улучшенная версия Огненного шара поражает две клетки вокруг точки попадания вместо одной. Наносит %1i урона за каждый уровень Силы Магии."},
{"Молния", 7, 2, 4, 25, {Hostile, Damage, Lighting}, CombatSpell, "Разряд электрической энергии поражает указанное существо нанося %1i урона на уровень Силы Магии."},
{"Цепь молний", 15, 4, 5, 40, {Hostile, Damage, Lighting}, CombatSpell, "Электрический разряд поражает выбранное существо, затем поражает соседнее существо с половиной урона, затем следующее соседнее существо с половиной урона и т.д. Внимание - это заклинание может поразить дружественных существ."},
{"Телепорт", 9, 3, 10, 0, {Friendly}, CombatSpell, "Перемещает существо, которое вы выбрали в любую открытую позицию на поле битвы."},
{"Лечение", 6, 1, 6, 5, {Friendly}, CombatSpell, "Убирает все негативные заклинания с одного вашего юнита и восстанавливает вплоть до %1i хитов на уровень Силы Магии."},
{"Общее лечение", 15, 3, 2, 5, {Friendly, Mass}, CombatSpell, "Убирает все негативные заклинания и восстанавливает вплоть до %1i хитов на уровень Силы Магии у всех ваших существ."},
{"Оживление", 12, 4, 13, 50, {Friendly}, CombatSpell, "Оживляет поврежденные или мертвые существа до конца боя."},
{"Истинное оживление", 15, 5, 12, 50, {Friendly}, CombatSpell, "Навсегда оживляет поврежденные или мертвые существа."},
{"Ускорение", 3, 1, 14, 2, {Friendly, Enchantment}, CombatSpell, "Увеличивет скорость существа на %1i."},
{"Общее ускорение", 10, 3, 14, 2, {Friendly, Mass, Enchantment}, CombatSpell, "Увеличивет скорость всех ваших существа на %1i."},
{"Замедление", 3, 1, 1, 0, {Hostile, Enchantment}, CombatSpell, "Замедляет цель до половины ее скорости."},
{"Общее замедление", 15, 4, 1, 0, {Hostile, Mass, Enchantment}, CombatSpell, "Замедляет всех врагов до половины их скорости."},
//
{"Ослепление", 6, 2, 21, 0, {Hostile, Enchantment}, CombatSpell, "Дым, поражающий глаза противника лишает возможности их ходить."},
{"Благословение", 3, 1, 7, 0, {Friendly, Enchantment}, CombatSpell, "Выбранный отряд наносит максимальные повреждения."},
{"Общее благословение", 12, 3, 7, 0, {Friendly, Mass, Enchantment}, CombatSpell, "Все ваши сущетва будут наносить максимальный урон."},
{"Каменная кожа", 3, 1, 31, 3, {Friendly, Enchantment}, CombatSpell, "Повышает защиту выбранного существа на %1i."},
{"Стальная кожа", 6, 2, 30, 5, {Friendly, Enchantment}, CombatSpell, "Повышает защиту выбранного существа на % 1i. Это улучшенная версия Каменной кожи."},
{"Проклятие", 3, 1, 3, 0, {Hostile, Enchantment}, CombatSpell, "Выбранный вражеский отряд наносит минимальные повреждения."},
{"Общее проклятие", 12, 3, 3, 0, {Hostile, Mass, Enchantment}, CombatSpell, "Все вражеские отряды наносят минимальные повреждения."},
{"Святое слово", 9, 3, 22, 10, {Hostile, Mass, Damage}, CombatSpell, "Наносит повреждения всем мертвецам в битве. Урон равен %1i на каждый уровень Силы Магии."},
{"Святой крик", 12, 4, 23, 20, {Hostile, Mass, Damage}, CombatSpell, "Наносит повреждения всем мертвецам в битве. Урон равен %1i на каждый уровень Силы Магии. Это улучшенная версия Святого слова."},
{"Антимагия", 7, 3, 17, 0, {Friendly, Hostile, Enchantment}, CombatSpell, "Защищает выбранное существо от всех заклинаний."},
{"Рассеять магию", 5, 1, 18, 0, {Friendly}, CombatSpell, "Убирает вся заклинания с выбранной цели."},
{"Общее рассеивание", 12, 3, 18, 0, {Friendly, Hostile, Mass}, CombatSpell, "Убирает все заклинания со всех существ."},
{"Волшебная стрела", 3, 1, 38, 10, {Hostile, Damage}, CombatSpell, "Волшебная стрела наносит урон выбранному существу. Урон равен %1i на каждый уровень Силы магии."},
{"Берсеркер", 12, 3, 19, 0, {Hostile}, CombatSpell, "Заставляет существо атаковать его соседа."},
{"Армагедон", 20, 5, 16, 50, {Damage, Hostile, Mass, Fire}, CombatSpell, "Ужасный взрыв поражает все поле битвы нанося страшные повреждения все присуствующим. Урон равен %1i на каждый уровень Силы Магии."},
{"Буря элементов", 15, 4, 11, 25, {Damage, Hostile, Mass}, CombatSpell, "Элементы стихии обрушиваются на поле битвы, нанося %1i урона на каждый уровень Силы Магии каждому существу."},
{"Метеоритный дождь", 15, 4, 24, 25, {Hostile, Area, Damage, Earth}, CombatSpell, "Метеоритный дождь обрушивается с неба и наносит всем соседним существам %1i урона на каждый уровень Силы Магии."},
{"Паралич", 9, 3, 20, 0, {Hostile}, CombatSpell, "Выбранное существо парализовано и лишино возможности двигаться и отвечать на атаку."},
{"Гипноз", 15, 5, 37, 50, {Hostile}, CombatSpell, "Вражеский юнит на один раунд боя переходит под ваш контроль, если количество его хитов менее чем %1i на уровень Силы магии."},
{"Луч холода", 6, 2, 36, 20, {Damage, Hostile, Cold}, CombatSpell, "Высасывает жизненное тепло с вражеского отряда. Наносит %1i урона на каждый уровень Силы Магии."},
{"Кольцо холода", 9, 3, 35, 10, {Damage, Hostile, Cold}, CombatSpell, "Высасывает жизненное тепло из всех отрядов вокруг центра, но не включая центр. Наносит %1i урона на каждый уровень Силы Магии."},
{"Луч разрушения", 7, 2, 34, 3, {Enchantment, Hostile}, CombatSpell, "Снижает защиту вражеского отряда на %1i."},
{"Дрожь смерти", 6, 2, 28, 5, {Damage, Hostile}, CombatSpell, "Наносит %1i на каждый уровень Силы Магии урона всем живым (не мертвецам) на поле боя."},
{"Волна смерти", 10, 3, 29, 10, {Damage, Hostile}, CombatSpell, "Наносит %1i на каждый уровень Силы Магии урона всем живым (не мертвецам) на поле боя. Эта улучшенная версия заклинания Дрожь смерти."},
{"Убийца драконов", 6, 2, 32, 6, {Enchantment, Friendly}, CombatSpell, "Повышает на %1i атаку выбранного отряда против Драконов."},
{"Жажда крови", 3, 1, 27, 3, {Enchantment, Friendly}, CombatSpell, "Увеличивает атаку отряда на %1i."},
{"Оживить мертвых", 10, 3, 25, 50, {Friendly}, CombatSpell, "Оживляет убитых в бою мертвецов."},
{"Иллюзия", 25, 5, 26, 0, {Friendly}, CombatSpell, "Creates an illusionary unit that duplicates one of your existing units.  This illusionary unit does the same damages as the original, but will vanish if it takes any damage."},
{"Щит", 3, 1, 15, 2, {Enchantment, Friendly}, CombatSpell, "Снимает половину урона от дистанционных атак, направленных в выбранный отряд."},
{"Общий щит", 7, 4, 15, 0, {Enchantment, Friendly}, CombatSpell, "Halves damage received from ranged attacks for all of your units."},
{"Вызов элементаля земли", 30, 5, 56, 3, {Friendly, Summon, Earth}, CombatSpell, "Summons Earth Elementals to fight for your army."},
{"Вызов элементаля воздуха", 30, 5, 57, 3, {Friendly, Summon, Air}, CombatSpell, "Summons Air Elementals to fight for your army."},
{"Вызов элементаля огня", 30, 5, 58, 3, {Friendly, Summon, Fire}, CombatSpell, "Summons Fire Elementals to fight for your army."},
{"Вызов элементаля воды", 30, 5, 59, 3, {Friendly, Summon, Water}, CombatSpell, "Summons Water Elementals to fight for your army."},
{"Землятресение", 15, 4, 33, 0, {Hostile, Mass, Earth}, CombatSpell, "Damages castle walls."},
// Adventure spells
{"Показать шахты", 1, 1, 39, 0, {}, AdventureSpell, "Causes all mines across the land to become visible."},
{"Показать ресурсы", 1, 2, 40, 0, {}, AdventureSpell, "Causes all res::tokens across the land to become visible."},
{"Показать артефакты", 2, 2, 41, 0, {}, AdventureSpell, "Causes all artifacts across the land to become visible."},
{"Показать города", 2, 3, 42, 0, {}, AdventureSpell, "Causes all towns and castles across the land to become visible."},
{"Показать героев", 2, 3, 43, 0, {}, AdventureSpell, "Causes all Heroes across the land to become visible."},
{"Показать все", 3, 4, 44, 0, {}, AdventureSpell, "Вся земля вокруг становится видна."},
{"Опознать героя", 3, 4, 45, 0, {}, AdventureSpell, "Allows the caster to view detailed information on enemy Heroes."},
{"Вызвать корабль", 5, 2, 46, 0, {}, AdventureSpell, "Summons the nearest unoccupied, friendly boat to an adjacent shore location.  A friendly boat is one which you just built or were the most recent player to occupy."},
{"Портал", 10, 5, 47, 0, {}, AdventureSpell, "Allows the caster to magically transport to a nearby location."},
{"Городские врата", 10, 4, 48, 0, {}, AdventureSpell, "Returns the caster to any town or castle currently owned."},
{"Городской портал", 20, 5, 49, 0, {}, AdventureSpell, "Returns the hero to the town or castle of choice, provided it is controlled by you."},
{"Видения", 6, 2, 50, 3, {}, AdventureSpell, "Visions predicts the likely outcome of an encounter with a neutral army camp."},
{"Запустение", 8, 2, 51, 4, {}, AdventureSpell, "Haunts a mine you control with Ghosts.  This mine stops producing res::tokens.  (If I can't keep it, nobody will!)"},
{"Стражи земли", 15, 4, 52, 4, {Earth}, AdventureSpell, "Sets Earth Elementals to guard a mine against enemy armies."},
{"Стражи воздуха", 15, 4, 53, 4, {Air}, AdventureSpell, "Sets Air Elementals to guard a mine against enemy armies."},
{"Стражи огня", 15, 4, 54, 4, {Fire}, AdventureSpell, "Sets Fire Elementals to guard a mine against enemy armies."},
{"Стражи воды", 15, 4, 55, 4, {Water}, AdventureSpell, "Устанавливает стражу и водяных элементалей для охраны шахты."},
{"Окаменение", 0, 0, 0, 0, {Hostile}, AdventureSpell, "Stone spell from Medusa."},
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