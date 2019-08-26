#include "view.h"

using namespace draw;

static void test_message() {
	setevil(true);
	variantcol elements[] = {{DivineBreastPlate}, {Ore, 10}, {Wood, 10}};
	message("Во время посещения форта вы улучшили некоторые войска в своей армии.", Knight, Tavern, elements, 3, ButtonOK);
}

static void full_castle(const playeri* player, kind_s type) {
	//heroi::initialize();
	//bsmeta<heroi>::elements[Dimitry].show();
	auto p = castlei::add();
	p->clear();
	p->setname("Гринвейл");
	p->set(player);
	p->set(type);
	p->set(Tent);
	p->set(Castle);
	p->set(Dwelving1);
	p->set(Dwelving2);
	p->set(MageGuild);
	p->set(MageGuild2);
	p->set(MageGuild3);
	p->set(Shipyard);
	p->set(ThievesGuild);
	p->set(Well);
	p->set(Well2);
	//p->set(SpecialBuilding);
	//p->set(RightTurret);
	//p->set(LeftTurret);
	p->set(Moat);
	p->add(Swordsman, 4);
	p->growth();
	p->show();
}

static void random_castle() {
	castlei castle = {};
	castle.random(false);
	castle.set(Necromancer);
	castle.show();
}

static void new_game() {
	//random_castle();
	heroi::initialize();
	playeri::initialize();
	auto p1 = bsmeta<playeri>::elements;
	p1->set(Human);
	p1->setup(EasyDifficulty);
	auto p2 = bsmeta<playeri>::elements + 1;
	p2->set(Computer);
	p2->setup(ImpossibleDifficulty);
	full_castle(p1, Knight);
}

static void load_game() {}

static void main_menu() {
	animation latern;
	while(ismodal()) {
		image(0, 0, HEROES, 0, 0, 0);
		button(0, 0, BTNSHNGL, gamei::newgame, {1, 2, 3}, 0, "Начать новую одиночную или мультиплеер игру.");
		button(0, 0, BTNSHNGL, load_game, {5, 6, 7}, Ctrl + Alpha + 'L', "Загрузить ранее сохраненную игру.");
		button(0, 0, BTNSHNGL, hightscore::show, {9, 10, 11});
		button(0, 0, BTNSHNGL, test_message, {13, 14, 15});
		button(0, 0, BTNSHNGL, new_game, {17, 18, 19});
		//latern.painting({0, 0});
		cursor(ADVMCO, 0);
		domodal();
		if(hot::key == InputTimer)
			latern.update();
	}
}

int main() {
	//setevil(true);
	if(!draw::create("test", 190, false))
		return -1;
	main_menu();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main();
}