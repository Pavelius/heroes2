#include "io.h"
#include "view.h"

using namespace draw;

static void* current_value;

static void button2() {
	breakmodal(2);
}

static void button3() {
	breakmodal(3);
}

static void button4() {
	breakmodal(4);
}

static int size_index(int id) {
	switch(id) {
	case SmallSize: return 26;
	case MediumSize: return 27;
	case LargeSize: return 28;
	case XLargeSize: return 29;
	default: return 27;
	}
}

static int new_game() {
	while(ismodal()) {
		image(0, 0, HEROES, 0);
		image(405, 5, REDBACK, 0);
		button(455, 45, BTNNEWGM, buttonok, {0, 0, 1});
		button(455, 110, BTNNEWGM, button2, {2, 2, 3});
		button(455, 175, BTNNEWGM, button3, {4, 4, 5});
		button(455, 375, BTNNEWGM, buttoncancel, {6, 6, 7});
		cursor(ADVMCO, 0);
		domodal();
	}
	return getresult();
}

static struct scenarioc : list {
	gamei*			source;
	unsigned		source_count;
	const gamei*	data[512];
	size_s			filter;
	scenarioc() : source(0), source_count(0), filter(NoSize) { row_per_screen = 9; }
	void row(int x, int y, int index) const override {
		const auto width = 264;
		draw::image(x, y + 1, REQUESTS, 19 + data[index]->getplayers()); // flags
		draw::image(x + 18, y + 1, REQUESTS, size_index(data[index]->size));
		draw::image(x + width - 18 * 2, y + 1, REQUESTS, 30 + data[index]->wins);
		draw::image(x + width - 18, y + 1, REQUESTS, 36 + data[index]->loss);
		draw::textm(x + 40, y + 3, width - 36 - 36, AlignCenter, data[index]->name);
	}
	const gamei& getcurrent() {
		return *data[current];
	}
	static unsigned getfilecount(const char* url, const char* filter) {
		unsigned result = 0;
		for(io::file::find e(url); e; e.next()) {
			if(e.name()[0] == '.')
				continue;
			result++;
		}
		return result;
	}
	void set(size_s size) {
		if(!source) {
			const char* url = "maps/";
			const char* filter = "*.mp2";
			source_count = getfilecount(url, filter);
			if(!source_count)
				return;
			source = new gamei[source_count];
			source_count = 0;
			for(io::file::find e(url); e; e.next()) {
				if(e.name()[0] == '.')
					continue;
				char temp[260];
				if(source[source_count].load(e.fullname(temp)))
					source_count++;
			}
		}
		filter = size;
		auto pb = data;
		for(unsigned i = 0; i < source_count; i++) {
			if(filter && source[i].size != filter)
				continue;
			*pb++ = &source[i];
		}
		maximum = pb - data;
		correct();
		ensurevisible();
	}
	void release() {
		if(source) {
			delete[] source;
			source = 0;
			source_count = 0;
		}
	}
} scenario_list;

static buttoni check(size_s v, size_s f, unsigned char pressed) {
	auto normal = (unsigned char)(pressed - 1);
	if(v != f)
		return {normal, normal, pressed};
	return {pressed, pressed, pressed};
}

static void set_filter() {
	scenario_list.set(size_s(hot::param));
}

