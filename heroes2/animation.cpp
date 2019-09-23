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
			this->pos.x = -draw::getwidth(res, frame) / 2;
			this->pos.y = -draw::getheight(res, frame) / 2;
		}
		break;
	case SPELLS:
		this->pos.x = -draw::getwidth(res, frame) / 2;
		this->pos.y = -draw::getheight(res, frame) / 2;
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

void animation::paint(int x, int y) const {
	image(x + pos.x, y + pos.y, res, frame, flags);
}

res_s animation::getmissile(monster_s id) {
	switch(id) {
	case Orc:
	case OrcChief:
		return ORC__MSL;
	case Druid:
	case GreaterDruid:
		return DRUIDMSL;
	case Halfling:
		return HALFLMSL;
	case Titan:
		return TITANMSL;
	case Troll:
	case WarTroll:
		return TROLLMSL;
	case Lich:
	case PowerLich:
		return LICH_MSL;
	case Mage:
	case ArchMage:
	case Centaur:
	case Archer:
	case Ranger:
		return ARCH_MSL;
	case Elf:
	case GrandElf:
		return ELF__MSL;
	default:
		return NoRes;
	}
}

int animation::getmissiledy(monster_s id, int param) {
	switch(id) {
	case Archer:
	case Ranger:
		switch(param) {
		case 1: return -15;
		case 2: return -3;
		case 3: return 10;
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
		case 1: return -30;
		case 2: return -20;
		case 3: return 0;
		default: break;
		}
		break;
	case Elf:
	case GrandElf:
		switch(param) {
		case 1: return -5;
		case 2: return 0;
		case 3: return 5;
		default: break;
		}
		break;
	case Centaur:
		switch(param) {
		case 1: return -20;
		case 2: return -10;
		case 3: return 5;
		default: break;
		}
		break;
	case Druid:
	case GreaterDruid:
		switch(param) {
		case 1: return -20;
		case 2: return -5;
		case 3: return 15;
		default: break;
		}
		break;
	case Halfling:
		switch(param) {
		case 1: return -20;
		case 2: return 10;
		case 3: return 20;
		default: break;
		}
		break;
	case Mage:
	case ArchMage:
		switch(param) {
		case 1: return -40;
		case 2: return -10;
		case 3: return 25;
		default: break;
		}
		break;
	case Titan:
		switch(param) {
		case 1: return -80;
		case 2: return -20;
		case 3: return 15;
		default: break;
		}
		break;
	default: break;
	}
	return 0;
}

point animation::getcenter() const {
	point pt;
	auto rc = draw::get(res, frame, pos.x, pos.y, flags);
	pt.x = rc.x1 + rc.width() / 2;
	pt.y = rc.y1 + rc.height() / 2;
	return pt;
}

point animation::getbreast() const {
	point pt = pos;
	//auto rc = draw::get(res, frame, pos.x, pos.y, flags);
	//pt.x = rc.x1 + rc.width() / 2;
	//pt.y = rc.y1 + rc.height() / 3;
	pt.y -= 50;
	return pt;
}

point animation::gethead() const {
	point pt = pos;
	pt.y -= 70;
	return pt;
}

int animation::getparam(direction_s d) {
	switch(d) {
	case Left: case Right: return 2;
	case LeftUp: case RightUp: return 1;
	default: return 3;
	}
}

point animation::getlaunch(monster_s id, direction_s d) const {
	auto pt = getcenter();
	pt.y += getmissiledy(id, getparam(d));
	return pt;
}

void animation::freezy(int duration) {
	frame = start + count - 1;
	wait = duration;
}

res_s animation::getspell(spell_s id) {
	switch(id) {
	case Bless: return BLESS;
	case Curse: return CURSE;
	default: return NoRes;
	}
}