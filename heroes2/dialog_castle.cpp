#include "view.h"

using namespace draw;

static castlei* current_castle;

static void statelack(const costi& c1, const costi& c2) {
	char temp[260]; stringbuilder sb(temp); temp[0] = 0;
	for(auto i = FirstResource; i <= Gems; i = (resource_s)(i + 1)) {
		auto d = c2.data[i - FirstResource] - c1.data[i - FirstResource];
		if(d <= 0)
			continue;
		if(sb.isempthy())
			sb.add("Не хватает");
		else
			sb.add(",");
		sb.adds("%1i %2", d, getstr(i));
	}
	status(temp);
}

static void statereq(castlei& castle, cflags<building_s>& req) {
	char temp[260]; stringbuilder sb(temp);
	auto kind = castle.getkind();
	for(auto i = FirstBuilding; i <= LastBuilding; i = (building_s)(i + 1)) {
		if(!req.is(i))
			continue;
		if(!castle.is(i)) {
			if(sb.isempthy())
				sb.add("Надо построить");
			else
				sb.add(",");
			sb.adds(getstr(i, kind));
		}
	}
	status(temp);
}

static bool isallow(castlei& castle, cflags<building_s>& req) {
	auto kind = castle.getkind();
	auto errors = 0;
	for(auto i = FirstBuilding; i <= LastBuilding; i = (building_s)(i + 1)) {
		if(!req.is(i))
			continue;
		if(!castle.is(i))
			return false;
	}
	return true;
}

bool castlei::build(building_s building, bool confirm) {
	auto kind = current_castle->getkind();
	auto cost = getcost(building, kind);
	auto player = getplayer();
	if(confirm) {
		char temp[260]; zprint(temp, "Вы действительно хотите построить %1?", getstr(building, kind));
		if(message(temp, kind, building, cost, ButtonYesNo) != 1)
			return false;
	}
	auto hero = heroi::find(getpos());
	paint(hero);
	screenshoot first;
	player->getresources() -= cost;
	set(building);
	set(AlreadyMoved);
	paint(hero);
	screenshoot second;
	first.blend(second);
	return true;
}

static void build_structure() {
	if(current_castle->build((building_s)hot::param, true))
		buttonok();
}

static void information() {
	current_castle->information((building_s)hot::param,
		current_castle->getkind());
}

static void building_control(const rect& rc, building_s building, castlei* castle, bool paint_border) {
	auto kind = castle->getkind();
	auto player = castle->getplayer();
	while(castle->is(building)) {
		auto v1 = castle->getupgrade(building, kind);
		if(v1 == NoBuilding)
			break;
		building = v1;
	}
	auto& c1 = player->getresources();
	auto& c2 = castlei::getcost(building, kind);
	auto r2 = castlei::getprereqisit(building, kind);
	bool lack_resources = !(c1 >= c2);
	bool lack_requisits = !isallow(*castle, r2);
	auto already_build = castle->is(building);
	auto already_moved = castle->is(AlreadyMoved);
	auto hilite = mousein({rc.x1, rc.y1, rc.x1 + 135, rc.y1 + 80});
	const char* name = getstr(building, kind);
	if(paint_border) {
		auto bld_res = getbuildings(kind);
		auto bld_index = castlei::getframe(building);
		image(rc.x1 + 1, rc.y1 + 1, bld_res, bld_index);
	}
	if(hilite && hot::key == MouseRight && hot::pressed) {
		current_castle = castle;
		execute(information, building);
	}
	if(already_build) {
		image(rc.x2, rc.y2, TOWNWIND, 11);
		if(hilite)
			status("%1 уже построено", name);
	} else if(lack_requisits) {
		image(rc.x2, rc.y2, TOWNWIND, 12);
		if(hilite)
			statereq(*castle, r2);
	} else if(lack_resources) {
		image(rc.x2, rc.y2, TOWNWIND, 13);
		if(hilite)
			statelack(c1, c2);
	} if(already_moved) {
		image(rc.x2, rc.y2, TOWNWIND, 12);
		if(hilite)
			status("Построить можно только одно здание в день", name);
	} else if(!already_build) {
		if(hilite) {
			status("Построить %1", name);
			if(hot::key == MouseLeft && hot::pressed) {
				current_castle = castle;
				execute(build_structure, building);
			}
		}
	}
	if(paint_border) {
		if(already_build) {
			// Do nothing
		} else if(already_moved || lack_resources || lack_requisits)
			image(rc.x1, rc.y2 + 18, CASLXTRA, 2);
		else if(!already_build)
			image(rc.x1, rc.y2 + 18, CASLXTRA, 1);
		state push;
		font = SMALFONT;
		text(rc.x1 + (132 - textw(name)) / 2, rc.y2 + 21, name);
	}
}

