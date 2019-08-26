#include "stringbuilder.h"
#include "view.h"

using namespace draw;

static castlei* current_castle;

// Barbarian, Knight, Necromancer, Sorcerer, Warlock, Wizard,
static rect buildings_rect[20][6] = {{{0, 88, 0 + 202, 88 + 62},// Castle
{0, 55, 0 + 290, 55 + 85},
{322, 63, 322 + 73, 63 + 97},
{0, 67, 0 + 198, 67 + 100},
{268, 35, 268 + 131, 35 + 129},
{0, 48, 0 + 187, 48 + 39}},
{{478, 100, 478 + 76, 100 + 42},// ThievesGuild
{0, 130, 0 + 50, 130 + 60},
{291, 134, 291 + 43, 134 + 59},
{423, 165, 423 + 65, 165 + 49},
{525, 109, 525 + 60, 109 + 48},
{507, 55, 507 + 47, 55 + 42}},
{{0, 205, 0 + 125, 205 + 60},// Tavern
{350, 110, 350 + 46, 110 + 56},
{0, 0, 0 + 0, 0 + 0},
{494, 140, 494 + 131, 140 + 87},
{479, 100, 479 + 39, 100 + 52},
{0, 160, 0 + 118, 160 + 50}},
{{535, 210, 535 + 105, 210 + 45},// Shipyard
{537, 221, 537 + 103, 221 + 33},
{516, 221, 516 + 124, 221 + 28},
{0, 220, 0 + 134, 220 + 35},
{520, 206, 520 + 120, 206 + 47},
{0, 218, 0 + 185, 218 + 35}},
{{272, 215, 272 + 44, 215 + 32}, // Well
{194, 225, 194 + 29, 225 + 27},
{217, 225, 217 + 23, 225 + 26},
{346, 209, 346 + 43, 209 + 25},
{348, 221, 348 + 63, 221 + 30},
{254, 143, 254 + 19, 143 + 28}},
{{470, 180, 470 + 30, 180 + 58},// Statue
{480, 205, 480 + 45, 205 + 50},
{374, 174, 374 + 26, 174 + 70},
{158, 173, 158 + 17, 173 + 58},
{473, 172, 473 + 45, 172 + 51},
{464, 58, 464 + 26, 58 + 62}},
{{0, 50, 60, 160},// Left turret
{10, 35, 60, 145},
{330, 50, 350, 110},
{95, 100, 125, 170},
{310, 85, 333, 160},
{35, 20, 50, 85}},
{{120, 45, 165, 145},// Right turret
{135, 40, 180, 140},
{360, 65, 380, 110},
{145, 100, 175, 165},
{358, 85, 380, 160},
{130, 20, 150, 85}},
{{113, 155, 113 + 106, 155 + 30}, // Moat
{53, 150, 53 + 93, 150 + 30},
{336, 169, 336 + 98, 169 + 11},
{143, 169, 143 + 98, 169 + 11},
{327, 166, 327 + 66, 166 + 17},
{0, 91, 0 + 198, 91 + 11}},
{{224, 168, 224 + 52, 168 + 36}, // Marketplace
{220, 144, 220 + 115, 144 + 20},
{415, 216, 415 + 85, 216 + 35},
{412, 122, 412 + 56, 122 + 40},
{391, 185, 391 + 70, 185 + 26},
{254, 176, 254 + 105, 176 + 39}},
{{210, 104, 210 + 40, 104 + 35}, // Captain
{293, 109, 293 + 48, 109 + 27},
{423, 126, 423 + 41, 126 + 46},
{238, 136, 238 + 32, 136 + 34},
{420, 102, 420 + 52, 102 + 60},
{210, 55, 210 + 28, 55 + 32}},
{{252, 120, 252 + 44, 120 + 16},// Well2
{288, 97, 288 + 63, 97 + 18},
{275, 206, 275 + 68, 206 + 39},
{135, 200, 135 + 63, 200 + 31},
{69, 46, 69 + 24, 46 + 124},
{234, 225, 234 + 107, 225 + 22}},
{{223, 79, 223 + 124, 79 + 27},// Specail Building
{0, 80, 0 + 250, 80 + 20},
{0, 0, 0 + 640, 0 + 54},
{147, 0, 147 + 252, 147 + 30},
{0, 162, 0 + 70, 162 + 77},
{304, 111, 304 + 95, 111 + 50}},
{{258, 142, 258 + 71, 142 + 41},// Dwelve 1
{195, 175, 195 + 50, 175 + 40},
{404, 181, 404 + 56, 181 + 25},
{478, 70, 478 + 92, 70 + 62},
{0, 63, 0 + 68, 63 + 53},
{459, 181, 459 + 45, 181 + 32}},
{{152, 190, 152 + 68, 190 + 50},// Dwelve 2
{145, 155, 145 + 58, 155 + 20},
{147, 184, 147 + 92, 184 + 32},
{345, 149, 345 + 70, 149 + 56},
{248, 192, 248 + 60, 192 + 55},
{253, 69, 253 + 90, 69 + 29}},
{{582, 81, 582 + 58, 81 + 40},// Dwelve 3
{250, 177, 250 + 70, 177 + 50},
{108, 69, 108 + 117, 69 + 91},
{90, 180, 90 + 56, 180 + 21},
{504, 53, 504 + 38, 53 + 30},
{156, 139, 156 + 74, 139 + 51}},
{{509, 148, 509 + 123, 148 + 57},// Dwelve 4
{328, 195, 328 + 100, 195 + 50},
{0, 154, 0 + 140, 182 + 74},
{208, 192, 208 + 127, 192 + 55},
{154, 168, 154 + 171, 168 + 76},
{593, 187, 593 + 47, 187 + 28}},
{{331, 186, 331 + 121, 186 + 44},// Dwelve 5
{0, 200, 0 + 150, 200 + 55},
{235, 136, 235 + 53, 136 + 70},
{345, 231, 345 + 165, 231 + 21},
{149, 98, 149 + 95, 98 + 58},
{417, 25, 417 + 35, 25 + 140}},
{{407, 13, 407 + 109, 13 + 80},// Dwelve 6
{465, 85, 465 + 175, 85 + 110},
{468, 112, 468 + 92, 112 + 78},
{202, 38, 202 + 42, 38 + 65},
{98, 25, 98 + 55, 25 + 229},
{196, 7, 196 + 129, 7 + 38}},
{{348, 20, 348 + 50, 20 + 124},// Mage guild
{398, 55, 398 + 58, 55 + 125},
{570, 45, 570 + 61, 45 + 162},
{285, 32, 285 + 55, 32 + 129},
{590, 14, 590 + 50, 14 + 155},
{585, 0, 585 + 57, 0 + 122}},
};

