#include "view.h"

using namespace draw;

static heroi*		current_hero;
static playeri*		current_player;
const unsigned		delay_information = 8;
const int			map_sx = 14;
const int			map_sy = 14;
static unsigned		info_stamp;
static char			info_text[512];
static rect			rcmap = {16, 16, 16 + 32 * 14, 16 + 32 * 14};
static unsigned		show_information = delay_information;
static unsigned		show_sunrise = delay_information;

static void information_hero() {
	auto hero = (heroi*)hot::param;
	hero->show(true);
}

static void choose_hero() {
	current_hero = (heroi*)hot::param;
	if(!current_hero)
		return;
	map::setcamera(current_hero->getpos());
}

static struct castlec : public list {

	castlei*		data[128];

	void setup(const playeri* player) {
		maximum = 0;
		for(auto i = 0; i < 128; i++) {
			auto& e = bsmeta<castlei>::elements[i];
			if(!e)
				continue;
			if(e.getplayer() != player)
				continue;
			data[maximum++] = &e;
		}
	}

	void row(int x, int y, int index) const override {
		if(index < maximum && data[index]) {
			auto p = data[index];
			auto index_sprite = 1;
			auto iscastle = p->is(Castle);
			auto icn = isevil(LOCATORE, LOCATORS);
			switch(p->getkind()) {
			case Knight: index_sprite = iscastle ? 9 : 15; break;
			case Barbarian: index_sprite = iscastle ? 10 : 16; break;
			case Sorcerer: index_sprite = iscastle ? 11 : 17; break;
			case Warlock: index_sprite = iscastle ? 12 : 18; break;
			case Wizard: index_sprite = iscastle ? 13 : 19; break;
			case Necromancer: index_sprite = iscastle ? 14 : 20; break;
			}
			image(x - 1, y, icn, index_sprite);
			//if(bsget(rec, AlreadyMoved))
			//	draw::image(x - 1, y + 1, icn, 24);
		} else
			image(x - 1, y, isevil(LOCATORE, LOCATORS), 5 + index);
	}

} castles;

static struct heroc : public list {

	heroi* data[LastHero - FirstHero + 1];

	void setup(const playeri* player) {
		maximum = 0;
		for(auto i = FirstHero; i <= LastHero; i = hero_s(i + 1)) {
			auto& e = bsmeta<heroi>::elements[i];
			if(e.getplayer() != player)
				continue;
			data[maximum++] = &e;
		}
	}

	void row(int x, int y, int index) const override {
		if(index < maximum && data[index]) {
			auto p = data[index];
			image(x + 4, y + 5, PORTXTRA, 0);
			image(x - 1, y, MINIPORT, p->getportrait());
			image(x + 4, y + 5, MOBILITY, imin(1000 / 100, 25));
			image(x + 43, y + 5, MANA, imin(20 / 5, 25));
			if(current_hero == p)
				rectb({x - 1, y, x + 54, y + 31}, 214);
			if(mousein({x, y, x + 54, y + 31})) {
				if(hot::key==MouseLeft && hot::pressed)
					draw::execute(choose_hero, (int)data[index]);
				else if(hot::key == MouseLeftDBL && hot::pressed)
					draw::execute(information_hero, (int)data[index]);
			}
		} else
			image(x - 1, y, isevil(LOCATORE, LOCATORS), 1 + index);
	}

} heroes;

static void correct_camera() {
	if(map::camera.x < 0)
		map::camera.x = 0;
	if(map::camera.y < 0)
		map::camera.y = 0;
	if(map::camera.y >= (map::height - map_sy - 1) * 32)
		map::camera.y = (map::height - map_sy - 1) * 32;
	if(map::camera.x >= (map::width - map_sx - 1) * 32)
		map::camera.x = (map::width - map_sy - 1) * 32;
}

static void move_camera() {
	switch(hot::param) {
	case KeyLeft: map::camera.x -= 32; break;
	case KeyRight: map::camera.x += 32; break;
	case KeyUp: map::camera.y -= 32; break;
	case KeyDown: map::camera.y += 32; break;
	}
	correct_camera();
}

static void paint_tiles(rect screen, point camera) {
	draw::state push;
	draw::clipping = screen;
	for(int y = screen.y1; y < screen.y2; y += 32) {
		for(int x = screen.x1; x < screen.x2; x += 32) {
			int index = map::m2i((x + camera.x) / 32, (y + camera.y) / 32);
			imagt(x, y, TisGROUND32, map::tiles[index], map::flags[index] & 0x03);
		}
	}
	switch(hot::key) {
	case KeyLeft: execute(move_camera, hot::key); break;
	case KeyRight: execute(move_camera, hot::key); break;
	case KeyUp: execute(move_camera, hot::key); break;
	case KeyDown: execute(move_camera, hot::key); break;
	}
}

static void minimap(int x, int y, int mode) {
	if(!mode)
		image(x, y, isevil(HEROLOGE, HEROLOGO), 0, 0);
	else {
	}
}

static void paint_blank_info(int x, int y, const playeri* player) {
	image(x, y, isevil(HEROLOGE, HEROLOGO), 0, 0);
}

