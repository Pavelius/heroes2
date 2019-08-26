#include "view.h"

using namespace draw;

void costi::paint(int x, int y) const {
	struct element {
		resource_s	id;
		const rect	rc;
	};
	static element elements[] = {{Wood, {0, 0, 42, 42}}, {Sulfur, {42, 0, 84, 42}},
	{Crystal, {0, 42, 42, 84}}, {Mercury, {42, 42, 84, 84}},
	{Ore, {0, 84, 42, 126}}, {Gems, {42, 84, 84, 126}},
	{Gold, {0, 126, 84, 164}}
	};
	rectf({x, y, x + 81, y + 191}, 0x20);
	//x += 20;
	for(auto& a : elements) {
		char tips[260];
		if(data[a.id])
			zprint(tips, "%1i %-2", data[a.id], bsmeta<resourcei>::elements[a.id].nameof);
		else
			zprint(tips, "У вас нету %1", bsmeta<resourcei>::elements[a.id].nameof);
		image({x + a.rc.x1, y + a.rc.y1, x + a.rc.x2, y + a.rc.y2}, a.id, data[a.id], tips);
	}
}