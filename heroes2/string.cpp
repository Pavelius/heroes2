#include "main.h"

void string::addi(variant v, int value) {
	add("$$");
	switch(v.type) {
	case Resource: add("rs%1i.%2i", v.resource, value); break;
	default: add("zz%1i.%2i", v.value, value); break;
	}
}

void string::addh(const char* format, ...) {
	add("##");
	addv(format, xva_start(format));
	add("\n");
}