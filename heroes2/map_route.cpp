#include "main.h"

static unsigned			path[256 * 256];
static unsigned short	path_stack[256 * 256];
static unsigned short	path_push;
static unsigned short	path_pop;
static unsigned short	path_goal;
static unsigned short	path_start;
static const direction_s all_around[] = {
	Left, Right, Up, Down,
	LeftUp, LeftDown, RightUp, RightDown
};

static void gnext(int index, unsigned& level, short unsigned& pos) {
	if(index == Blocked)
		return;
	auto nlevel = path[index];
	if(!nlevel)
		return;
	if(nlevel < level) {
		level = nlevel;
		pos = index;
	}
}

bool map::ispathable(short unsigned index) {
	return path[index] < BlockedPath;
}

bool map::isinteract(object_s v) {
	return v <= LastObject;
}

unsigned map::getcost(short unsigned index) {
	if(index == Blocked)
		return BlockedPath;
	return path[index];
}

unsigned map::getcost(short unsigned index, direction_s direct, unsigned pathfinding) {
	//         none   basc   advd   expr
	// Desert  2.00   1.75   1.50   1.00
	// Snow    1.75   1.50   1.25   1.00
	// Swamp   1.75   1.50   1.25   1.00
	// Cracked 1.25   1.00   1.00   1.00
	// Beach   1.25   1.00   1.00   1.00
	// Lava    1.00   1.00   1.00   1.00
	// Dirt    1.00   1.00   1.00   1.00
	// Grass   1.00   1.00   1.00   1.00
	// Water   1.00   1.00   1.00   1.00
	// Road    0.75   0.75   0.75   0.75
	//if(map::isroad(index, direct))
	//	return 75;
	unsigned result = 100;
	switch(gettile(index)) {
	case Desert:
		switch(pathfinding) {
		case 3:	break;
		case 2:	result += 50; break;
		case 1:	result += 75; break;
		default: result += 100; break;
		}
		break;
	case Snow:
	case Swamp:
		switch(pathfinding) {
		case 3:	break;
		case 2:	result += 25; break;
		case 1:	result += 50; break;
		default: result += 75; break;
		}
		break;
	case Waste:
	case Beach:
		result += (pathfinding == 0) ? 25 : 0;
		break;
	default: break;
	}
	switch(direct) {
	case RightDown:
	case RightUp:
	case LeftDown:
	case LeftUp:
		result += result * 55 / 100;
	}
	return result;
}

static unsigned getcost2(short unsigned from, direction_s d, unsigned pathfinding) {
	const auto c1 = map::getcost(from, d, pathfinding); // penalty: for [cur] out
	const auto c2 = map::getcost(map::to(from, d), map::to(d, Down), pathfinding); // penalty: for [tmp] in
	return (c1 + c2) >> 1;
}

unsigned map::getcost(short unsigned from, short unsigned to, unsigned pathfinding) {
	const auto c1 = map::getcost(from, map::getdir(from, to), pathfinding); // penalty: for [cur] out
	const auto c2 = map::getcost(to, map::getdir(to, from), pathfinding); // penalty: for [tmp] in
	return (c1 + c2) >> 1;
}

static void snode(unsigned short from, direction_s d, unsigned from_cost, int skill) {
	auto index = map::to(from, d);
	if(index == Blocked)
		return;
	auto cost = from_cost + getcost2(from, d, skill);
	if(path[index] >= BlockedPath)
		return;
	auto a = path[index];
	if(a != 0 && cost >= a)
		return;
	path_stack[path_push++] = index;
	path[index] = cost;
}

direction_s map::getdir(short unsigned from, short unsigned to) {
	static const direction_s orientations[25] = {
		LeftUp, LeftUp, Up, RightUp, RightUp,
		LeftUp, LeftUp, Up, RightUp, RightUp,
		Left, Left, Right, Right, Right,
		LeftDown, LeftDown, Down, RightDown, RightDown,
		LeftDown, LeftDown, Down, RightDown, RightDown
	};
	auto dx = i2x(to) - i2x(from);
	auto dy = i2y(to) - i2y(from);
	auto div = imax(iabs(dx), iabs(dy));
	if(!div)
		return Right; //default
	if(div > 3)
		div /= 2;
	int ax = dx / div;
	int ay = dy / div;
	return orientations[(ay + 2) * 5 + ax + 2];
}

static void clear_map_flags() {
	for(auto y = 0; y < map::height; y++) {
		for(auto x = 0; x < map::width; x++) {
			auto i = map::m2i(x, y);
			map::flags[i] &= 3;
		}
	}
}

