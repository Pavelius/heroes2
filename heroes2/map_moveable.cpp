#include "view.h"

using namespace draw;

DECLBASE(moveablei, 8192);

static resource_s	frame2resource[] = {Ore, Sulfur, Crystal, Gems, Gold};

static unsigned char getroad(unsigned char object, unsigned char index) {
	switch(getres(object)) {
		// from sprite road
	case ROAD:
		switch(index) {
		case 0: case 4: case 5:case 13: case 26:
			return SH(Up) | SH(Down);
		case 2: case 21: case 28:
			return SH(Left) | SH(Right);
		case 17: case 29:
			return SH(LeftUp) | SH(RightDown);
		case 18: case 30:
			return SH(RightUp) | SH(LeftDown);
		case 3:
			return SH(Up) | SH(Down) | SH(Left) | SH(Right);
		case 6:
			return SH(Up) | SH(Down) | SH(Right);
		case 7:
			return SH(Up) | SH(Right);
		case 9:
			return SH(Down) | SH(Right);
		case 12:
			return SH(Down) | SH(Left);
		case 14:
			return SH(Up) | SH(Down) | SH(Left);
		case 16:
			return SH(Up) | SH(Left);
		case 19:
			return SH(LeftUp) | SH(RightDown);
		case 20:
			return SH(RightUp) | SH(LeftDown);
		default:
			return 0;
		}
		// castle and tower (gate)
	case OBJNTOWN:
		if(13 == index ||
			29 == index ||
			45 == index ||
			61 == index ||
			77 == index ||
			93 == index ||
			109 == index ||
			125 == index ||
			141 == index ||
			157 == index ||
			173 == index ||
			189 == index)
			return SH(Up) | SH(Down);
		return 0;
		// castle lands (gate)
	case OBJNTWBA:
		if(7 == index ||
			17 == index ||
			27 == index ||
			37 == index ||
			47 == index ||
			57 == index ||
			67 == index ||
			77 == index)
			return SH(Up) | SH(Down);
		return 0;
	default:
		return 0;
	}
}

moveablei* add_moveable(short unsigned index, variant v, short unsigned quantity) {
	auto p = bsmeta<moveablei>::add();
	switch(v.type) {
	case Resource:
		switch(v.resource) {
		case Ore: case Wood: quantity = xrand(5, 10); break;
		case Gold: quantity = 100 * xrand(3, 9); break;
		default: quantity = xrand(3, 6); break;
		}
		break;
	case Monster:
		switch(bsmeta<monsteri>::elements[v.monster].level) {
		case 2: quantity = xrand(8, 12); break;
		case 3: quantity = xrand(4, 7); break;
		case 4: quantity = xrand(1, 3); break;
		default: quantity = xrand(12, 30); break;
		}
		break;
	}
	p->element = v;
	p->index = index;
	p->value = quantity;
	p->player = RandomPlayer;
	p->value2 = 0;
	return p;
}

moveablei* add_object(unsigned short index, unsigned char object, unsigned char frame, unsigned char quantity) {
	static moveablei* last_object;
	const drawobji* pi = 0;
	auto icn = getres(object);
	switch(icn) {
	case OBJNTOWN: // Не будем добавлять города
	case OBJNTWBA: // Не будем добавлять базу городов
	case OBJNTWRD: // Не будем добалять случайные города
		map::roads[index] = getroad(object, frame);
		return 0;
	case MINIHERO: // No heroes
	case SHADOW32: // No heroes shadows
	case FLAG32: // No player flags
	case OBJNTWSH: // No towns and castles shadow
	case OBJNARTI: // No artifacts tiles
	case MONS32: // No monster images
	case OBJNRSRC: // No resource images
		return 0;
	case OBJNMUL2:
		if(frame == 163) // No event signal (used only in editor)
			return 0;
		pi = drawobji::find(icn, frame);
		break;
	case EXTRAOVR:
		if(last_object) {
			// Abandone mine and Mountain Mines has overlay just after their objects
			if(last_object->element.object == Mines || last_object->element.object == AbandoneMine)
				last_object->value2 = maptbl(frame2resource, frame);
		}
		return last_object;
	case STREAM:
		return add_moveable(index, Stream, frame);
	case ROAD:
		map::roads[index] = getroad(object, frame);
		return add_moveable(index, Road, frame);
	default:
		pi = drawobji::find(icn, frame);
		//assert(pi);
		break;
	}
	if(pi) {
		// Skip all frame not zero frames
		if((pi->first + pi->shape.zero) != frame)
			return 0;
	}
	// В конце добави новый подвижный объект
	if(pi) {
		last_object = add_moveable(index, pi->object, pi - bsmeta<drawobji>::elements);
		return last_object;
	}
	return 0;
}

