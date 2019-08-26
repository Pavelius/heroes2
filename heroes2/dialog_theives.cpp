#include "view.h"

using namespace draw;

const int textx = 185;
const int startx = 210;
const int maxw = 430;

static void paint_heroes(int x, int y, int width, playeri** players, int count, heroi** source) {
	if(!count)
		return;
	draw::state push;
	font = SMALFONT;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++) {
		auto px = x + chunk / 2 + i * chunk;
		image(px + 4 - 20, y + 5, source[i]->getid(), false);
		image(px - 20, y + 1, LOCATORS, 22, AFNoOffset);
		auto p = source[i]->getname();
		text(px - textw(p) / 2, y - draw::texth() - 2, p);
	}
}

static void paint_monsters(int x, int y, int width, playeri** players, int count, monster_s* source) {
	if(!count)
		return;
	draw::state push;
	font = SMALFONT;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++) {
		int id = source[i];
		int frame = (id - FirstMonster) * 9;
		auto px = x + chunk / 2 + i * chunk;
		image(px, y + 12, MINIMON, frame);
		image(px, y + 12, MINIMON, frame + 1);
		auto p = getstr(source[i]);
		text(px - draw::textw(p) / 2, y + 15, p);
	}
}

static void paint_heroes_stat(int x, int y, int width, playeri** players, int count, heroi** source) {
	static const char* attr_names[] = {"Ат", "Зщ", "Сл", "Зн"};
	if(!count)
		return;
	draw::state push;
	font = SMALFONT;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++) {
		char temp[64];
		auto hero = source[i];
		auto px = x + chunk / 2 + i * chunk;
		for(auto j = Attack; j <= Knowledge; j = (ability_s)(j + 1)) {
			zprint(temp, "%1: %2i", attr_names[j - Attack], hero->get(j));
			text(px - textw(temp) / 2, y + (j - Attack)*texth() - 2, temp);
		}
	}
}

static void paint_title(int x, int y, int width, int count) {
	static const char* text_header[6] = {"1-ое", "2-ое", "3-ье", "4-ое", "5-ое", "6-ое"};
	if(!count)
		return;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++) {
		auto px = x + chunk / 2 + i * chunk;
		auto p = text_header[i];
		draw::text(px - draw::textw(p) / 2, y, p);
	}
}

static void paint_header(int x, int y, int width, playeri** players, int count) {
	if(!count)
		return;
	const int chunk = width / count;
	for(auto i = 0; i < count; i++) {
		auto px = x + chunk / 2 + i * chunk;
		auto p = players[i]->getname();
		draw::text(px - draw::textw(p) / 2, 270, p);
	}
}

static void paint_flags(int x, int y, int width, playeri** players, int count, int* source) {
	int places[7] = {0};
	const auto chunk = width / count;
	for(int i = 0; i < count; ++i) {
		playeri* players_source[7] = {};
		for(int j = 0; j < count; j++) {
			if(places[j] == i)
				zcat(players_source, players[j]);
		}
		int players_count = zlen(players_source);
		if(!players_count)
			continue;
		auto sw = getwidth(FLAG32, 1);
		auto px = x + chunk / 2 + i * chunk - (players_count * sw) / 2;
		for(int j = 0; j < players_count; j++) {
			auto frame = players_source[j]->getid() * 2 + 1;
			draw::image(px, y, FLAG32, frame, AFNoOffset);
			px += sw;
		}
	}
}

static void paint_summary(int x, int y, const char* text, playeri** players, int count, int* source) {
	draw::text(x, y, text);
	paint_flags(x + startx, y, maxw, players, count, source);
}

void playeri::thieves() {
	// Select active players
	playeri* players[LastPlayer - FirstPlayer + 1];
	int	count = 0;
	for(auto i = FirstPlayer; i <= LastPlayer; i = (player_s)(i + 1)) {
		if(!bsmeta<playeri>::elements[i])
			continue;
		players[count++] = bsmeta<playeri>::elements + i;
	}
	if(!count)
		return;
	// Select cities and towns
	int cities[LastPlayer - FirstPlayer + 1] = {};
	int towns[LastPlayer - FirstPlayer + 1] = {};
	int heroes[LastPlayer - FirstPlayer + 1] = {};
	monster_s monsters[LastPlayer - FirstPlayer + 1] = {};
	for(int i = 0; i < count; i++)
		cities[i] = players[i]->getcastles();
	for(int i = 0; i < count; i++)
		towns[i] = players[i]->gettowns();
	for(int i = 0; i < count; i++)
		heroes[i] = players[i]->getheroes();
	//for(int i = 0; i < count; i++)
	//	monsters[i] = game::getbestmonster(players[i]);
	// Constants
	while(ismodal()) {
		image(0, 0, STONEBAK, 0);
		image(0, 461, WELLXTRA, 2);
		status({21, draw::height - 16, 21 + getwidth(SMALLBAR, 0), draw::height - 1});
		int x = 4;
		// Часть 1
		paint_title(x + startx, 5, maxw, count);
		paint_summary(x, 28, "Количество городов:", players, count, towns);
		paint_summary(x, 54, "Количество замков:", players, count, cities);
		paint_summary(x, 80, "Количество героев:", players, count, heroes);
		text(x, 106, "Золота в казне:");
		text(x, 132, "Дерево и руда:");
		text(x, 158, "Другие ресурсы:");
		text(x, 184, "Найдено обелисков:");
		text(x, 210, "Общая сила армии:");
		text(x, 238, "Доход:");
		// Часть 2
		paint_header(x + startx, 270, maxw, players, count);
		text(x, 306, "Лучший герой:");
		//paint_heroes(x + startx, 306, maxw, players, count, heroes);
		text(x, 347, "Атрибуты лучшего героя:");
		//paint_heroes_stat(x + startx, 347, maxw, players, count, heroes);
		text(x, 388, "Характер:");
		text(x, 429, "Лучший монстер:");
		paint_monsters(x + startx, 429, maxw, players, count, monsters);
		button(578, 461, WELLXTRA, buttoncancel, {0, 0, 1}, KeyEscape);
		cursor(ADVMCO, 0);
		domodal();
	}
}