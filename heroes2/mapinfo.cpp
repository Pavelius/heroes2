#include "main.h"

DECLBASE(mapinfoi, 64);

const mapinfoi* mapinfoi::find(short unsigned index) {
	for(unsigned i = 0; i < bsmeta<mapinfoi>::count; i++) {
		if(bsmeta<mapinfoi>::elements[i].index == index)
			return &bsmeta<mapinfoi>::elements[i];
	}
	return 0;
}