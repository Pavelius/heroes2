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

static void test_battle() {
	heroi::initialize();
	auto ph = bsmeta<heroi>::elements + Natasha;
	auto pd = bsmeta<heroi>::elements + RandomHero;
	pd->clear();
	pd->setpos(4640);
	pd->armyi::clear();
	pd->add(Skeleton, 30);
	pd->add(Zombie, 4);
	ph->set(MagicArrow);
	ph->set(PlayerBlue);
	ph->add(WarTroll, 4);
	ph->setup_battle(pd);
	ph->battlestart();
}

static void test_experience() {
	heroi::initialize();
	setevil(true);
	auto& e = bsmeta<heroi>::elements[Jezebel];
	e.addexperience(5000);
	e.show(true);
}

static void load_game() {}
void dialog_palette();
void animation_view();

static void main_menu() {
	animation latern;
	while(ismodal()) {
		image(0, 0, HEROES, 0, 0, 0);
		button(0, 0, BTNSHNGL, gamei::newgame, {1, 2, 3}, 0, "Начать новую одиночную или мультиплеер игру.");
		button(0, 0, BTNSHNGL, dialog_palette, {5, 6, 7}, Ctrl + Alpha + 'L', "Загрузить ранее сохраненную игру.");
		button(0, 0, BTNSHNGL, hightscore::show, {9, 10, 11});
		button(0, 0, BTNSHNGL, test_battle, {13, 14, 15});
		button(0, 0, BTNSHNGL, buttoncancel, {17, 18, 19});
		latern.paint(0, 0);
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
	setevil(true);
	draw::initialize();
	drawobji::initialize();
	before_open();
	if(!draw::create("test", 190, false))
		return -1;
	srand(21837);
	util_main();
	//animation_view();
	main_menu();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main();
}