static void building(int x, int y, building_s building, castlei* castle) {
	building_control({x, y, x + 115, y + 40}, building, castle, true);
}

static res_s captainicn(int race) {
	switch(race) {
	case Barbarian: return CSTLCAPB;
	case Knight: return CSTLCAPK;
	case Necromancer: return CSTLCAPN;
	case Sorcerer: return CSTLCAPS;
	case Warlock: return CSTLCAPW;
	case Wizard: return CSTLCAPZ;
	default: return NoRes;
	}
}

static void captain(int x, int y, castlei* castle) {
	auto race = castle->getkind();
	auto present = castle->is(Captain);
	auto already_moved = false;
	auto icn = captainicn(race);
	int sx = getwidth(icn, 0);
	int sy = getheight(icn, 0);
	if(!present) {
		image(x + 85, y - 4, CAPTCOVR, 0);
		image(x, y, icn, 0);
		building_control({x, y, x + 62, y + 58}, Captain, castle, false);
	} else {
		state push;
		font = SMALFONT;
		int x1 = x + 85;
		int y1 = y + 16;
		auto& hero = bsmeta<heroi>::elements[CaptainBarbarian + race];
		for(auto i = Attack; i <= Knowledge; i = (ability_s)(i + 1)) {
			char temp[32]; stringbuilder sb(temp);
			sb.add(getstr(i));
			sb.add(":");
			text(x1 + 80 - textw(temp), y1, temp);
			sb.clear();
			sb.add("%1i", hero.get(i));
			text(x1 + 84, y1, temp);
			y1 += texth() + 2;
		}
		image(x, y, icn, 1);
	}
}

void information_hero() {
	auto hero = (heroi*)hot::param;
	hero->show(false);
}

static void hire_hero() {
}

static void hireling(int x, int y, playeri* player, short unsigned index, heroi* hero) {
	rect rc = {x, y, x + getwidth(PORT0000, 0), y + getheight(PORT0000, 0)};
	auto hilite = mousein(rc);
	auto& c1 = player->getresources();
	auto& c2 = hero->cost;
	auto lack_resources = !(c1>=c2);
	auto already_hero = heroi::find(index) != 0;
	image(x, y, hero->getid());
	if(lack_resources) {
		image(rc.x2 - 20, rc.y2 - 20, TOWNWIND, 13);
		if(hilite)
			statelack(c1, c2);
	} else if(already_hero) {
		image(rc.x2 - 20, rc.y2 - 20, TOWNWIND, 12);
		if(hilite)
			status("Нельзя нанять потому что в замке уже есть герой");
	} else {
		if(hilite) {
			status("Нанять героя");
			if(hilite && hot::key == MouseLeft && hot::pressed)
				execute(hire_hero, (int)hero);
		}
	}
	if(hilite && hot::key == MouseRight && hot::pressed)
		execute(information_hero, (int)hero);
}

void castlei::build() {
	auto race = getkind();
	auto player = getplayer();
	auto index = 12;
	while(ismodal()) {
		bool already_moved = false;
		status({21, height - 16, 21 + getwidth(SMALLBAR, 0), height - 1});
		image(0, 0, CASLWIND, 0);
		// hide captain options
		paint(460, 5, Dirt, race, false, false);
		if(true) {
			auto p = getname();
			text(440 + (635 - 440 - textw(p)) / 2, 2, p);
		}
		//
		building(5, 2, Dwelving1, this);
		building(149, 2, Dwelving2, this);
		building(293, 2, Dwelving3, this);
		building(5, 77, Dwelving4, this);
		building(149, 77, Dwelving5, this);
		building(293, 77, Dwelving6, this);
		//
		building(5, 157, MageGuild, this);
		if(race == Necromancer)
			image(145, 157, STONBACK, 0);
		else
			building(149, 157, Tavern, this);
		building(293, 157, ThievesGuild, this);
		building(5, 232, Shipyard, this);
		building(149, 232, Statue, this);
		building(293, 232, MarketPlace, this);
		building(5, 307, Well, this);
		building(149, 307, Well2, this);
		building(293, 307, SpecialBuilding, this);
		building(5, 387, LeftTurret, this);
		building(149, 387, RightTurret, this);
		building(293, 387, Moat, this);
		captain(444, 165, this);
		if(player) {
			hireling(443, 260, player, getpos(), player->gethire(0));
			hireling(443, 362, player, getpos(), player->gethire(1));
			player->getresources().paint(552, 262);
		}
		button(553, 428, SWAPBTN, buttoncancel, {0, 0, 1}, KeyEscape, "Вернуться в город");
		image(0, 461, CASLBAR, 0);
		domodal();
	}
}