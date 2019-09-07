#include "main.h"

const char* castle_names[] = {"Потралис", "Черный Ров", "Зеленый луг", "Сорпигал", "Речной опор",
"Неприступная стена", "Черный замок", "Чернолесье", "Западный предел", "Орлинное гнездо",
"Высокая стена", "Золотые ворота"};

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

static unsigned char fill_values(unsigned char* indecies, unsigned char source_count, allowproc allow, int param) {
	auto p = indecies;
	for(unsigned char i = 0; i < source_count; i++) {
		if(allow && !allow(i, param))
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

static unsigned char get_random(unsigned char count, allowproc allow, int param) {
	unsigned char indecies[256];
	auto c = fill_values(indecies, count, allow, param);
	if(!c)
		return 0xFF;
	auto n = indecies[rand() % c];
	return n;
}

static bool allow_artifact(unsigned char i, int level) {
	if(i == HideousMask || i == FizbinMesfortune || i == TaxLien)
		return false;
	if(level == 0)
		return bsmeta<artifacti>::elements[i].level != 4;
	return bsmeta<artifacti>::elements[i].level == level;
}

static bool allow_spell(unsigned char i, int level) {
	if(level == 0)
		return bsmeta<spelli>::elements[i].level != 5;
	return bsmeta<spelli>::elements[i].level == level;
}

static bool allow_monster(unsigned char i, int level) {
	if(level == 0)
		return bsmeta<artifacti>::elements[i].level != 4;
	return bsmeta<monsteri>::elements[i].level == level;
}

artifact_s generator::artifact(int level) {
	return (artifact_s)get(artifacts, sizeof(artifacts), allow_artifact, level);
}

artifact_s generator::any_artifact(int level) {
	return (artifact_s)get_random(BlackPearl, allow_artifact, level);
}

monster_s generator::monster(int level) {
	return (monster_s)get(monsters, sizeof(monsters), allow_monster, level);
}

resource_s generator::resource() {
	return resource_s(xrand(Gold, Gems));
}

const char* generator::castlename() {
	return castle_names[(castle_index++)% (sizeof(castle_names) / sizeof(castle_names[0]))];
}

spell_s	generator::spell(int level) {
	return (spell_s)get(spells, sizeof(spells), allow_spell, level);
}