#include "view.h"

using namespace draw;

void drawable::border() const {
	rectb({x, y, x + 32, y + 32}, 0x10);
}

void drawable::paint() const {
	switch(object.type) {
	case Moveable:
		border();
		switch(object.moveable->element.type) {
		case Monster:
			image(x + 16, y + 16, object.moveable->element.monster, object.moveable->index);
			break;
		case Resource:
			image(x - 32, y, OBJNRSRC, object.moveable->element.resource * 2);
			image(x, y, OBJNRSRC, object.moveable->element.resource * 2 + 1);
			break;
		}
		break;
	default:
		break;
	}
}