static void update_map_flags(bool ship_master) {
	static point block_castle[] = {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {1, 0}, {2, 0}};
	if(ship_master) {
	} else {
		// Block all water part
		for(int y = 0; y < map::height; y++) {
			for(int x = 0; x < map::width; x++) {
				auto i = map::m2i(x, y);
				if(map::gettile(i) == Sea)
					map::set(i, BlockedTile);
			}
		}
		// Block all heroes parts
		for(auto i = FirstHero; i <= LastHero; i = hero_s(i + 1)) {
			auto& e = bsmeta<heroi>::elements[i];
			if(!e.isadventure())
				continue;
			auto index = e.getpos();
			map::set(index, BlockedTile);
			map::set(index, ActionTile);
		}
		// Block all known castle parts
		for(unsigned i = 0; i < bsmeta<castlei>::count; i++) {
			auto& e = bsmeta<castlei>::elements[i];
			auto index = e.getpos();
			auto x = map::i2x(index);
			auto y = map::i2y(index);
			for(auto pt : block_castle) {
				auto x1 = x + pt.x;
				auto y1 = y + pt.y;
				if(y1 < 0 || y1 >= map::height || x1 < 0 || x1 >= map::width)
					continue;
				auto i1 = map::m2i(x1, y1);
				map::set(i1, BlockedTile);
			}
			map::set(index, ActionTile);
			map::set(index, BlockedTile);
		}
		// Block overland part
		for(unsigned i = 0; i < bsmeta<moveablei>::count; i++) {
			auto& e = bsmeta<moveablei>::elements[i];
			if(!e)
				continue;
			switch(e.element.type) {
			case Monster:
				for(auto d : all_around) {
					auto i = map::to(e.index, d);
					map::set(i, BlockedTile);
					map::set(i, AttackTile);
				}
				map::set(e.index, BlockedTile);
				map::set(e.index, ActionTile);
				map::set(e.index, AttackTile);
				break;
			case Resource:
			case Artifact:
				map::set(e.index, BlockedTile);
				map::set(e.index, ActionTile);
				break;
			case Object:
				switch(e.element.object) {
				case Stream:
				case StreamDelta:
				case Road:
					break;
				case TreasureChest:
				case AncientLamp:
					map::set(e.index, BlockedTile);
					map::set(e.index, ActionTile);
					break;
				default:
					e.blockpath(path);
					break;
				}
				break;
			}
		}
	}
}

static void update_cost() {
	for(int y = 0; y < map::height; y++) {
		for(int x = 0; x < map::width; x++) {
			auto i = map::m2i(x, y);
			if(map::is(i, BlockedTile))
				path[i] = BlockedPath;
			else
				path[i] = 0;
		}
	}
}

// First, make wave and see what cell on map is passable
void map::wave(short unsigned start, int skill, int ship_master) {
	path_push = 0;
	path_pop = 0;
	if(start == Blocked)
		return;
	clear_map_flags();
	update_map_flags(ship_master);
	update_cost();
	path_stack[path_push++] = start;
	path[start] = 1;
	while(path_push != path_pop) {
		auto pos = path_stack[path_pop++];
		auto cost = path[pos];
		if(cost >= BlockedPath - 1024)
			break;
		snode(pos, Left, cost, skill);
		snode(pos, Right, cost, skill);
		snode(pos, Up, cost, skill);
		snode(pos, Down, cost, skill);
		snode(pos, LeftUp, cost, skill);
		snode(pos, LeftDown, cost, skill);
		snode(pos, RightUp, cost, skill);
		snode(pos, RightDown, cost, skill);
	}
	path_pop = 0;
	path_push = 0;
	path_goal = Blocked;
	path_start = start;
}

// Second calculate path to any cell on map use wave result
void map::route(short unsigned goal) {
	path_push = 0;
	path_goal = Blocked;
	auto pos = goal;
	unsigned level = BlockedPath;
	path_stack[path_push++] = goal;
	while(pos != path_start) {
		auto n = pos;
		gnext(to(pos, Left), level, n);
		gnext(to(pos, Right), level, n);
		gnext(to(pos, Up), level, n);
		gnext(to(pos, Down), level, n);
		gnext(to(pos, LeftDown), level, n);
		gnext(to(pos, LeftUp), level, n);
		gnext(to(pos, RightDown), level, n);
		gnext(to(pos, RightUp), level, n);
		if(pos == n)
			return;
		pos = n;
		path_stack[path_push++] = n;
		level = path[pos];
	}
	path_goal = goal;
}

short unsigned* map::getpath() {
	return path_stack;
}

int map::getpathcount() {
	return path_push;
}

void map::removestep() {
	if(path_push)
		path_push--;
}

void map::clearpath() {
	path_push = 0;
}