bool gamei::choose() {
	const int x = width - getwidth(REQSBKG, 0) - 24;
	const int y = (height - getheight(REQSBKG, 0)) / 2;
	scenario_list.set(NoSize);
	if(!scenario_list.source_count) {
		scenario_list.release();
		message("У вас нету ни одной карты. Попробуйте загрузить несколько из интернета.");
		return false;
	}
	while(ismodal()) {
		image(0, 0, HEROES, 0);
		image(x, y, REQSBKG, 0);
		scenario_list.view(x + 57, y + 55);
		auto& e = scenario_list.getcurrent();
		image(x + 85, y + 265, REQUESTS, 19 + e.getplayers()); // flags
		image(x + 103, y + 265, REQUESTS, size_index(e.size));
		textm(x + 120 + 1, y + 265 + 2, 160, AlignCenter, e.name);
		image(x + 288, y + 265, REQUESTS, 30 + e.wins);
		image(x + 288 + 18, y + 265, REQUESTS, 36 + e.loss);
		text(x + 66, y + 294, "Сложность карты:", -1);
		textm(x + 236, y + 294, 114, AlignCenter, getstr(e.level));
		textm(x + 66, y + 324, 278, AlignCenter, e.description);
		button(x + 140 + 16, y + 410, REQUESTS, buttonok, {1, 1, 2}, KeyEnter);
		// maps
		button(x + 37 + 16, y + 22, REQUESTS, cmd(set_filter, SmallSize), check(scenario_list.filter, SmallSize, 10), 0, "Отобрать только карты маленького рамера");
		button(x + 99 + 16, y + 22, REQUESTS, cmd(set_filter, MediumSize), check(scenario_list.filter, MediumSize, 12), 0, "Отобрать только карты среднего рамера");
		button(x + 161 + 16, y + 22, REQUESTS, cmd(set_filter, LargeSize), check(scenario_list.filter, LargeSize, 14), 0, "Отобрать только карты большого рамера");
		button(x + 223 + 16, y + 22, REQUESTS, cmd(set_filter, XLargeSize), check(scenario_list.filter, XLargeSize, 16), 0, "Отобрать только карты огромного рамера");
		button(x + 285 + 16, y + 22, REQUESTS, cmd(set_filter, NoSize), check(scenario_list.filter, NoSize, 18), 0, "Показать все карты");
		cursor(ADVMCO, 0);
		domodal();
	}
	if(getresult()) {
		if(scenario_list.maximum > 0)
			*this = *scenario_list.data[scenario_list.current];
	}
	scenario_list.release();
	return true;
}

static void choose_game() {
	auto p = (gamei*)hot::param;
	auto e = *p;
	if(e.choose())
		*p = e;
}

static void set_unsigned_char() {
	*((unsigned char*)current_value) = (unsigned char)hot::param;
}

static void button_difficult(int x, int y, difficult_s id, difficult_s& id_select) {
	rect rc = {x, y, x + getwidth(NGEXTRA, 62), y + getheight(NGEXTRA, 62)};
	if(id_select == id)
		draw::image(x, y, NGEXTRA, 62);
	if(hot::mouse.in(rc)) {
		if(hot::key == MouseLeft && hot::pressed) {
			current_value = &id_select;
			execute(set_unsigned_char, id);
		}
	}
	state push;
	font = SMALFONT;
	auto p = getstr(id);
	text(x + (rc.width() - textw(p)) / 2, y + rc.height() + 2, p);
}

static int xpos(int current, int count, int width) {
	return current * width * 6 / count + (width * (6 - count) / (2 * count));
}

static int race2index(int race, bool class_color) {
	switch(race) {
	case Knight: return class_color ? 51 : 70; break;
	case Barbarian: return class_color ? 52 : 71; break;
	case Sorcerer: return class_color ? 53 : 72; break;
	case Warlock: return class_color ? 54 : 73; break;
	case Wizard: return class_color ? 55 : 74; break;
	case Necromancer: return class_color ? 56 : 75; break;
	//case MultiPlayers: return 76; break;
	default: return 58;
	}
}

static void button_type(int x, int y, activity_s& type, int i, bool show_name = true) {
	int index = 0;
	switch(type) {
	case Computer: index = 3 + i; break;
	case ComputerOnly: index = 3 + 12 + i; break;
	case Human: index = 9 + i; break;
	default: break;
	}
	if(show_name)
		index += 24;
	draw::image(x, y, NGEXTRA, index, AFNoOffset);
	rect rc = {x, y, x + getwidth(NGEXTRA, 3), y + getheight(NGEXTRA, 3)};
	if(show_name) {
		state push;
		font = SMALFONT;
		int id = player_s(FirstPlayer + i);
		const char* p = bsmeta<playeri>::elements[id].getname();
		switch(id) {
		case PlayerPurple:
			p = "Фиол.";
			break;
		case PlayerOrange:
			p = "Оранж.";
			break;
		}
		text(x + (rc.width() - textw(p)) / 2, y + rc.height(), p);
	}
	if(hot::mouse.in(rc)) {
		if(hot::key == MouseLeft && hot::pressed) {
			if(type == ComputerOnly)
				return;
			current_value = &type;
			switch(type) {
			case Computer: execute(set_unsigned_char, Human); break;
			case Human: execute(set_unsigned_char, Computer); break;
			}
			
		}
	}
}

