#include "main.h"

armysizei bsmeta<armysizei>::elements[] = {{"����", 1},
{"���������", 5},
{"������", 10},
{"�����", 20},
{"����", 50},
{"�����", 100},
{"���", 250},
{"�����", 500},
{"������", 1000},
};

const armysizei* armysizei::find(int value) {
	auto result = bsmeta<armysizei>::elements + Legion;
	for(auto& e : bsmeta<armysizei>::elements) {
		if(e.count <= value)
			result = &e;
	}
	return result;
}

armysize_s armysizei::get(int value) {
	return armysize_s(find(value) - bsmeta<armysizei>::elements);
}