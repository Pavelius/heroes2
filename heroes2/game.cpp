#include "io.h"
#include "main.h"

#pragma pack(push)
#pragma pack(1)

#define mp2obj(i) (i-FirstObject+0x80)

namespace mp2 {
struct header {
	unsigned		id;				// file identification
	short int		level;			// difficult level
	unsigned char	width;
	unsigned char	height;
	unsigned char	player[6];
	unsigned char	humans[6];
	unsigned char	computers[6];
	unsigned char	unknown1[3];
	unsigned char	conditions_wins;
	unsigned char	comp_also_wins;
	unsigned char	allow_normal_victory;
	unsigned short	wins;
	unsigned char	conditions_loss;
	unsigned short	loss;
	unsigned char   hero;
	unsigned char   races[6];
	unsigned short	wins_data;
	unsigned short	loss_data;
	unsigned char   unknown2[10];
	char			name[16];
	unsigned char	unknown4[44];
	char			text[143];
};
struct addon {
	unsigned short	indexAddon;		// zero or next addons_t
	unsigned char	objectNameN1;	// level 1.N
	unsigned char	indexNameN1;	// level 1.N or 0xFF
	unsigned char	quantityN;
	unsigned char	objectNameN2;	// id of object (use map::object)
	unsigned char	indexNameN2;	// index if file from 'Name'
	unsigned		uniqNumberN1;	// level 1.N
	unsigned		uniqNumberN2;	// level 2.N
};
struct tile {
	unsigned short	tileIndex;		// tile (ocean, grass, snow, swamp, lava, desert, dirt, wasteland, beach)
	unsigned char	objectName1;	// level 1.0 (resource file name id)
	unsigned char	indexName1;		// index level 1.0 or 0xFF (index in resource file)
	unsigned char	quantity1;		// count
	unsigned char	quantity2;		// count
	unsigned char	objectName2;	// level 2.0
	unsigned char	indexName2;		// index level 2.0 or 0xFF
	unsigned char	shape;			// shape reflect % 4, 0 none, 1 vertical, 2 horizontal, 3 any
	unsigned char	generalObject;	// zero or object from mp2obj(generalObject)
	unsigned short	indexAddon;		// zero or index mp2addon
	unsigned		uniqNumber1;	// level 1.0
	unsigned		uniqNumber2;	// level 2.0
};
struct army {
	unsigned char	monster[5];
	unsigned short	count[5];
};
struct castle {
	unsigned char	player; 			// 00 blue, 01 green, 02 red, 03 yellow, 04 orange, 05 purpl, ff unknown
	unsigned char	has_buildings;
	unsigned short	buildings;
	unsigned short	dwellings;
	unsigned char	magic_tower;
	unsigned char	has_army;
	struct army		army;
	unsigned char	has_captain;
	unsigned char	has_name;
	char			name[13];		// name + '\0'
	unsigned char	race;			// 00 knight, 01 barb, 02 sorc, 03 warl, 04 wiz, 05 necr, 06 rnd
	unsigned char	castle_in_town;
	unsigned char	disable_castle_upgrade;	// 00 TRUE, 01 FALSE
	unsigned char	unknown[29];
};
// 0x004c - size
struct hero {
	unsigned char	unknown1;
	unsigned char	has_army;
	struct army		army;
	unsigned char	has_type;
	unsigned char	type;
	unsigned char	artifact[3]; // 0xff none
	unsigned char	unknown2;	// 0
	unsigned		exerience;
	unsigned char	has_skills;
	unsigned char	skill[8]; // 0xff none
	unsigned char	skill_level[8];
	unsigned char	unknown3;	// 0
	unsigned char	has_name;
	char			name[13];	// name + '\0'
	unsigned char	has_patrol;
	unsigned char	patrol;	// for patrol in squards
	unsigned char	unknown4[15];	// 0
};
// origin mp2 sign or buttle
struct info {
	unsigned char	id;		// 0x01
	unsigned char	zero[8];// 8 byte 0x00
	char			text;	// message  + '/0'
};
// origin mp2 event for coord
struct eventcoord {
	unsigned char	id;		// 0x01
	unsigned		wood;
	unsigned		mercury;
	unsigned		ore;
	unsigned		sulfur;
	unsigned		crystal;
	unsigned		gems;
	unsigned		golds;
	unsigned short	artifact;	// 0xffff - none
	unsigned char	computer;	// allow events for computer
	unsigned char	cancel;		// cancel event after first visit
	unsigned char	zero[10];	// 10 byte 0x00
	unsigned char	blue;
	unsigned char	green;
	unsigned char	red;
	unsigned char	yellow;
	unsigned char	orange;
	unsigned char	purple;
	char			text[1];		// message + '/0'
};
// origin mp2 event for day
struct eventday {
	unsigned char	id;		// 0x00
	unsigned		wood;
	unsigned		mercury;
	unsigned		ore;
	unsigned		sulfur;
	unsigned		crystal;
	unsigned		gems;
	unsigned		golds;
	unsigned short	artifact;	// always 0xffff - none
	unsigned short	computer;	// allow events for computer
	unsigned short	first;		// day of first occurent
	unsigned short	subsequent;	// subsequent occurrences
	unsigned char	zero[6];	// 6 byte 0x00 and end 0x01
	unsigned char	blue;
	unsigned char	green;
	unsigned char	red;
	unsigned char	yellow;
	unsigned char	orange;
	unsigned char	purple;
	char			text;		// message + '/0'
};
// origin mp2 rumor
struct rumor {
	unsigned char	id;		// 0x00
	unsigned char	zero[7];	// 7 byte 0x00
	char			text[1];   	// message + '/0'
};
// origin mp2 riddle sphinx
struct riddle {
	unsigned char	id;		// 0x00
	unsigned		wood;
	unsigned		mercury;
	unsigned		ore;
	unsigned		sulfur;
	unsigned		crystal;
	unsigned		gems;
	unsigned		golds;
	unsigned short	artifact;	// 0xffff - none
	unsigned char	count;		// count answers (1, 8)
	char			answer1[13];
	char			answer2[13];
	char			answer3[13];
	char			answer4[13];
	char			answer5[13];
	char			answer6[13];
	char			answer7[13];
	char			answer8[13];
	char			text[1];		// message + '/0'
};
}
#pragma pack(pop)

