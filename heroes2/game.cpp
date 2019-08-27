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

enum conditions {
	Wins = 0x1000, CaptureTown = 0x1001, DefeatHero = 0x1002, FindArtifact = 0x1003, SideWins = 0x1004, AccumulateGold = 0x1005,
	CompAlsoWins = 0x0100, AllowNormalVictory = 0x0200,
	Loss = 0x2000, LoseTown = 0x2001, LoseHero = 0x2002, OutTime = 0x2003
};

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

static int mini2player(int index) {
	if(index < 7) return PlayerBlue;
	else if(index < 14) return PlayerGreen;
	else if(index < 21) return PlayerRed;
	else if(index < 28) return PlayerYellow;
	else if(index < 35) return PlayerOrange;
	else return PlayerPurple;
}

static player_s mini2type(int index) {
	auto r = player_s((index % 7) + PlayerBlue);
	if(r == PlayerPurple + 1)
		r = RandomPlayer;
	return r;
}

unsigned short getLE16(unsigned short u) {
	return (u << 8) | (u >> 8);
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
	if(header.hero)
		start_hero = true;
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