static building_s buildings_priority[6][20] = {{SpecialBuilding, Well2, Dwelving6, MageGuild, Captain, Tent, LeftTurret, RightTurret, Moat,
Dwelving3, ThievesGuild, Tavern, Dwelving1, MarketPlace,
Dwelving2, Dwelving4,
Dwelving5,
Well, Statue, Shipyard},
// Knight
{Tent, SpecialBuilding, Well2, Captain, LeftTurret, RightTurret, Moat,
MarketPlace, Dwelving2, ThievesGuild, Tavern, MageGuild,
Dwelving5, Dwelving6,
Dwelving1, Dwelving3, Dwelving4,
Well, Statue, Shipyard},
// Necromancer
{SpecialBuilding, Tavern, Tent, Captain, LeftTurret, RightTurret, Dwelving6, Moat,
Dwelving1, ThievesGuild, Dwelving3, Dwelving5, Dwelving2,
Dwelving4, MageGuild, Shipyard, Well2, MarketPlace,
Statue, Well},
// Sorcerer
{SpecialBuilding, Dwelving6, MageGuild, Captain, Tent, LeftTurret, RightTurret, Moat,
Dwelving3, Shipyard, MarketPlace, Dwelving2,
ThievesGuild, Dwelving1, Tavern, Statue, Well2, Dwelving4, Well, Dwelving5},
// Warlock
{Dwelving5, Dwelving3, Tent, LeftTurret, RightTurret, Captain, Moat,
Shipyard, MageGuild, Tavern, ThievesGuild, MarketPlace, Statue,
Dwelving1, Well2, SpecialBuilding,
Dwelving4, Dwelving2, Dwelving6, Well},
// Wizard
{Dwelving6, Tent, LeftTurret, RightTurret, Moat, Captain,
Dwelving2, ThievesGuild, Tavern, Shipyard, Well, SpecialBuilding,
Dwelving3, Dwelving5, MageGuild, Statue,
Dwelving1, Dwelving4, MarketPlace, Well2},
};