void draw::imags(int x, int y, unsigned short value, unsigned short index) {
	auto& e = bsmeta<drawobji>::elements[value];
	auto& sh = e.shape;
	for(int i = 0; i < sh.count; i++) {
		auto px = x + sh.points[i].x * 32;
		auto py = y + sh.points[i].y * 32;
		auto frame = e.first + sh.indecies[i];
		if(sh.animation[i])
			image(px, py, e.res, frame + 1 + ((draw::counter + index * index) % sh.animation[i]));
		image(px, py, e.res, frame);
	}
}

bool gamei::isresource(unsigned char object) {
	return getres(object) == OBJNRSRC;
}

static void paint_hero(int x, int y, kind_s type, direction_s direction, bool shipmaster) {
	auto icn = NoRes;
	auto index = 0;
	if(shipmaster)
		icn = BOAT32;
	else switch(type) {
	case Knight: icn = KNGT32; break;
	case Barbarian: icn = BARB32; break;
	case Sorcerer: icn = SORC32; break;
	case Warlock: icn = WRLK32; break;
	case Necromancer: icn = NECR32; break;
	default: icn = WZRD32; break;
	}
	unsigned flags = 0;
	switch(direction) {
	case Up:          index = 0; break;
	case RightUp:     index = 9; break;
	case Right:       index = 18; break;
	case RightDown:   index = 27; break;
	case Down:        index = 36; break;
	case LeftDown:    index = 27; flags |= AFMirror; break;
	case Left:        index = 18; flags |= AFMirror; break;
	case LeftUp:      index = 9; flags |= AFMirror; break;
	default: break;
	}
	if(flags&AFMirror)
		image(x + 32, y, icn, index, flags);
	else
		image(x, y, icn, index, flags);
}

static void paint_flag(int x, int y, player_s player, direction_s direction, int tick, bool still) {
	auto icn = NoRes;
	auto index_sprite = 0;
	switch(player) {
	case PlayerBlue: icn = B_FLAG32; break;
	case PlayerGreen: icn = G_FLAG32; break;
	case PlayerRed: icn = R_FLAG32; break;
	case PlayerYellow: icn = Y_FLAG32; break;
	case PlayerOrange: icn = O_FLAG32; break;
	case PlayerPurple: icn = P_FLAG32; break;
	default: return;
	}
	unsigned flags = 0;
	switch(direction) {
	case Up:        index_sprite = 0; break;
	case RightUp:   index_sprite = 9; break;
	case Right:     index_sprite = 18; break;
	case RightDown: index_sprite = 27; break;
	case Down:      index_sprite = 36; break;
	case LeftDown:  index_sprite = 27; flags |= AFMirror; x += 8; break;
	case Left:      index_sprite = 18; flags |= AFMirror; x += 12; break;
	case LeftUp:    index_sprite = 9; flags |= AFMirror; x += 19; break;
	default: break;
	}
	if(still)
		index_sprite += 56;
	int id = index_sprite + (tick % ((index_sprite >= 45) ? 8 : 9));
	if(flags&AFMirror)
		image(x + 16 /*+ res::ox(icn, id) * 2*/, y, icn, id, flags);
	else
		image(x, y, icn, id, flags);
}

static void paint_shad(int x, int y, direction_s direction, int index) {
	auto icn = SHADOW32;
	auto index_sprite = 0;
	switch(direction) {
	case Up:          index_sprite = 0; break;
	case RightUp:     index_sprite = 9; break;
	case Right:       index_sprite = 18; break;
	case RightDown:   index_sprite = 27; break;
	case Down:        index_sprite = 36; break;
	case LeftDown:    index_sprite = 45; break;
	case Left:        index_sprite = 54; break;
	case LeftUp:      index_sprite = 63; break;
	default: break;
	}
	image(x, y, icn, index_sprite + (index % 9));
}

void drawable::paint_castle(int x, int y, landscape_s tile, kind_s race, bool castle, bool shadow) {
	int index;
	switch(tile) {
	case Grass: index = 0; break;
	case Snow: index = 10; break;
	case Swamp: index = 20; break;
	case Lava: index = 30; break;
	case Desert: index = 40; break;
	case Dirt: index = 50; break;
	case Waste: index = 60; break;
	default: index = 70; break;
	}
	// Paint background
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 3 * 32, OBJNTWBA, index + ii);
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 4 * 32, OBJNTWBA, index + 5 + ii);
	// draw castle
	switch(race) {
	case Knight: index = 0; break;
	case Barbarian: index = 32; break;
	case Sorcerer: index = 64; break;
	case Warlock: index = 96; break;
	case Necromancer: index = 160; break;
	default: index = 128; break;
	}
	if(castle)
		index += 16;
	if(shadow) {
		for(int iy = 0; iy < 4; iy++) {
			for(int ix = -2; ix <= 1; ix++) {
				int x1 = x + ix * 32;
				int y1 = y + iy * 32;
				if(iy == 3)
					x1 += 32;
				draw::image(x1, y1, OBJNTWSH, index + iy * 4 + (ix + 2));
			}
		}
	}
	// Town
	image(x + 2 * 32, y, OBJNTOWN, index);
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 1 * 32, OBJNTOWN, index + 1 + ii);
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 2 * 32, OBJNTOWN, index + 6 + ii);
	for(int ii = 0; ii < 5; ++ii)
		image(x + ii * 32, y + 3 * 32, OBJNTOWN, index + 11 + ii);
}

