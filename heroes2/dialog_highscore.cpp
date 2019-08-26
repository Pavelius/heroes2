#include "view.h"

using namespace draw;

static hightscore elements[10] = {{"Павелиус", "Мир ветров", 100, 2031, BlackDragon},
{"Лорд Киллбурн", "Жадность", 110, 2000, RedDragon},
{"Сер Галлахарт", "Земли драконов", 100, 1900, Titan},
{"Мириус", "Земли драконов", 100, 1900, Giant},
{"Грейтхарт", "Земли драконов", 100, 1900, Cyclop},
{"Мистли", "Мир ветров", 100, 1900, Troll},
{"Максимус", "Мир ветров", 100, 1900, Wolf},
{"Елена", "Сломанный", 100, 120, Pikeman},
{"Юлианна", "Сломанный", 100, 100, Centaur},
{"Неуч", "Жадность", 100, 1900, Goblin}
};

void hightscore::show() {
	while(ismodal()) {
		image(0, 0, HSBKG, 0, 0);
		image(50, 31, HISCORE, 6);
		int y = 0;
		for(auto& e : elements) {
			if(!e.name[0])
				continue;
			char temp[32];
			text(88, y + 70, e.name);
			text(246, y + 70, e.map);
			zprint(temp, "%1i", e.days);
			text(420 - textw(temp), y + 70, temp);
			zprint(temp, "%1i", e.score);
			text(456, y + 70, temp);
			image(540, y+86, e.monster, y/33);
			y += 40;
		}
		button(9, 315, HISCORE, buttonok, {0, 0, 1});
		button(640 - 36, 315, HISCORE, buttoncancel, {4, 4, 5});
		cursor(ADVMCO, 0);
		domodal();
	}
}