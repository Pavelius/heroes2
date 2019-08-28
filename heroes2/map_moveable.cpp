#include "main.h"

moveablei	bsmeta<moveablei>::elements[2048];
unsigned	bsmeta<moveablei>::count;

moveablei* moveablei::add() {
	return &bsmeta<moveablei>::elements[bsmeta<moveablei>::count++];
}

void add_moveable(short unsigned index, variant v, short unsigned quantity) {
	auto p = moveablei::add();
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
}