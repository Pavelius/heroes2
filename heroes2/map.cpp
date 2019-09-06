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
	if(index == Blocked)
		return Beach;
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

short unsigned map::to(short unsigned i, direction_s d) {
	if(i == Blocked)
		return Blocked;
	auto x = i2x(i), y = i2y(i);
	switch(d) {
	case Left:
		if(x == 0)
			return Blocked;
		return i - 1;
	case LeftUp:
		if(x == 0)
			return Blocked;
		if(y == 0)
			return Blocked;
		return m2i(x - 1, y - 1);
	case LeftDown:
		if(x == 0)
			return Blocked;
		if(y >= height - 1)
			return Blocked;
		return m2i(x - 1, y + 1);
	case Right:
		if(x >= width - 1)
			return Blocked;
		return i + 1;
	case RightUp:
		if(x >= width - 1)
			return Blocked;
		if(y == 0)
			return Blocked;
		return m2i(x + 1, y - 1);
	case RightDown:
		if(x >= width - 1)
			return Blocked;
		if(y >= height - 1)
			return Blocked;
		return m2i(x + 1, y + 1);
	case Up:
		if(y == 0)
			return Blocked;
		return m2i(x, y - 1);
	case Down:
		if(y >= height - 1)
			return Blocked;
		return m2i(x, y + 1);
	default:
		return i;
	}
}

direction_s map::to(direction_s f, direction_s d) {
	switch(d) {
	case Down:
		switch(f) {
		case Left: return Right;
		case Right: return Left;
		case Down: return Up;
		case LeftUp: return RightDown;
		case RightDown: return LeftUp;
		case RightUp: return LeftDown;
		case LeftDown: return RightUp;
		default: return Down;
		}
	default: return f;
	}
}

bool map::is(short unsigned index, map_flag_s v) {
	if(index == Blocked)
		return false;
	return (flags[index] & (4 << v))!=0;
}

void map::set(short unsigned index, map_flag_s v) {
	if(index == Blocked)
		return;
	flags[index] |= (4 << v);
}

pvar map::find(short unsigned index, bool rich_find) {
	if(index == Blocked)
		return pvar();
	// Heroes
	auto hero = heroi::find(index);
	if(hero)
		return hero;
	// Castles
	for(unsigned i = 0; i <= bsmeta<castlei>::count; i++) {
		auto& e = bsmeta<castlei>::elements[i];
		if(e.getpos() == index)
			return &e;
	}
	// Moveables
	moveablei* pf = 0;
	for(unsigned i = 0; i <= bsmeta<moveablei>::count; i++) {
		auto& e = bsmeta<moveablei>::elements[i];
		if(e.index == index)
			pf = &e;
		else if(rich_find) {
			switch(e.gettype()) {
			case ArtifactObject:
			case MonsterObject:
			case ResourceObject:
			case TreasureChest:
			case Road:
			case Stream:
				break;
			default:
				if(e.getshape().is(e.index, index))
					pf = &e;
				break;
			}
		}
	}
	if(pf)
		return pf;
	return gettile(index);
}