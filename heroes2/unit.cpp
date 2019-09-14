#include "main.h"

const int awd = 11, ahd = 9;
static const direction_s all_around[] = {Left, Right, LeftUp, LeftDown, RightUp, RightDown};

void uniti::setup(squadi& v, heroi* h) {
	unit = v.unit;
	count = v.count;
	source = &v;
	leader = h;
	shoots = bsmeta<monsteri>::elements[unit].shoots;
}

void uniti::refresh() {
	remove(Moved);
	remove(CounterAttacked);
	remove(TotalDefence);
}

int	uniti::get(ability_s v) const {
	switch(v) {
	case Shoots: return shoots;
	default: return squadi::get(v, leader); break;
	}
}

static bool isvalidindex(short unsigned i, const heroi* leader) {
	if(i == Blocked)
		return true;
	auto p = uniti::find(i);
	if(!p)
		return true;
	if(p->leader == leader)
		return true;
	return false;
}

bool uniti::canshoot() const {
	if(!shoots)
		return false;
	auto i = getpos();
	for(auto d : all_around) {
		if(!isvalidindex(to(i, d), leader))
			return false;
	}
	return true;
}

short unsigned uniti::to(short unsigned i, direction_s d) {
	if(i == Blocked)
		return Blocked;
	auto x = i % awd, y = i / awd;
	switch(d) {
	case Left:
		if(x == 0)
			return Blocked;
		return i - 1;
	case Right:
		if(x < awd - 1)
			return i + 1;
		return Blocked;
	case LeftUp:
		if(x >= (y & 1) && y > 0)
			return i - 11 - (y & 1);
		return Blocked;
	case RightUp:
		if(x >= awd - 1 + (y & 1))
			return Blocked;
		if(y == 0)
			return Blocked;
		return i - 10 - (y & 1);
	case LeftDown:
		if((x >= (y & 1)) && y < (ahd - 1))
			return i + 11 - (y & 1);
		return Blocked;
	case RightDown:
		if(x < (awd - 1 + (y & 1)) && y < (ahd - 1))
			return i + 12 - (y & 1);
		return Blocked;
	default:
		return i;
	}
}