static int paint_buttons(int x, int y, const playeri* player) {
	auto icn = isevil(ADVEBTNS, ADVBTNS);
	draw::button(x + 0 * 36, y + 0 * 36, icn, buttonok, {0, 0, 1}, Alpha + 'H', "��������� �����");
	draw::button(x + 1 * 36, y + 0 * 36, icn, buttonok, {2, 2, 3}, Alpha + 'M', "���������� ��������");
	draw::button(x + 2 * 36, y + 0 * 36, icn, buttonok, {4, 4, 5});
	draw::button(x + 3 * 36, y + 0 * 36, icn, buttonok, {6, 6, 7});
	draw::button(x + 0 * 36, y + 1 * 36, icn, buttonok, {8, 8, 9}, Alpha + 'E', "��������� ���");
	draw::button(x + 1 * 36, y + 1 * 36, icn, buttonok, {10, 10, 11});
	draw::button(x + 2 * 36, y + 1 * 36, icn, buttonok, {12, 12, 13});
	draw::button(x + 3 * 36, y + 1 * 36, icn, buttonok, {14, 14, 15});
	return 2 * 36;
}

static void paint_calendar(int x, int y) {
	auto d = map::getweekday();
	if(d == 0)
		image(x, y, isevil(SUNMOONE, SUNMOON), 4 - (map::getweek() % 4));
	else
		image(x, y, isevil(SUNMOONE, SUNMOON), 0);
	char temp[64];
	if(true) {
		state push;
		font = SMALFONT;
		zprint(temp, "%1: %3i, %2: %4i", "�����", "������", map::getmonth() + 1, map::getweek() + 1);
		textm(x, y + 34, 140, AlignCenter, temp);
	}
	zprint(temp, "%1: %2i", "����", map::getweekday() + 1);
	textm(x, y + 50, 140, AlignCenter, temp);
}

static void change_mode() {
	show_sunrise = 0;
	show_information = 0;
}

static void paint_information(int x, int y, const playeri* player) {
	y += paint_buttons(x, y, player);
	auto mouse_in_info = mousein({x, y, x + 142, y + 4 * 36});
	if(mouse_in_info) {
		if(hot::key == MouseLeft && hot::pressed)
			execute(change_mode);
	}
	image(x, y, isevil(STONBAKE, STONBACK), 0);
	if(show_sunrise) {
		paint_calendar(x, y);
		if(hot::key == InputTimer)
			show_sunrise--;
	} else if(show_information) {
		state push;
		font = SMALFONT;
		if(info_text[0])
			textf(x + 2, y + 8, 142 - 4, info_text);
		if(hot::key == InputTimer)
			show_information--;
	} else if(true) {
		state push;
		font = SMALFONT;
		image(x, y, RESSMALL, 0);
		text(x + 26, y + 31, "1"); // castle
		text(x + 78, y + 31, "0"); // town
		//information_resource(x + 122, y + 31, Gold, player);
		//information_resource(x + 14, y + 61, Wood, player);
		//information_resource(x + 35, y + 61, Mercury, player);
		//information_resource(x + 59, y + 61, Ore, player);
		//information_resource(x + 82, y + 61, Sulfur, player);
		//information_resource(x + 106, y + 61, Crystal, player);
		//information_resource(x + 128, y + 61, Gems, player);
	}
}

void map::setcamera(short unsigned index) {
	camera.x = i2x(index) * 32 - (32 * map_sx / 2);
	camera.y = i2y(index) * 32 - (32 * map_sy / 2);
	correct_camera();
}

//static void paint_test(int x, int y) {
//	// Show text information
//	switch(information_mode) {
//	case Resource:
//		if(true) {
//		}
//		break;
//	case Hero:
//		//army(x + 4, y + 20, 132, world::hero->garmy()->units, 5, false);
//		break;
//	case Information:
//		draw::image(x, y, draw::isevil(res::SUNMOONE, res::SUNMOON), 3 - ((game::getweek() - 1) % 4) + 1);
//		if(true) {
//			draw::state push;
//			draw::font = res::SMALFONT;
//			szprint(temp, "%1: %3i, %2: %4i",
//				szt("Month", "�����"), szt("Week", "������"),
//				game::getmonth(), game::getweek());
//			draw::textm(x, y + 34, 140, draw::Center, temp);
//		}
//		szprint(temp, "%1: %2i",
//			szt("Day", "����"),
//			game::getday());
//		draw::textm(x, y + 50, 140, draw::Center, temp);
//		break;
//	}
//}

static void paint_screen(const playeri* player) {
	// After some time clear text info
	if(info_stamp < clock())
		info_text[0] = 0;
	image(0, 0, isevil(ADVBORDE, ADVBORD), 0, 0);
	minimap(480, 16, 0);
	heroes.draw(481, 176, 56, 32);
	castles.draw(553, 176, 56, 32);
	paint_information(480, 320, player);
	paint_tiles(rcmap, map::camera);
	//paint_objects(drawables, rcmap, map::camera);
	//paint_route(rcmap, map::camera);
}

static void setup_collections(const playeri* player) {
	castles.row_per_screen = 4;
	castles.setup(player);
	heroes.row_per_screen = 4;
	heroes.setup(player);
}

void playeri::adventure() {
	setup_collections(this);
	while(ismodal()) {
		paint_screen(this);
		cursor(ADVMCO, 0);
		domodal();
	}
}