inline short unsigned mp2i(short unsigned i) {
	return map::m2i(i%map::width, i / map::width);
}

static unsigned short getLE16(unsigned short u) {
	return (u << 8) | (u >> 8);
}

static kind_s index2race(int index) {
	switch(index) {
	case 0: return Knight;
	case 1: return Barbarian;
	case 2: return Sorcerer;
	case 3: return Warlock;
	case 4: return Wizard;
	case 5: return Necromancer;
	default: return RandomKind;
	}
}

static player_s index2player(int index) {
	switch(index) {
	case 0: return PlayerBlue;
	case 1: return PlayerGreen;
	case 2: return PlayerRed;
	case 3: return PlayerYellow;
	case 4: return PlayerOrange;
	case 5: return PlayerPurple;
	default: return RandomPlayer;
	}
}

static kind_s oppose_type(kind_s type) {
	switch(type) {
	case Knight:
	case Barbarian:
		return kind_s(xrand(Necromancer, Wizard));
	default:
		return kind_s(xrand(Barbarian, Wizard));
	}
}

static player_s mini2player(int index) {
	if(index < 7) return PlayerBlue;
	else if(index < 14) return PlayerGreen;
	else if(index < 21) return PlayerRed;
	else if(index < 28) return PlayerYellow;
	else if(index < 35) return PlayerOrange;
	else return PlayerPurple;
}

static kind_s mini2type(int index) {
	return kind_s(index % (Wizard+2));
}

bool gamei::load(const char* url) {
	clear();
	io::file st(url);
	if(!st)
		return false;
	mp2::header header; // (36,36), (72,72), (108,108), (144,144)
	st.read(&header, sizeof(header));
	if(header.id != 0x5C)
		return false;
	// difficult
	level = level_s(LevelEasy + header.level);
	// size
	switch(header.width) {
	case 36: size = SmallSize; break;
	case 72: size = MediumSize; break;
	case 108: size = LargeSize; break;
	case 144: size = XLargeSize; break;
	default: return false;
	}
	// start player
	start_hero = !header.hero;
	// text info
	szfnamewe(file, url);
	zcpy(description, header.text, sizeof(description) - 1);
	zcpy(name, header.name, sizeof(name) - 1);
	for(int i = 0; i < 6; i++) {
		if(!header.player[i])
			continue;
		if(header.humans[i])
			types[i] = Human;
		else if(header.computers[i])
			types[i] = ComputerOnly;
		races[i] = index2race(header.races[i]);
	}
	loss = loss_s(header.conditions_loss);
	wins = header.conditions_wins;
	return true;
}

