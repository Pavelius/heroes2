#include "view.h"

using namespace draw;

void battleimage::operator=(const variant& e) {
	type = e.type;
	value = e.value;
}

void battleimage::clear() {
	memset(this, 0, sizeof(*this));
	index = Blocked;
}

void battleimage::paint() const {
	image(pos.x, pos.y, res, frame, flags);
}

void battleimage::set(action_s action) {
	if(type == Hero) {
		auto kind = bsmeta<heroi>::elements[hero].getkind();
		switch(kind) {
		case Barbarian:
			res = CMBTHROB;
			switch(value) {
			case Wait: start = 15; count = 1; break;
			case Warn: start = xrand(15, 18); count = 1; break;
			case PalmFace: start = 1; count = 5; break;
			case Cast: start = 6; count = 9; break;
			default: break;
			}
			break;
		case Knight:
			res = CMBTHROK;
			switch(action) {
			case Warn: case Wait: start = 15; count = 5; break;
			case PalmFace: start = 1; count = 5; break;
			case Cast: start = 12; count = 2; break;
			default: break;
			}
			break;
		case Necromancer:
			res = CMBTHRON;
			switch(action) {
			case Wait: start = 19; count = 1; break;
			case Warn: start = xrand(17, 19); count = 1; break;
			case PalmFace: start = 1; count = 5; break;
			case Cast: start = 6; count = 9; break;
			default: break;
			}
			break;
		case Sorcerer:
			res = CMBTHROS;
			switch(action) {
			case Warn: case Wait: start = 13; count = 4; break;
			case PalmFace: start = 1; count = 5; break;
			case Cast: start = 6; count = 7; break;
			default: break;
			}
			break;
		case Warlock:
			res = CMBTHROW;
			switch(action) {
			case Warn: case Wait: start = 14; count = 3; break;
			case PalmFace: start = 1; count = 5; break;
			case Cast: start = 6; count = 8; break;
			default: break;
			}
			break;
		case Wizard:
			res = CMBTHROZ;
			switch(action) {
			case Warn: case Wait: start = 16; count = 3; break;
			case PalmFace: start = 1; count = 5; break;
			case Cast: start = 12; count = 7; break;
			default: break;
			}
			break;
		default: res = NoRes; start = 0; count = 0; break;
		}
		frame = start;
	}
}