#include "stringbuilder.h"
#include "view.h"

using namespace draw;

enum infotype_s : unsigned char {
	DayInfo, KindomInfo, ObjectInfo,
	LastInfo = ObjectInfo,
};

static pvar				hilite_var, current_var;
static short unsigned	hilite_index, current_index;
const unsigned			delay_information = 8;
const int				map_sx = 14;
const int				map_sy = 14;
static char				info_text[512];
static costi			info_cost;
static rect				rcmap = {16, 16, 16 + 32 * 14, 16 + 32 * 14};
static unsigned			show_message = delay_information;
static unsigned			show_sunrise = delay_information;
static infotype_s		info_type = ObjectInfo;
static drawable			drawables[2048];
static unsigned			drawables_count;

static unsigned select_drawables(const rect& rcmap, point camera, drawable* source, unsigned count) {
	rect rc = {rcmap.x1 - 32, rcmap.y1 - 32, rcmap.x2 + 64, rcmap.y2 + 64};
	auto p = source;
	auto pe = p + count;
	for(unsigned i = 0; i < bsmeta<moveablei>::count; i++) {
		auto& e = bsmeta<moveablei>::elements[i];
		*(static_cast<pvar*>(p)) = &e;
		p->x = map::i2x(e.index) * 32 - camera.x + 16;
		p->y = map::i2y(e.index) * 32 - camera.y + 16;
		if(!p->in(rc))
			continue;
		if(p < pe)
			p++;
	}
	for(unsigned i = 0; i < bsmeta<castlei>::count; i++) {
		auto& e = bsmeta<castlei>::elements[i];
		auto index = e.getpos();
		*(static_cast<pvar*>(p)) = &e;
		p->x = map::i2x(index) * 32 - 32 * 2 - camera.x;
		p->y = map::i2y(index) * 32 - 32 * 3 - camera.y;
		if(!p->in(rc))
			continue;
		if(p < pe)
			p++;
	}
	for(auto i = FirstHero; i <= LastHero; i = (hero_s)(i + 1)) {
		auto& e = bsmeta<heroi>::elements[i];
		if(e.getpos() == Blocked)
			continue;
		*(static_cast<pvar*>(p)) = &e;
		auto index = e.getpos();
		p->x = map::i2x(index) * 32 - camera.x;
		p->y = map::i2y(index) * 32 + 30 - camera.y;
		if(!p->in(rc))
			continue;
		if(p < pe)
			p++;
	}
	return p - source;
}

static int compare_drawables(const void* p1, const void* p2) {
	auto& e1 = *((drawable*)p1);
	auto& e2 = *((drawable*)p2);
	auto v1 = e1.getlevel();
	auto v2 = e2.getlevel();
	if(v1 != v2)
		return v1 - v2;
	if(e1.y != e2.y)
		return e1.y - e2.y;
	return e1.x - e2.x;
}

static void normalize_drawables() {
	qsort(drawables, drawables_count, sizeof(drawables[0]), compare_drawables);
}

static void update_drawables() {
	drawables_count = select_drawables(rcmap, map::camera, drawables, sizeof(drawables) / sizeof(drawables[0]));
	normalize_drawables();
}

static void information_hero() {
	auto hero = (heroi*)hot::param;
	hero->show(true);
}

