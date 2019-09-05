#include "main.h"

DECLBASE(eventi, 64);

void eventi::clear() {
	memset(this, 0, sizeof(*this));
}
