#include "view.h"

using namespace draw;

list::list() : origin(0), current(0), maximum(0) {}

static void input_edit() {
}

void list::draw(int x, int y, int sy, int sx, int count) {
	prerender();
	int ib = origin;
	int ie = origin + count;
	int y1 = y;
	while(ib < ie) {
		row(x, y1, ib);
		y1 += sy;
		ib++;
	}
	// scroll
	res_s icn = isevil(SCROLLE, SCROLLCN);
	button(x + 56, y + 1, icn, input_edit, {0, 0, 1});
	button(x + 56, y + 113, icn, input_edit, {2, 2, 3});
	button(x + 59, y + 19, icn, input_edit, {4, 4, 4});
}

void list::flatbutton(int x, int y, res_s icn, int index, int command, int count) {
	int w1 = getwidth(icn, index);
	int n1 = getheight(icn, index);
	rect rc = {x, y, x + w1, y + n1};
	if(hot::mouse.in(rc)) {
		if(hot::pressed)
			index++;
		if(hot::key == MouseLeft && hot::pressed) {
			switch(command) {
			case KeyUp:
				if(origin)
					origin--;
				break;
			case KeyDown:
				if(origin + count < maximum)
					origin++;
				break;
			}
		}
	}
	image(x, y, icn, index);
}

void list::area(int x, int y, int count) {
	box(x, y, count, REQUESTS, 19, 300,
		5, 7, 3, 1,
		-1, -1, ESCROLL);
}

void list::box(int x, int y, int count) {
	box(x, y, count, LISTBOX, 19, 1,
		3, 5, 10, 0,
		7, 0, LISTBOXS);
}

void list::boxs(int x, int y, int count) {
	box(x, y, count, LISTBOXS, 19, 1,
		3, 5, 10, 0,
		7, 0, LISTBOXS);
}

void list::box(int x, int y, int count, res_s icn, int n1, int dx, int sbu, int sbd, int sbs, int sbds, int sb, int bf, res_s iss) {
	prerender();
	int x1 = x + 2;
	int y1 = y;
	int w1 = dx < 16 ? getwidth(icn, dx) : dx;
	int w3 = getwidth(icn, sbu);
	rect rc = {x, y, x + w1, y + count * n1 + 2 * 2};
	rect scroll = {0};
	bool correction = false;
	// pre-correction
	if(current < 0)
		current = 0;
	if(current >= maximum)
		current = maximum - 1;
	if(origin > maximum - count)
		origin = maximum - count;
	if(origin < 0)
		origin = 0;
	if(maximum > count) {
		scroll.set(rc.x2 - w3, rc.y1, rc.x2, rc.y2);
		rc.x2 -= w3;
		rc.x2 -= sbds * 20;
	} else if(sbds * 20) {
		rc.x2 -= w3;
		rc.x2 -= sbds * 20;
	}
	// input handle
	switch(hot::key) {
	case KeyUp:
		current--;
		correction = true;
		break;
	case KeyDown:
		current++;
		correction = true;
		break;
	case KeyHome:
		current = 0;
		correction = true;
		break;
	case KeyEnd:
		current = maximum - 1;
		correction = true;
		break;
	case KeyPageUp:
		if(current != origin)
			current = origin;
		else
			current -= count - 1;
		correction = true;
		break;
	case KeyPageDown:
		if(current != (origin + count - 1))
			current = (origin + count - 1);
		else
			current += count - 1;
		correction = true;
		break;
	case MouseLeft:
		if(hot::mouse.in(rc))
			current = origin + (hot::mouse.y - rc.y1 - 2) / n1;
		break;
	}
	// show
	if(bf != -1) {
		int n0 = getheight(icn, 0);
		int n1 = getheight(icn, 1);
		image(x, y1, icn, 0); y1 += n0;
		for(int i = 2; i < count; i++) {
			image(x, y1, icn, 1);
			y1 += n1;
		}
		image(x, y1, icn, 2);
	}
	// scroll bar
	if(scroll) {
		flatbutton(scroll.x1, scroll.y1, icn, sbu, KeyUp, count);
		flatbutton(scroll.x1, scroll.y2 - getheight(icn, sbd) + sbds, icn, sbd, KeyDown, count);
		if(sb != -1) {
			int n7 = getheight(icn, 7);
			int n8 = getheight(icn, 8);
			image(scroll.x1, scroll.y1 + getheight(icn, sbd), icn, 7);
			y1 = scroll.y1 + getheight(icn, sbu) + n7;
			int y2 = scroll.y2 - getheight(icn, sbd) - n7 - n8 + 2;
			while(y1 < y2) {
				image(scroll.x1, y1, icn, 8);
				y1 += n8;
			}
			image(scroll.x1, scroll.y2 - getheight(icn, sbd) - n7, icn, 9);
		}
		// slider
		int n10 = getwidth(iss, sbs);
		int s1 = scroll.y1 + getheight(icn, sbu) + 4;
		int s2 = scroll.y2 - getheight(icn, sbd) - 8 - sbds * 4;
		int ds = s2 - s1 - n10;
		int dr = maximum - count;
		int p = (origin*ds) / dr;
		image(scroll.x1 + 5 - sbds * 2, s1 + p, iss, sbs);
		// mouse click
		rect rc = {scroll.x1, s1, scroll.x2, s2};
		if(hot::mouse.in(rc) && hot::key == MouseLeft) {
			if(hot::mouse.y < s1 + p) {
				origin -= count;
				if(origin < 0)
					origin = 0;
			} else if(hot::mouse.y > s1 + p + n10) {
				origin += count;
				if(origin + count > maximum)
					origin = maximum - count;
			} else {
				// TODO: Slider handle.
			}
		}
	}
	// correction
	if(current < 0)
		current = 0;
	if(current >= maximum)
		current = maximum - 1;
	if(correction) {
		if(current < origin)
			origin = current;
		if(current > origin + count - 1)
			origin = current - count + 1;
	}
	// content
	y1 = y + 2;
	for(int i = 0; i < count; i++) {
		int id = i + origin;
		if(id >= maximum)
			break;
		row(x1, y1, origin + i);
		if(id == current)
			shadow(x1, y1, x1 + rc.width() - 4, y1 + n1 - 1, 1);
		y1 += n1;
	}
}