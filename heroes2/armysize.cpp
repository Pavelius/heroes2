#include "main.h"

armysizei bsmeta<armysizei>::elements[] = {{"Мало", 1},
{"Несколько", 5},
{"Группа", 10},
{"Много", 20},
{"Орда", 50},
{"Толпа", 100},
{"Рой", 250},
{"Армия", 500},
{"Легион", 1000},
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