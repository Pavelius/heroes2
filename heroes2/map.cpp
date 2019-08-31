#include "main.h"

point					map::camera;
unsigned				map::day;
unsigned char			map::flags[256 * 256];
unsigned char			map::height;
unsigned char			map::obelisc_count;
unsigned short			map::tiles[256 * 256];
unsigned char			map::width;
unsigned char			map::roads[256 * 256];

void map::clear() {
	width = height = obelisc_count = 0;
	memset(tiles, 0, sizeof(tiles));
	memset(flags, 0, sizeof(flags));
}

landscape_s map::gettile(short unsigned index) {
	index = tiles[index];
	if(30 > index) // 30 indexes
		return Sea;
	else if(92 > index) // 62 indexes
		return Grass;
	else if(146 > index) // 54
		return Snow;
	else if(208 > index) // 62
		return Swamp;
	else if(262 > index) // 44
		return Lava;
	else if(321 > index)
		return Desert;
	else if(361 > index)
		return Dirt;
	else if(415 > index)
		return Waste;
	return Beach;
}