static int getmode(monster_s m) {
	switch(m) {
	case Zombie:
	case MutantZombie:
		return 1;
	default:
		return 0;
	}
}

void drawable::paint() const {
	static unsigned char decode_resource[] = {12, 0, 2, 4, 6, 8, 10, 16};
	static unsigned char decode_extraovr[] = {4, 0, 0, 0, 1, 2, 3, 0};
	int i;
	switch(type) {
	case Moveable:
		switch(moveable->element.type) {
		case Monster:
			image(x + 16, y + 30, moveable->element.monster, moveable->index, 0, getmode(moveable->element.monster));
			break;
		case Resource:
			i = decode_resource[moveable->element.resource];
			image(x - 32, y, OBJNRSRC, i);
			image(x, y, OBJNRSRC, i + 1);
			break;
		case Artifact:
			i = moveable->element.artifact * 2;
			image(x - 32, y, OBJNARTI, i);
			image(x, y, OBJNARTI, i + 1);
			break;
		case MapObject:
			switch(moveable->element.object) {
			case TreasureChest:
				image(x - 32, y, OBJNRSRC, 18);
				image(x, y, OBJNRSRC, 19);
				break;
			case AncientLamp:
				image(x - 32, y, OBJNRSRC, 14);
				image(x, y, OBJNRSRC, 15);
				break;
			case Road:
				image(x, y, ROAD, moveable->value);
				break;
			case Stream:
				image(x, y, STREAM, moveable->value);
				break;
			case Mines:
				imags(x, y, moveable->value, moveable->index);
				if(moveable->player != RandomPlayer)
					image(x + 6, y - 26, FLAG32, moveable->player);
				image(x, y, EXTRAOVR, decode_extraovr[moveable->value2]);
				break;
			case SawMill:
				imags(x, y, moveable->value, moveable->index);
				if(moveable->player != RandomPlayer)
					image(x + 12, y - 48, FLAG32, moveable->player);
				break;
			default:
				imags(x, y, moveable->value, moveable->index);
				break;
			}
			break;
		}
		break;
	case Hero:
		paint_hero(x, y, hero->getkind(), hero->getdirection(), false);
		paint_flag(x, y, hero->getplayer()->getid(), hero->getdirection(), draw::counter, true);
		paint_shad(x, y, hero->getdirection(), 0);
		break;
	case CastleVar:
		paint_castle(x - 32 * 2, y - 32 * 3,
			map::gettile(castle->getpos()),
			castle->getkind(), !castle->is(Castle), true);
		break;
	default:
		break;
	}
}

void drawable::getrect(rect& rc) const {
	rc.x1 = x;
	rc.y1 = y;
	if(type == Moveable && moveable->element.type == MapObject) {
		auto& sh = bsmeta<drawobji>::elements[moveable->value].shape;
		rc.x1 += sh.offset.x * 32;
		rc.y1 += sh.offset.y * 32;
		rc.x2 += rc.x1 + sh.size.x * 32;
		rc.y2 += rc.y1 + sh.size.y * 32;
	} else if(type == CastleVar) {
		rc.x1 -= 32 * 3;
		rc.y1 -= 32 * 3;
		rc.x2 = rc.x1 + 6 * 32;
		rc.y2 = rc.y1 + 6 * 32;
	} else {
		rc.x2 = rc.x1 + 32;
		rc.y2 = rc.y1 + 32;
	}
}

void drawable::border() const {
	rectb({x, y, x + 32, y + 32}, 0x10);
}

int	drawable::getlevel() const {
	if(type == Moveable && moveable->element.type == MapObject) {
		switch(moveable->element.object) {
		case Road: return 3;
		case Lake: case Cliff: case Hole: return 2;
		case Stream: case StreamDelta: return 1;
		default: return 10;
		}
	}
	return 10;
}

int drawable::getzpos() const {
	auto v = y;
	if(type == Moveable && moveable->element.type == MapObject) {
		switch(moveable->element.object) {
		case TreeKnowledge:
			v += 33;
			break;
		}
	}
	return v;
}