#include "main.h"

typedef bool(*allowproc)(unsigned char i, int param);

static unsigned char fill_source(unsigned char* indecies, unsigned char* source, unsigned char source_count, unsigned char level, allowproc allow, int param) {
	auto p = indecies;
	for(unsigned char i = 0; i < source_count; i++) {
		if(allow && !allow(i, param))
			continue;
		if(source[i] > level)
			continue;
		*p++ = i;
	}
	return p - indecies;
}

static unsigned char get(unsigned char* source, unsigned char source_count, allowproc allow, int param) {
	for(unsigned char i = 0; i < 255; i++) {
		unsigned char indecies[256];
		auto c = fill_source(indecies, source, source_count, i, allow, param);
		if(!c)
			continue;
		auto n = indecies[rand() % c];
		source[n]++;
		return n;
	}
	return 0;
}

static bool allow_artifact(unsigned char i, int level) {
	if(level == 0)
		return true;
	return bsmeta<artifacti>::elements[i].level == level;
}

static bool allow_monster(unsigned char i, int level) {
	if(level == 0)
		return true;
	return bsmeta<monsteri>::elements[i].level == level;
}

generator::generator() {
	memset(this, 0, sizeof(*this));
}

artifact_s generator::artifact(int level) {
	return (artifact_s)get(artifacts, sizeof(artifacts), allow_artifact, level);
}

monster_s generator::monster(int level) {
	return (monster_s)get(monsters, sizeof(monsters), allow_monster, level);
}

resource_s generator::resource() {
	return resource_s(xrand(Gold, Gems));
}