static res_s buildings_resource[6][32] = {{TWNBCSTL, TWNBTHIE, TWNBTVRN, TWNBDOCK, TWNBWELL, TWNBSTAT,
TWNBLTUR, TWNBRTUR, TWNBMOAT,
TWNBMARK, TWNBCAPT, TWNBWEL2, TWNBSPEC,
TWNBDW_0, TWNBDW_1, TWNBDW_2, TWNBDW_3, TWNBDW_4, TWNBDW_5,
TWNBMAGE, TWNBTENT,
NoRes, TWNBUP_1, NoRes, TWNBUP_3, TWNBUP_4, NoRes, NoRes,
TWNBMAGE, TWNBMAGE, TWNBMAGE, TWNBMAGE},
// Knight
{TWNKCSTL, TWNKTHIE, TWNKTVRN, TWNKDOCK, TWNKWELL, TWNKSTAT,
TWNKLTUR, TWNKRTUR, TWNKMOAT,
TWNKMARK, TWNKCAPT, TWNKWEL2, TWNKSPEC,
TWNKDW_0, TWNKDW_1, TWNKDW_2, TWNKDW_3, TWNKDW_4, TWNKDW_5,
TWNKMAGE, TWNKTENT,
NoRes, TWNKUP_1, TWNKUP_2, TWNKUP_3, TWNKUP_4, TWNKUP_5, NoRes,
TWNKMAGE, TWNKMAGE, TWNKMAGE, TWNKMAGE},
// Necromancer
{TWNNCSTL, TWNNTHIE, NoRes, TWNNDOCK, TWNNWELL, TWNNSTAT,
TWNNLTUR, TWNNRTUR, TWNNMOAT,
TWNNMARK, TWNNCAPT, TWNNWEL2, TWNNSPEC,
TWNNDW_0, TWNNDW_1, TWNNDW_2, TWNNDW_3, TWNNDW_4, TWNNDW_5,
TWNNMAGE, TWNNTENT,
NoRes, TWNNUP_1, TWNNUP_2, TWNNUP_3, TWNNUP_4, NoRes, NoRes,
TWNNMAGE, TWNNMAGE, TWNNMAGE, TWNNMAGE},
// Sorcerer,
{TWNSCSTL, TWNSTHIE, TWNSTVRN, TWNSDOCK, TWNSWELL, TWNSSTAT,
TWNSLTUR, TWNSRTUR, TWNSMOAT,
TWNSMARK, TWNSCAPT, TWNSWEL2, TWNSSPEC,
TWNSDW_0, TWNSDW_1, TWNSDW_2, TWNSDW_3, TWNSDW_4, TWNSDW_5,
TWNSMAGE, TWNSTENT,
NoRes, TWNSUP_1, TWNSUP_2, TWNSUP_3, NoRes, NoRes, NoRes,
TWNSMAGE, TWNSMAGE, TWNSMAGE, TWNSMAGE},
// Warlock
{TWNWCSTL, TWNWTHIE, TWNWTVRN, TWNWDOCK, TWNWWELL, TWNWSTAT,
TWNWLTUR, TWNWRTUR, TWNWMOAT,
TWNWMARK, TWNWCAPT, TWNWWEL2, TWNWSPEC,
TWNWDW_0, TWNWDW_1, TWNWDW_2, TWNWDW_3, TWNWDW_4, TWNWDW_5,
TWNWMAGE, TWNWTENT,
NoRes, NoRes, NoRes, TWNWUP_3, NoRes, TWNWUP_5, TWNWUP5B,
TWNWMAGE, TWNWMAGE, TWNWMAGE, TWNWMAGE},
// Wizard
{TWNZCSTL, TWNZTHIE, TWNZTVRN, TWNZDOCK, TWNZWELL, TWNZSTAT,
TWNZLTUR, TWNZRTUR, TWNZMOAT,
TWNZMARK, TWNZCAPT, TWNZWEL2, TWNZSPEC,
TWNZDW_0, TWNZDW_1, TWNZDW_2, TWNZDW_3, TWNZDW_4, TWNZDW_5,
TWNZMAGE, TWNZTENT,
NoRes, NoRes, TWNZUP_2, NoRes, TWNZUP_4, TWNZUP_5, NoRes,
TWNZMAGE, TWNZMAGE, TWNZMAGE, TWNZMAGE},
};
static unsigned char buildings_indecies[NoBuilding + 1] = {
	6, 1, 2, 3, 4, 7,
	8, 9, 12,
	10, 15, 11, 13,
	19, 20, 21, 22, 23, 24,
	0, 5,
	0, 25, 26, 27, 28, 29, 30,
	0, 0, 0, 0,
	0 // End symbol No Building
};