void gamei::clear() {
	memset(this, 0, sizeof(*this));
	difficult = NormalDifficulty;
	level = LevelNormal;
}

int gamei::getplayers() const {
	auto r = 0;
	for(auto e : types) {
		if(e != NotAllowed)
			r++;
	}
	return r;
}

static unsigned get32(io::file& st) {
	unsigned r = 0;
	st.read(&r, sizeof(r));
	return r;
}

static unsigned short get16(io::file& st) {
	unsigned short r = 0;
	st.read(&r, sizeof(r));
	return r;
}

static int get(io::file& st) {
	unsigned char r = 0;
	st.read(&r, sizeof(r));
	return r;
}

void add_object(unsigned short index, unsigned char object, unsigned char frame, unsigned char quantity) {
}

static void load_object(armyi& e, mp2::army& r) {
	for(int i = 0; i < 5; i++) {
		if(!r.count[i])
			continue;
		e.units[i].unit = monster_s(r.monster[i] + FirstMonster);
		e.units[i].count = getLE16(r.count[i]);
	}
}

static void load_object(castlei& e, mp2::castle& r) {
	auto pla = index2player(r.player);
	e.setname("Порталис");
	e.set(bsmeta<playeri>::elements + pla);
	e.set(index2race(r.race));
	// custom building
	if(r.has_buildings) {
		// building
		auto build = getLE16(r.buildings);
		if(0x0002 & build) e.set(ThievesGuild);
		if(0x0004 & build) e.set(Tavern);
		if(0x0008 & build) e.set(Shipyard);
		if(0x0010 & build) e.set(Well);
		if(0x0080 & build) e.set(Statue);
		if(0x0100 & build) e.set(LeftTurret);
		if(0x0200 & build) e.set(RightTurret);
		if(0x0400 & build) e.set(MarketPlace);
		if(0x1000 & build) e.set(Moat);
		if(0x0800 & build) e.set(Well2);
		if(0x2000 & build) e.set(SpecialBuilding);
		// dwelling
		auto dwell = getLE16(r.dwellings);
		if(0x0008 & dwell) e.set(Dwelving1);
		if(0x0010 & dwell) e.set(Dwelving2);
		if(0x0020 & dwell) e.set(Dwelving3);
		if(0x0040 & dwell) e.set(Dwelving4);
		if(0x0080 & dwell) e.set(Dwelving5);
		if(0x0100 & dwell) e.set(Dwelving6);
		if(0x0200 & dwell) e.set(Dwelving2u);
		if(0x0400 & dwell) e.set(Dwelving3u);
		if(0x0800 & dwell) e.set(Dwelving4u);
		if(0x1000 & dwell) e.set(Dwelving5u);
		if(0x2000 & dwell) e.set(Dwelving6u);
		// magic tower
		if(r.magic_tower >= 1)
			e.set(MageGuild);
		if(r.magic_tower >= 2)
			e.set(MageGuild2);
		if(r.magic_tower >= 3)
			e.set(MageGuild3);
		if(r.magic_tower >= 4)
			e.set(MageGuild4);
		if(r.magic_tower >= 5)
			e.set(MageGuild5);
	} else
		e.set(Dwelving1);
	// custom troops
	if(r.has_army)
		load_object(e, r.army);
	// captain
	if(r.has_captain)
		e.set(Captain);
	// race
	if(index2race(r.race) == RandomKind) {
		//auto player = bsget(rec, Player);
		//if(!player)
		//	bsset(rec, Type, xrand(Barbarian, Wizard));
		//else
		//	bsset(rec, Type, bsget(player, Type));
	}
	// name
	if(r.has_name)
		zcpy(e.name, e.name, sizeof(e.name));
	if(!r.castle_in_town)
		e.set(Castle);
	// allow upgrade to castle (0 - true, 1 - false)
	//bsset(rec, DisableCastleUpgrade, p->disable_castle_upgrade);
}

