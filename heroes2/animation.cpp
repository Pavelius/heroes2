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

bool animation::update(int step) {
	if(wait) {
		wait--;
		return false;
	}
	frame += step;
	if(step > 0) {
		if(frame >= start + count) {
			frame = start;
			return true;
		}
	} else {
		if(frame <= start) {
			frame = start + count - 1;
			return true;
		}
	}
	return false;
}

void animation::paint(int x, int y, unsigned flags) const {
	image(x + pos.x, y + pos.y, res, frame, flags);
}