#include "view.h"

using namespace draw;

void object_view();

static void test_message() {
	setevil(true);
	string str;
	str.addsep('\n');
	str.addi(Tsabu);
	str.addh("Форт Месса");
	str.addn("Во время посещения форта вы улучшили некоторые войска в своей армии.");
	str.addsep('\n');
	str.addi(PlayerRed);
	str.addi(PlayerYellow);
	variantcol source[2];
	source[0] = {Luck, 3};
	//message(str, source, 2, ButtonYesNo);
	//message(str, 0, 0, ButtonOK);
	heroi::learn(str, source, 1);
}

static void test_experience() {
	heroi::initialize();
	setevil(true);
	auto& e = bsmeta<heroi>::elements[Jezebel];
	e.addexperience(5000);
	e.show(true);
}

static void show_test() {
	test_experience();
}

static void full_castle(const playeri* player, kind_s type) {
	//heroi::initialize();
	//bsmeta<heroi>::elements[Dimitry].show();
	auto p = bsmeta<castlei>::add();
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

static void load_game() {}
void dialog_palette();

static void main_menu() {
	animation latern;
	while(ismodal()) {
		image(0, 0, HEROES, 0, 0, 0);
		button(0, 0, BTNSHNGL, gamei::newgame, {1, 2, 3}, 0, "Начать новую одиночную или мультиплеер игру.");
		button(0, 0, BTNSHNGL, dialog_palette, {5, 6, 7}, Ctrl + Alpha + 'L', "Загрузить ранее сохраненную игру.");
		button(0, 0, BTNSHNGL, hightscore::show, {9, 10, 11});
		button(0, 0, BTNSHNGL, object_view, {13, 14, 15});
		button(0, 0, BTNSHNGL, buttoncancel, {17, 18, 19});
		//latern.painting({0, 0});
		domodal();
		if(hot::key == InputTimer)
			latern.update();
	}
}

static void before_open() {
	loadb("boptions.dat", &battle, sizeof(battle));
}

void util_main();

int main() {
	variantcol n = {Experience, 1000};
	setevil(true);
	draw::initialize();
	drawobji::initialize();
	before_open();
	if(!draw::create("test", 190, false))
		return -1;
	srand(21837);
	util_main();
	main_menu();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main();
}