static void hire_monster() {
	hot::key = 0;
	current_castle->recruit((building_s)hot::param);
}

static void begin_trade() {
	hot::key = 0;
	((playeri*)hot::param)->marketplace();
}

static void show_tavern() {
	hot::key = 0;
	((playeri*)hot::param)->tavern();
}

static void thieves_guild() {
	hot::key = 0;
	((playeri*)hot::param)->thieves();
}

static void castle_build() {
	hot::key = 0;
	((castlei*)hot::param)->build();
}

static void show_well() {
	hot::key = 0;
	((castlei*)hot::param)->well();
}

unsigned char castlei::getframe(building_s v) {
	return buildings_indecies[v];
}

res_s draw::getbuildings(kind_s v) {
	switch(v) {
	case Barbarian: return CSTLBARB;
	case Knight: return CSTLKNGT;
	case Necromancer: return CSTLNECR;
	case Sorcerer: return CSTLSORC;
	case Warlock: return CSTLWRLK;
	case Wizard: return CSTLWZRD;
	default: return NoRes;
	}
}

static void buildings_information() {
	auto kind = current_castle->getkind();
	auto building = (building_s)hot::param;
	auto p = castlei::getdescription(building, kind);
	if(p)
		message(p, kind, building, 0, 0, ButtonOK);
}

static void buildings_tips() {
	hot::key = 0;
	auto kind = current_castle->getkind();
	auto building = (building_s)hot::param;
	current_castle->information(building, kind);
}

void castlei::information(building_s v, kind_s k) {
	auto p = getdescription(v, k);
	if(!p)
		return;
	message(p, k, v, 0, 0, NoButtons);
}

