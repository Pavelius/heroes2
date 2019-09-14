#include "main.h"

void uniti::setup(squadi& v, heroi* h) {
	unit = v.unit;
	count = v.count;
	source = &v;
	leader = h;
}

void uniti::refresh() {
	remove(Moved);
	remove(CounterAttacked);
	remove(TotalDefence);
}