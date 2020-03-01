#include "view.h"

uniti* uniti::getbestenemy(bool close_only) const {
	uniti* pr = 0;
	unsigned prs = 0;
	for(auto& e : bsmeta<draw::unitai>()) {
		if(!e.isalive())
			continue;
		if(!e.isenemy(this))
			continue;
		if(close_only) {
			if(!isclose(e.getpos()))
				continue;
		}
		auto prs1 = getscore(e);
		if(prs1 > prs) {
			prs = prs1;
			pr = &e;
		}
	}
	return pr;
}

void uniti::automove() {
	if(isarcher() && canshoot()) {
		auto p = getbestenemy(false);
		if(p)
			shoot(*p);
	} else if(ishandfight()) {
		auto p = getbestenemy(true);
		auto i = p->getpos();
		melee(*p, getattackpos(i));
	} else {
		auto p = getbestenemy(false);
		if(p) {
			auto i = p->getpos();
			auto d = getattackpos(i);
			auto t = to(i, to(d, Down));
			auto m = getnextmove(t, get(Speed) + 1);
			move(m);
			if(t == m)
				melee(*p, to(d, Down));
		}
	}
}