static void button_race(int x, int y, kind_s& value, int i, bool show_name, bool disabled) {
	int index = race2index(value, true);
	if(disabled)
		index += 19;
	draw::image(x, y, NGEXTRA, index, AFNoOffset);
	int w = getwidth(NGEXTRA, index);
	int h = getheight(NGEXTRA, index);
	if(show_name) {
		state push;
		font = SMALFONT;
		auto p = bsmeta<kindi>::elements[value].name_abbr;
		text(x + (w - textw(p)) / 2, y + h + 2, p);
	}
	rect rc = {x, y, x + w, y + h};
	if(hot::mouse.in(rc) && !disabled) {
		if(hot::key == MouseLeft && hot::pressed) {
			current_value = &value;
			if(value == RandomKind)
				execute(set_unsigned_char, Barbarian);
			else
				execute(set_unsigned_char, value + 1);
		}
	}
}

bool gamei::setupmap() {
	const int x = draw::width - getwidth(NGHSBKG, 0) - 8;
	const int y = (draw::height - getheight(NGHSBKG, 0)) / 2;
	const int w = getwidth(NGHSBKG, 0);
	const char* p;
	while(ismodal()) {
		image(0, 0, HEROES, 0);
		image(x, y, NGHSBKG, 0, AFNoOffset);
		// map
		p = "Сценарий";
		text(x + (w - draw::textw(p)) / 2, y + 24, p);
		button(x + 309, y + 45, NGEXTRA, cmd(choose_game, (int)this), {64, 64, 65}, F4);
		text(x + (w - draw::textw(name)) / 2, y + 48, name);
		// difficult
		p = "Сложность:";
		text(x + (w - draw::textw(p)) / 2, y + 76, p);
		button_difficult(x + 21, y + 91, EasyDifficulty, difficult);
		button_difficult(x + 98, y + 91, NormalDifficulty, difficult);
		button_difficult(x + 174, y + 91, HardDifficulty, difficult);
		button_difficult(x + 251, y + 91, VeryHardDifficulty, difficult);
		button_difficult(x + 328, y + 91, ImpossibleDifficulty, difficult);
		//
		auto maximum = getplayers();
		auto y1 = y + 200;
		auto w1 = getwidth(NGEXTRA, 3);
		auto current = 0;
		for(int i = 0; i < 6; i++) {
			if(!isallow(i))
				continue;
			auto x1 = x + (w - w1 * 6) / 2 + xpos(current, maximum, w1);
			button_type(x1, y1, types[i], i, true);
			button_race(x1, y1 + 78, races[i], i, true, false);
			current++;
		}
		// opponents
		p = "Оппоненты:";
		text(x + (w - textw(p)) / 2, y + 182, p);
		// class
		p = "Класс:";
		text(x + (w - textw(p)) / 2, y + 262, p);
		// rating
		char temp[260];
		zprint(temp, "%1 %2i%%", "Рейтинг", bsmeta<difficulti>::elements[difficult].rating);
		text(x + (w - textw(temp)) / 2, y + 383, temp);
		// buttons
		button(x + 31, y + 380, NGEXTRA, buttonok, {66, 66, 67}, KeyEnter, "Начать сценарий с выбранными настройками.");
		button(x + 287, y + 380, NGEXTRA, buttoncancel, {68, 68, 69}, KeyEscape, "Отменить текущий выбор.");
		cursor(ADVMCO, 0);
		domodal();
	}
	return getresult() != 0;
}

void gamei::newgame() {
	playeri::initialize();
	auto n = new_game();
	if(!n)
		return;
	if(n == 1) {
		gamei game;
		game.load("maps/beltway.mp2");
		//if(!game.setupmap())
		//	return;
		game.prepare();
		bsmeta<playeri>::elements[0].adventure();
	}
}