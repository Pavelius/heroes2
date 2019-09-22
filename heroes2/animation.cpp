#include "view.h"

using namespace draw;

void animation::set(res_s res, unsigned char frame, point pos) {
	this->res = res;
	this->frame = frame;
	this->pos = pos;
	start = frame; count = 1;
	wait = 0;
	flags = 0;
	switch(this->res) {
	case ADVMCO:
		if(frame != 0) {
			pos.x = -draw::getwidth(res, frame)/2;
			pos.y = -draw::getheight(res, frame)/2;
		}
		break;
	}
}

bool animation::increment() {
	if(wait)
		wait--;
	else if(frame < start + count - 1)
		frame++;
	else
		return true;
	return false;
}

void animation::update() {
	if(increment())
		frame = start;
}

void animation::paint(int x, int y, unsigned flags) const {
	image(x + pos.x, y + pos.y, res, frame, flags);
}

int animation::getmissiledy(monster_s id, int param) {
	switch(id) {
	case Archer:
	case Ranger:
		switch(param) {
		case 0: return -15;
		case 1: return -3;
		case 2: return 10;
		default: break;
		}
		break;
	case Orc:
	case OrcChief:
		return 5;
	case Troll:
	case WarTroll:
		return -20;
	case Lich:
	case PowerLich:
		switch(param) {
		case 0: return -30;
		case 1: return -20;
		case 2: return 0;
		default: break;
		}
		break;
	case Elf:
	case GrandElf:
		switch(param) {
		case 0: return -5;
		case 1: return 0;
		case 2: return 5;
		default: break;
		}
		break;
	case Centaur:
		switch(param) {
		case 0: return -20;
		case 1: return -10;
		case 2: return 5;
		default: break;
		}
		break;
	case Druid:
	case GreaterDruid:
		switch(param) {
		case 0: return -20;
		case 1: return -5;
		case 2: return 15;
		default: break;
		}
		break;
	case Halfling:
		switch(param) {
		case 0: return -20;
		case 1: return 10;
		case 2: return 20;
		default: break;
		}
		break;
	case Mage:
	case ArchMage:
		switch(param) {
		case 0: return -40;
		case 1: return -10;
		case 2: return 25;
		default: break;
		}
		break;
	case Titan:
		switch(param) {
		case 0: return -80;
		case 1: return -20;
		case 2: return 15;
		default: break;
		}
		break;
	default: break;
	}
	return 0;
}