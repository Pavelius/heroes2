#include "view.h"

using namespace draw;

static list*		current_list;
static list::proc	current_proc;

static void execute_list() {
	(current_list->*current_proc)(hot::param);
}

list::list() : origin(0), current(0), maximum(0) {}

//static void input_edit() {
//}
//
//void list::draw(int x, int y, int sy, int sx, int count) {
//	int ib = origin;
//	int ie = origin + count;
//	int y1 = y;
//	while(ib < ie) {
//		row(x, y1, ib);
//		y1 += sy;
//		ib++;
//	}
//	// scroll
//	res_s icn = isevil(SCROLLE, SCROLLCN);
//	button(x + 56, y + 1, icn, input_edit, {0, 0, 1});
//	button(x + 56, y + 113, icn, input_edit, {2, 2, 3});
//	button(x + 59, y + 19, icn, input_edit, {4, 4, 4});
//}

void list::flatbutton(int x, int y, res_s icn, int index, int command) const {
	int w1 = getwidth(icn, index);
	int n1 = getheight(icn, index);
	rect rc = {x, y, x + w1, y + n1};
	if(hot::mouse.in(rc)) {
		if(hot::pressed)
			index++;
		if(hot::key == MouseLeft && hot::pressed) {
			switch(command) {
			case KeyUp: execute(&list::scroll, origin - 1); break;
			case KeyDown: execute(&list::scroll, origin + 1); break;
			}
		}
	}
	image(x, y, icn, index);
}

void list::view(int x, int y) const {
	box(x, y, REQUESTS, 19, 300,
		5, 7, 3, 1,
		-1, -1, ESCROLL);
}

void list::box(int x, int y, int count) {
	box(x, y, LISTBOX, 19, 1,
		3, 5, 10, 0,
		7, 0, LISTBOXS);
}

void list::boxs(int x, int y, int count) {
	box(x, y, LISTBOXS, 19, 1,
		3, 5, 10, 0,
		7, 0, LISTBOXS);
}

void list::execute(list::proc proc, int param) const {
	current_list = (list*)this;
	current_proc = proc;
	draw::execute(execute_list, param);
}

void list::correct() {
	if(current < 0)
		current = 0;
	if(current >= maximum)
		current = maximum - 1;
	if(origin > maximum - row_per_screen)
		origin = maximum - row_per_screen;
	if(origin < 0)
		origin = 0;
}

void list::ensurevisible() {
	if(current < origin)
		origin = current;
	if(current > origin + row_per_screen - 1)
		origin = current - row_per_screen + 1;
}

void list::select(int index) {
	current = index;
	correct();
	ensurevisible();
}

void list::scroll(int value) {
	origin = value;
	if(origin < 0)
		origin = 0;
	if(origin + row_per_screen > maximum)
		origin = maximum - row_per_screen;
}

void list::box(int x, int y, res_s icn, int n1, int dx, int sbu, int sbd, int sbs, int sbds, int sb, int bf, res_s iss) const {
	auto x1 = x + 2;
	auto y1 = y;
	auto w1 = dx < 16 ? getwidth(icn, dx) : dx;
	auto w3 = getwidth(icn, sbu);
	rect rc = {x, y, x + w1, y + row_per_screen * n1 + 2 * 2};
	rect scroll = {0};
	// pre-correction
	if(maximum > row_per_screen) {
		scroll.set(rc.x2 - w3, rc.y1, rc.x2, rc.y2);
		rc.x2 -= w3;
		rc.x2 -= sbds * 20;
	} else if(sbds * 20) {
		rc.x2 -= w3;
		rc.x2 -= sbds * 20;
	}
	// input handle
	switch(hot::key) {
	case KeyUp: execute(&list::select, current - 1); break;
	case KeyDown: execute(&list::select, current + 1); break;
	case KeyHome: execute(&list::select, 0); break;
	case KeyEnd: execute(&list::select, maximum - 1); break;
	case KeyPageUp:
		if(current != origin)
			execute(&list::select, origin);
		else
			execute(&list::select, current - row_per_screen - 1);
		break;
	case KeyPageDown:
		if(current != (origin + row_per_screen - 1))
			execute(&list::select, (origin + row_per_screen - 1));
		else
			execute(&list::select, (current + row_per_screen - 1));
		break;
	}
	// Mouse input handle
	if(hot::mouse.in(rc)) {
		switch(hot::key) {
		case MouseWheelUp: execute(&list::scroll, origin - 1); break;
		case MouseWheelDown: execute(&list::scroll, origin + 1); break;
		case MouseLeft:
			if(hot::pressed)
				execute(&list::select, origin + (hot::mouse.y - rc.y1 - 2) / n1);
			break;
		}
	}
	// show
	if(bf != -1) {
		auto n0 = getheight(icn, 0);
		auto n1 = getheight(icn, 1);
		image(x, y1, icn, 0); y1 += n0;
		for(int i = 2; i < row_per_screen; i++) {
			image(x, y1, icn, 1);
			y1 += n1;
		}
		image(x, y1, icn, 2);
	}
	// scroll bar
	if(scroll) {
		flatbutton(scroll.x1, scroll.y1, icn, sbu, KeyUp);
		flatbutton(scroll.x1, scroll.y2 - getheight(icn, sbd) + sbds, icn, sbd, KeyDown);
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
		int dr = maximum - row_per_screen;
		int p = (origin*ds) / dr;
		image(scroll.x1 + 5 - sbds * 2, s1 + p, iss, sbs);
		// mouse click
		rect rc = {scroll.x1, s1, scroll.x2, s2};
		if(hot::mouse.in(rc) && hot::key == MouseLeft && hot::pressed) {
			if(hot::mouse.y < s1 + p)
				execute(&list::scroll, origin - row_per_screen);
			else if(hot::mouse.y > s1 + p + n10)
				execute(&list::scroll, origin + row_per_screen);
			else {
				// TODO: Slider handle.
			}
		}
	}
	// content
	y1 = y + 2;
	for(int i = 0; i < row_per_screen; i++) {
		int id = i + origin;
		if(id >= maximum)
			break;
		row(x1, y1, origin + i);
		if(id == current)
			shadow(x1, y1, x1 + rc.width() - 4, y1 + n1 - 1, 1);
		y1 += n1;
	}
}