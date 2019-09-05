#include "main.h"

DECLBASE(eventi, 64);

void eventi::clear() {
	memset(this, 0, sizeof(*this));
	index = Blocked;
}

DECLBASE(dayeventi, 64);

void dayeventi::clear() {
	memset(this, 0, sizeof(*this));
}