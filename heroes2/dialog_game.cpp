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
		draw::image(x + width - 18, y + 1, REQUESTS, 30 + data[index]->wins);
		draw::image(x + width - 18 * 2, y + 1, REQUESTS, 36 + data[index]->lose);
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
		image(x + 288 + 18, y + 265, REQUESTS, 36 + e.lose);
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
	scenario_list.release();
	return true;
}

static void choose_game() {
	auto p = (gamei*)hot::param;
	auto e = *p;
	if(e.choose()) {
		*p = e;
	}
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
		cursor(ADVMCO, 0);
		domodal();
	}
	return getresult() != 0;
}

void gamei::newgame() {
	auto n = new_game();
	if(!n)
		return;
	if(n == 1) {
		gamei game;
		game.load("maps/beltway.mp2");
		game.setupmap();
	}
}