#include "view.h"

using namespace draw;

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

void drawable::border() const {
	rectb({x, y, x + 32, y + 32}, 0x10);
}

void drawable::paint() const {
	static unsigned char decode_resource[] = {12, 0, 2, 4, 6, 8, 10, 16};
	int i;
	switch(object.type) {
	case Moveable:
		//border();
		switch(object.moveable->element.type) {
		case Monster:
			image(x + 16, y + 30, object.moveable->element.monster, object.moveable->index);
			break;
		case Resource:
			i = decode_resource[object.moveable->element.resource];
			image(x - 32, y, OBJNRSRC, i);
			image(x, y, OBJNRSRC, i + 1);
			break;
		case Artifact:
			i = object.moveable->element.artifact * 2;
			image(x - 32, y, OBJNARTI, i);
			image(x, y, OBJNARTI, i + 1);
			break;
		case MapObject:
			switch(object.moveable->element.mapobject) {
			case TreasureChest:
				image(x - 32, y, OBJNRSRC, 18);
				image(x, y, OBJNRSRC, 19);
				break;
			case AncientLamp:
				image(x - 32, y, OBJNRSRC, 14);
				image(x, y, OBJNRSRC, 15);
				break;
			}
			break;
		}
		break;
	case Hero:
		paint_hero(x, y + 30, object.hero->getkind(), object.hero->getdirection(), false);
		paint_flag(x, y + 30, object.hero->getplayer()->getid(), object.hero->getdirection(), draw::counter, true);
		paint_shad(x, y + 30, object.hero->getdirection(), 0);
		break;
	default:
		break;
	}
}