void castlei::paint_panorama(int x, int y) const {
	bool coastal = true;
	switch(kind) {
	case Knight:
		image(x, y, TOWNBKG0, 0, 0);
		break;
	case Barbarian:
		image(x, y, TOWNBKG1, 0, 0);
		image(x, y, TWNBEXT1, 1 + draw::counter % 5, 0);
		break;
	case Sorcerer:
		image(x, y, TOWNBKG2, 0, 0);
		break;
	case Warlock:
		image(x, y, TOWNBKG3, 0, 0);
		break;
	case Necromancer:
		image(x, y, TOWNBKG5, 0, 0);
		break;
	default:
		image(x, y, TOWNBKG4, 0, 0);
		break;
	}
	if(coastal) {
		switch(kind) {
		case Knight:
			image(x, y, TWNKEXT0, 0);
			image(x, y, TWNKEXT0, 1 + counter % 5);
			break;
		case Barbarian:
			image(x, y, TWNBEXT0, 0);
			image(x, y, TWNBEXT0, 1 + counter % 5);
			break;
		case Sorcerer:
			draw::image(x, y, TWNSEXT0, 0);
			draw::image(x, y, TWNSEXT0, 1 + counter % 5);
			break;
		case Necromancer:
			image(x, y, TWNNEXT0, 0);
			image(x, y, TWNNEXT0, 1 + counter % 5);
			break;
		case Warlock:
			image(x, y, TWNWEXT0, 0);
			image(x, y, TWNWEXT0, 1 + counter % 5);
			break;
		default:
			image(x, y, TWNZEXT0, 0);
			image(x, y, TWNZEXT0, 1 + counter % 5);
			break;
		}
	}
	for(auto raw_building : buildings_priority[kind]) {
		auto building = getupgraded(raw_building);
		if(building == NoBuilding)
			continue;
		auto id = buildings_resource[kind][building];
		if(id == NoRes)
			continue;
		int index = 0;
		if(raw_building == MageGuild) {
			if(kind == Necromancer) {
				switch(building) {
				case MageGuild2: index = 6; break;
				case MageGuild3: index = 12; break;
				case MageGuild4: index = 18; break;
				case MageGuild5: index = 24; break;
				}
			} else {
				switch(building) {
				case MageGuild2: index = 1; break;
				case MageGuild3: index = 2; break;
				case MageGuild4: index = 3; break;
				case MageGuild5: index = 4; break;
				}
			}
		}
		image(x, y, id, index, 0);
		auto rbd = raw_building;
		if(rbd == Tent && is(Castle))
			rbd = Castle;
		if(hot::mouse.in(buildings_rect[rbd][kind])) {
			//rectb(buildings_rect[rbd][kind], 0x10);
			char temp[260]; stringbuilder sb(temp);
			sb.add(getstr(building, kind));
			auto monster = getmonster(building, getkind());
			if(monster != RandomMonster)
				sb.adds("(нанять %1)", bsmeta<monsteri>::elements[monster].multiname);
			status(sb);
			if(hot::key == MouseLeft && hot::pressed) {
				if(monster != RandomMonster) {
					current_castle = (castlei*)this;
					execute(hire_monster, building);
				} else if(building == MarketPlace)
					execute(begin_trade, (int)getplayer());
				else if(building == Well)
					execute(show_well, (int)this);
				else if(building == Tavern)
					execute(show_tavern, (int)getplayer());
				else if(building == ThievesGuild)
					execute(thieves_guild, (int)getplayer());
				else if(building == Castle)
					execute(castle_build, (int)this);
				else {
					current_castle = const_cast<castlei*>(this);
					execute(buildings_information, building);
				}
				hot::key = MouseLeft;
			} else if(hot::key == MouseRight && hot::pressed) {
				current_castle = const_cast<castlei*>(this);
				execute(buildings_tips, building);
				hot::key = MouseRight;
			}
		}
		index = animation::getanimate(id, index, draw::counter, false);
		if(index)
			draw::image(x, y, id, index);
	}
}

void castlei::paint_panel(int x, int y, const heroi* hero) const {
	image(x, y, STRIP, 0);
	image(x + 5, y + 6, CREST, 3);
	armyi::paint(x + 112, y + 6, 0);
	if(hero) {
		image(x + 5 + 50, y + 105, hero->getid(), true);
		armyi::paint(x + 112, y + 105, hero);
	} else {
		//if(bsget(mid, Captain))
		//	draw::clipart(x + 5 + 50, y + 105, bsget(mid, Type) - Barbarian + BarbarianCaptain, bsget(mid, Player));
		//else
		image(x + 5, y + 105, STRIP, 3);
		image(x + 112, y + 105, STRIP, 11);
	}
}

void castlei::paint_name() const {
	state push;
	font = SMALFONT;
	image(0, 0, TOWNNAME, 0);
	text(231 + 179 / 2 - textw(name) / 2, 248 + 2, name);
}

static void previous_town() {}
static void next_town() {}

void castlei::paint(const heroi* hero) const {
	status({21, height - 16, 21 + getwidth(SMALLBAR, 0), height - 1});
	button(0, height - 19, SMALLBAR, previous_town, {1, 1, 2}, KeyLeft, "Предыдущий город");
	image(21, height - 19, SMALLBAR, 0);
	button(width - 21, height - 19, SMALLBAR, next_town, {3, 3, 4}, KeyRight, "Следующий город");
	paint_panorama(0, 0);
	paint_panel(0, 256, hero);
	paint_name();
	auto player = getplayer();
	if(player)
		player->getresources().paint(552, 262);
	button(553, 428, SWAPBTN, buttoncancel, {0, 0, 1}, KeyEscape, "Покинуть город");
}

building_s castlei::getupgrade(building_s v, kind_s k) {
	if(v == NoBuilding)
		return NoBuilding;
	auto v1 = getupgrade(v);
	if(v1 == NoBuilding)
		return NoBuilding;
	if(buildings_resource[k][v1] == NoRes)
		return NoBuilding;
	return v1;
}

void castlei::show() {
	heroi* hero = 0;
	while(ismodal()) {
		paint(hero);
		cursor(ADVMCO, 0);
		domodal();
	}
}
