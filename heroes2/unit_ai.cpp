#include "view.h"

uniti* uniti::getbestenemy() const {
	uniti* pr = 0;
	unsigned prs = 0;
	for(auto& e : bsmeta<draw::unitai>()) {
		if(!e.isalive())
			continue;
		if(!e.isenemy(this))
			continue;
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
		auto p = getbestenemy();
		if(p)
			shoot(*p);
	} else {
		auto p = getbestenemy();
	}
}