static void load_object(heroi& e, mp2::hero& r) {
	// custom troops
	if(r.has_army)
		load_object(e, r.army);
	// custom portrate
	//if(r.has_type)
	//	bsset(rec, Portrait, p->type);
	// 3 artifacts
	for(int i = 0; i < 3; i++) {
		if(r.artifact[i] == 0xFF)
			continue;
		e.add(artifact_s(r.artifact[i] + FirstArtifact));
	}
	// experience
	//bsset(rec, Experience, p->exerience);
	// custom skill
	if(r.has_skills) {
		for(int i = 0; i < 8; i++) {
			if(r.skill_level[i])
				e.set(skill_s(FirstSkill + r.skill[i]), r.skill_level[i]);
		}
	}
	// custom name
	//bsset(rec, Name, p->name);
	// patrol
	if(r.has_patrol) {
	}
}

void gamei::updatebase() {
	for(auto i = FirstPlayer; i <= LastPlayer; i = player_s(i+1)) {
		auto& e1 = bsmeta<playeri>::elements[i];
		e1.set(this->types[i]);
		if(this->races[i]==RandomKind)
			e1.set(Knight);
		else
			e1.set(this->races[i]);
	}
}

void add_moveable(short unsigned index, variant v, short unsigned quantity = 0);

void gamei::prepare() {
	generator generate;
	static resource_s decode_resource[] = {Gold, Wood, Mercury, Ore, Sulfur, Crystal, Gems};
	char temp[260]; zprint(temp, "maps/%1.mp2", file);
	io::file st(temp);
	if(!st)
		return;
	// width and heigh
	map::clear();
	st.seek(420, SeekSet);
	map::width = get32(st);
	map::height = get32(st);
	if(!map::width || !map::height)
		return;
	updatebase();
	// tiles loading
	short unsigned tiles_count = map::height * map::width;
	mp2::tile* tiles = new mp2::tile[tiles_count];
	st.read(tiles, tiles_count * sizeof(mp2::tile));
	// addons loading
	int addon_count = get32(st);
	mp2::addon* addons = new mp2::addon[addon_count];
	st.read(addons, addon_count * sizeof(mp2::addon));
	// normalize addon
	for(int i = 0; i < addon_count; i++)
		addons[i].objectNameN1 = addons[i].objectNameN1 * 2;
	// skip coordinates
	st.seek(72 * 3 + 144 * 3, SeekCur);
	// other information
	map::obelisc_count = get(st);
	// count final mp2 blocks
	int countblock = 0;
	while(1) {
		unsigned l = get(st);
		unsigned h = get(st);
		if(0 == h && 0 == l)
			break;
		else
			countblock = 256 * h + l - 1;
	}
	for(int ii = 0; ii < countblock; ++ii) {
		unsigned char pblock[512];
		int findobject = -1;
		// read block
		unsigned sizeblock = get16(st);
		if(sizeblock > sizeof(pblock)) {
			st.seek(sizeblock, SeekCur);
			continue;
		}
		st.read(pblock, sizeblock);
		for(int id = 0; id < tiles_count; id++) {
			const mp2::tile& tile = tiles[id];
			// orders(quantity2, quantity1)
			unsigned orders = (tile.quantity2 ? tile.quantity2 : 0);
			orders <<= 8;
			orders |= tile.quantity1;
			if(orders && !(orders % 0x08) && (ii + 1 == orders / 0x08)) {
				findobject = id;
				break;
			}
		}
		if(findobject > 0) {
			const mp2::tile& tile = tiles[findobject];
			switch(tile.generalObject) {
			case mp2obj(RndTown):
			case mp2obj(RndCastle):
			case mp2obj(CastleObject):
				// add castle
				if(sizeblock == sizeof(mp2::castle)) {
					auto p = bsmeta<castlei>::add();
					load_object(*p, *((mp2::castle*)pblock));
					p->setpos(mp2i(findobject));
				}
				break;
			case mp2obj(Hero):
				// add heroes
				if(sizeblock == sizeof(mp2::hero)) {
					auto ple = mini2player(tiles[findobject].indexName1);
					auto pla = bsmeta<playeri>::elements + ple;
					auto type = mini2type(tiles[findobject].indexName1);
					if(type == RandomPlayer)
						type = pla->getkind();
					if(pblock[17] && pblock[18] <= (Bax - FirstHero)) {
						auto& e = bsmeta<heroi>::elements[pblock[18]];
						load_object(e, *((mp2::hero*)pblock));
						e.set(pla);
						e.set(Right);
						e.setpos(mp2i(findobject));
					} /*else
						game::hire(game::random::hero(type), pla, mp2i(findobject));*/
				}
				break;
			case mp2obj(Sign):
			case mp2obj(Bottle):
				// add sign or buttle
				if(sizeblock > sizeof(mp2::info) - 1 && pblock[0] == 0x01) {
					//int rec = bscreate(FirstSign);
					//bsset(rec, Index, mp2i(findobject));
					//bsset(rec, Name, (char*)&pblock[9]);
				}
				break;
			case mp2obj(Event):
				// add event maps
				if(sizeblock > sizeof(mp2::eventday) - 1 && pblock[0] == 0x01) {
					mp2::eventcoord& e = (mp2::eventcoord&)pblock;
					//int rec = bscreate(FirstEvent);
					//bsset(rec, Index, mp2i(findobject));
					//bsset(rec, Name, e.text);
					//bsset(rec, OneTime, e.cancel);
					//bsset(rec, Computer, e.computer);
					//bsset(rec, PlayerBlue, e.blue);
					//bsset(rec, PlayerGreen, e.green);
					//bsset(rec, PlayerRed, e.red);
					//bsset(rec, PlayerYellow, e.yellow);
					//bsset(rec, PlayerOrange, e.orange);
					//bsset(rec, PlayerPurple, e.purple);
					//bsset(rec, Gold, e.golds);
					//bsset(rec, Mercury, e.mercury);
					//bsset(rec, Sulfur, e.sulfur);
					//bsset(rec, Crystal, e.crystal);
					//bsset(rec, Ore, e.ore);
					//bsset(rec, Wood, e.wood);
					//bsset(rec, Gems, e.gems);
					//if(e.artifact != 0xFFFF)
					//	bsset(rec, Artifact, e.artifact);
				}
				break;
			case mp2obj(Sphinx):
				// add riddle sphinx
				if(sizeblock > sizeof(mp2::riddle) - 1 && pblock[0] == 0x00) {
					//mp2::riddle& e = (mp2::riddle&)pblock;
					//int rec = bscreate(FirstEvent);
					//bsset(rec, Index, mp2i(findobject));
					//bsset(rec, Name, e.text);
					//bsset(rec, Gold, e.golds);
					//bsset(rec, Mercury, e.mercury);
					//bsset(rec, Sulfur, e.sulfur);
					//bsset(rec, Crystal, e.crystal);
					//bsset(rec, Ore, e.ore);
					//bsset(rec, Wood, e.wood);
					//bsset(rec, Gems, e.gems);
					//if(e.artifact != 0xFFFF)
					//	bsset(rec, Artifact, e.artifact);
					//ne->answers[0] = szdup(e.answer1);
					//ne->answers[1] = szdup(e.answer2);
					//ne->answers[2] = szdup(e.answer3);
					//ne->answers[3] = szdup(e.answer4);
					//ne->answers[4] = szdup(e.answer5);
					//ne->answers[5] = szdup(e.answer6);
					//ne->answers[6] = szdup(e.answer7);
					//ne->answers[7] = szdup(e.answer8);
				}
				break;
			}
		} else if(pblock[0] == 0) {
			if(sizeblock > sizeof(mp2::eventday) - 1 && pblock[42] == 1) {
				// add event day
			} else if(sizeblock > sizeof(mp2::rumor) - 1 && pblock[8]) {
				//int mid = signs::add();
				//signs::set(mid, Index, e.id);
				//signs::set(mid, Name, e.text);
			}
		} else {
			// error
		}
	}
	// after load tiles
	for(int i = 0; i < tiles_count; i++) {
		auto i1 = mp2i(i);
		map::tiles[i1] = tiles[i].tileIndex;
		map::flags[i1] = tiles[i].shape % 4;
		for(int level = 3; level >= 0; level--) {
			// level 1.0
			if(tiles[i].indexName1 != 0xFF && (tiles[i].quantity1 % 4) == level)
				add_object(i1, tiles[i].objectName1, tiles[i].indexName1, tiles[i].quantity1);
			// level 1.N
			if(tiles[i].indexAddon) {
				auto ai = tiles[i].indexAddon;
				while(ai) {
					auto& a = addons[ai];
					if(a.objectNameN1 && a.indexNameN1 != 0xFF && (a.quantityN % 4) == level)
						add_object(i1, a.objectNameN1, a.indexNameN1, a.quantityN);
					ai = a.indexAddon;
				}
			}
			// level 2.0
			if(tiles[i].indexName2 != 0xFF && (tiles[i].quantity2 % 4) == level)
				add_object(i1, tiles[i].objectName2, tiles[i].indexName2, tiles[i].quantity2);
			// level 2.N
			if(tiles[i].indexAddon) {
				auto ai = tiles[i].indexAddon;
				while(ai) {
					auto& a = addons[ai];
					if(a.objectNameN2 && a.indexNameN2 != 0xFF && (a.quantityN % 4) == level)
						add_object(i1, a.objectNameN2, a.indexNameN2, a.quantityN);
					ai = a.indexAddon;
				}
			}
		}
	}
	// Prepare monster/artifact/resource
	for(short unsigned i = 0; i < tiles_count; i++) {
		auto i1 = mp2i(i);
		auto m = tiles[i].generalObject;
		switch(m) {
		case mp2obj(MonsterObject):
			add_moveable(i1, generate.add(monster_s(FirstMonster + tiles[i].indexName1)), 0);
			break;
		case mp2obj(RndMonster):
			add_moveable(i1, generate.monster(0), 0);
			break;
		case mp2obj(RndMonster1):
		case mp2obj(RndMonster2):
		case mp2obj(RndMonster3):
		case mp2obj(RndMonster4):
			add_moveable(i1, generate.monster(tiles[i].generalObject - mp2obj(RndMonster1) + 1), 0);
			break;
		case mp2obj(ArtifactObject):
			add_moveable(i1, generate.add(artifact_s(FirstArtifact + (tiles[i].indexName1 - 1) / 2)), 0);
			break;
		case mp2obj(RndArtifact):
			add_moveable(i1, generate.artifact(0), 0);
			break;
		case mp2obj(RndArtifact1):
		case mp2obj(RndArtifact2):
		case mp2obj(RndArtifact3):
			add_moveable(i1, generate.artifact(m - mp2obj(RndArtifact1) + 1), 0);
			break;
		case mp2obj(RndUltimateArtifact):
			add_moveable(i1, generate.artifact(4), 0);
			break;
		case mp2obj(Resource):
			if(isresource(tiles[i].objectName1)) {
				auto ms = tiles[i].indexName1 / 2;
				if(ms < sizeof(decode_resource) / sizeof(decode_resource[0]))
					add_moveable(i1, decode_resource[ms], 0);
			}
			break;
		case mp2obj(RndResource):
			add_moveable(i1, generate.resource(), 0);
			break;
		case mp2obj(TreasureChest):
			if(isresource(tiles[i].objectName1)) {
				auto quantity = tiles[i].quantity1;
				if(!quantity) {
					auto percent = d100();
					if(percent < 75) // Золото
						quantity = rand() % 4;
					else if(percent < 95)
						quantity = generate.artifact(1);
					else {
						static artifact_s bad_artifacts[] = {TaxLien, FizbinMesfortune, HideousMask};
						quantity = bad_artifacts[rand() % sizeof(bad_artifacts) / sizeof(bad_artifacts[0])];
					}
				}
				add_moveable(i1, TreasureChest, quantity);
			}
			break;
		}
	}
	// Create start heroes if need
	// Heroes must be valid race
	if(start_hero) {
		for(auto i = FirstPlayer; i <= LastPlayer; i = (player_s)(i+1)) {
			auto& e = bsmeta<playeri>::elements[i];
			if(!e)
				continue;
			auto hero = e.randomhire(0);
			hero->set(&e);
			auto castle = castlei::find(&e);
			if(castle)
				hero->setpos(castle->getpos());
		}
	}
	delete tiles;
	delete addons;
}