static void choose_hero() {
	current_var = (heroi*)hot::param;
	if(!current_var)
		return;
	info_type = ObjectInfo;
	map::setcamera(current_var.hero->getpos());
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
			if(current_var.hero == p)
				rectb({x - 1, y, x + 54, y + 31}, 214);
			if(mousein({x, y, x + 54, y + 31})) {
				if(hot::key == MouseLeft && hot::pressed)
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
	if(map::camera.y >= (map::height - map_sy) * 32)
		map::camera.y = (map::height - map_sy) * 32;
	if(map::camera.x >= (map::width - map_sx) * 32)
		map::camera.x = (map::width - map_sx) * 32;
	update_drawables();
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
	hilite_index = Blocked;
	hilite_var.clear();
	auto x2 = camera.x + screen.width();
	auto y2 = camera.y + screen.height();
	for(int y = camera.y; y < y2; y += 32) {
		for(int x = camera.x; x < x2; x += 32) {
			int index = map::m2i(x / 32, y / 32);
			auto x1 = x - camera.x + screen.x1;
			auto y1 = y - camera.y + screen.y1;
			imagt(x1, y1, TisGROUND32, map::tiles[index], map::flags[index] & 0x03);
			const rect rc = {x1, y1, x1 + 31, y1 + 31};
			if(mousein(rc)) {
				hilite_var = map::gettile(index);
				hilite_index = index;
#ifdef _DEBUG
				if(hilite_index == index)
					rectb(rc, 0x10);
#endif // _DEBUG
			}
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

static void endturn() {
	playeri::endturn();
	show_sunrise = delay_information;
}

static int paint_buttons(int x, int y, const playeri* player) {
	auto icn = isevil(ADVEBTNS, ADVBTNS);
	draw::button(x + 0 * 36, y + 0 * 36, icn, buttonok, {0, 0, 1}, Alpha + 'H', "Следующий герой");
	draw::button(x + 1 * 36, y + 0 * 36, icn, buttonok, {2, 2, 3}, Alpha + 'M', "Продолжить движение");
	draw::button(x + 2 * 36, y + 0 * 36, icn, buttonok, {4, 4, 5});
	draw::button(x + 3 * 36, y + 0 * 36, icn, buttonok, {6, 6, 7});
	draw::button(x + 0 * 36, y + 1 * 36, icn, endturn, {8, 8, 9}, Alpha + 'E', "Закончить ход");
	draw::button(x + 1 * 36, y + 1 * 36, icn, buttonok, {10, 10, 11});
	draw::button(x + 2 * 36, y + 1 * 36, icn, buttonok, {12, 12, 13});
	draw::button(x + 3 * 36, y + 1 * 36, icn, buttonok, {14, 14, 15});
	return 2 * 36;
}

static void paint_calendar(int x, int y) {
	auto d = map::getweekday();
	if(d == 0)
		image(x, y, isevil(SUNMOONE, SUNMOON), 4 - map::getmonthweek());
	else
		image(x, y, isevil(SUNMOONE, SUNMOON), 0);
	char temp[64];
	if(true) {
		state push;
		font = SMALFONT;
		zprint(temp, "%1: %3i, %2: %4i", "Месяц", "Неделя", map::getmonth() + 1, map::getmonthweek() + 1);
		textm(x, y + 34, 140, AlignCenter, temp);
	}
	zprint(temp, "%1: %2i", "День", map::getweekday() + 1);
	textm(x, y + 50, 140, AlignCenter, temp);
}

static void change_mode() {
	show_sunrise = 0;
	show_message = 0;
	info_type = infotype_s(info_type + 1);
	if(info_type > LastInfo)
		info_type = infotype_s(0);
}

static void information_resource(int x, int y, resource_s id, const playeri* player) {
	char temp[16];
	auto& cost = ((playeri*)player)->getresources();
	zprint(temp, "%1i", cost.get(id));
	text(x - draw::textw(temp) / 2, y, temp);
}

static void paint_kindom(int x, int y, const playeri* player) {
	state push;
	font = SMALFONT;
	image(x, y, RESSMALL, 0);
	text(x + 26, y + 31, "1"); // castle
	text(x + 78, y + 31, "0"); // town
	information_resource(x + 122, y + 31, Gold, player);
	information_resource(x + 14, y + 61, Wood, player);
	information_resource(x + 35, y + 61, Mercury, player);
	information_resource(x + 59, y + 61, Ore, player);
	information_resource(x + 82, y + 61, Sulfur, player);
	information_resource(x + 106, y + 61, Crystal, player);
	information_resource(x + 128, y + 61, Gems, player);
}

static void paint_army(int x, int y, const armyi& e) {
	e.paintsmall({x + 2, y + 2, x + 142, y + 70}, true, false);
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
	} else if(show_message) {
		state push;
		font = SMALFONT;
		if(info_text[0])
			textf(x + 2, y + 8, 142 - 4, info_text);
		if(hot::key == InputTimer)
			show_message--;
	} else {
		switch(info_type) {
		case ObjectInfo:
			if(current_var.type == Hero)
				paint_army(x, y, *current_var.hero);
			else
				paint_kindom(x, y, player);
			break;
		case KindomInfo:
			paint_kindom(x, y, player);
			break;
		default:
			paint_calendar(x, y);
			break;
		}
	}
}

void map::setcamera(short unsigned index) {
	camera.x = i2x(index) * 32 - (32 * map_sx / 2);
	camera.y = i2y(index) * 32 - (32 * map_sy / 2);
	correct_camera();
}

static void paint_objects(const rect& rcmap, point camera) {
	state push;
	clipping = rcmap;
	for(unsigned i = 0; i < drawables_count; i++) {
		auto& e = drawables[i];
		e.paint();
		switch(e.type) {
		case CastleVar:
			if(e.castle->getpos() == hilite_index)
				hilite_var = e;
			break;
		case Hero:
			if(e.hero->getpos() == hilite_index)
				hilite_var = e;
			break;
		case Moveable:
			if(e.moveable->index == hilite_index)
				hilite_var = e;
			break;
		}
	}
}

static void tips_info(bool show_resource_count, bool show_monster_count, bool show_artifact_name) {
	char temp[260]; stringbuilder sb(temp); temp[0] = 0;
	//sb.add("%1i (%2i, %3i)", hilite_index, map::i2x(hilite_index), map::i2y(hilite_index));
	switch(hilite_var.type) {
	case Moveable:
		switch(hilite_var.moveable->element.type) {
		case Monster:
			if(show_monster_count)
				sb.addn("%1i %2",
					hilite_var.moveable->value,
					bsmeta<monsteri>::elements[hilite_var.moveable->element.monster].multiname);
			else
				sb.addn("%1 %-2",
					armysizei::find(hilite_var.moveable->value)->name,
					bsmeta<monsteri>::elements[hilite_var.moveable->element.monster].multiname);
			break;
		case Artifact:
			if(show_artifact_name)
				sb.addn(getstr(hilite_var.moveable->element.artifact));
			else
				sb.addn("Артефакт");
			break;
		case Resource:
			if(show_resource_count)
				sb.addn("%1i %-2", hilite_var.moveable->value,
					bsmeta<resourcei>::elements[hilite_var.moveable->element.resource].nameof);
			else
				sb.addn(getstr(hilite_var.moveable->element.resource));
			break;
		case MapObject:
			switch(hilite_var.moveable->element.mapobject) {
			case TreasureChest:
				sb.addn("Сундук с сокровищами");
				break;
			case AncientLamp:
				sb.addn("Волшебная лампа");
				break;
			}
			break;
		}
		break;
	case CastleVar: sb.addn("Замок %1", hilite_var.castle->getname()); break;
	case Hero: sb.addn("Герой %1", hilite_var.hero->getname()); break;
	case Landscape: sb.addn(getstr(hilite_var.landscape)); break;
	}
	quicktips(hot::mouse.x, hot::mouse.y, temp);
}

static void tips_info() {
	tips_info(true, false, true);
}

static void paint_screen(const playeri* player) {
	image(0, 0, isevil(ADVBORDE, ADVBORD), 0, 0);
	minimap(480, 16, 0);
	heroes.draw(481, 176, 56, 32);
	castles.draw(553, 176, 56, 32);
	paint_information(480, 320, player);
	paint_tiles(rcmap, map::camera);
	paint_objects(rcmap, map::camera);
	//paint_route(rcmap, map::camera);
	if(mousein(rcmap)) {
		if(hot::key == MouseRight && hot::pressed)
			execute(tips_info);
	}
}

static void update_lists(const playeri* player) {
	castles.row_per_screen = 4;
	castles.setup(player);
	heroes.row_per_screen = 4;
	heroes.setup(player);
}

void playeri::quickmessage(const costi& cost, const char* format, ...) {
	stringbuilder sb(info_text);
	sb.addv(format, xva_start(format));
	info_cost = cost;
	show_message = delay_information;
}

void playeri::adventure() {
	update_drawables();
	update_lists(this);
	quickmessage({}, "Ход начал %1 игрок", getname());
	while(ismodal()) {
		paint_screen(this);
		cursor(ADVMCO, 0);
		domodal();
	}
}