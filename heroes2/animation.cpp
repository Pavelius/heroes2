#include "view.h"

using namespace draw;

void animation::set(res_s res, unsigned char frame) {
	this->res = res;
	this->frame = frame;
	pos.x = pos.y = 0;
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

bool animation::update() {
	if(wait) {
		wait--;
		return false;
	}
	if(++frame >= start + count)
		frame = start;
	return frame == start;
}

void animation::paint(int x, int y, unsigned flags) const {
	image(x + pos.x, y + pos.y, res, frame, flags);
}