#include "main.h"

point					map::camera;
unsigned				map::day;
unsigned char			map::flags[256 * 256];
unsigned char			map::height;
unsigned char			map::obelisc_count;
unsigned short			map::tiles[256 * 256];
unsigned char			map::width;

void map::clear() {
	width = height = obelisc_count = 0;
	memset(tiles, 0, sizeof(tiles));
	memset(flags, 0, sizeof(flags));
}