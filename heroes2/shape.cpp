#include "main.h"

bool shapei::is(short unsigned index) const {
	auto x = map::i2x(index);
	auto y = map::i2y(index);
	for(int i = 0; i < count; i++) {
		if(content[i] == 0)
			continue;
		auto px = x + points[i].x;
		auto py = y + points[i].y;
		if(px < 0 || px >= map::width || py < 0 || py >= map::height)
			continue;
		if(map::m2i(px, py) == index)
			return true;
	}
	return false;
}