#include "main.h"

namespace {
struct iconi {
	variant_s		variant;
	char			nm[3];
};
}
static iconi icons[] = {{Ability, "ab"},
{Artifact, "ar"},
{Building, "bd"},
{Hero, "hr"},
{Monster, "mn"},
{Player, "pl"},
{Resource, "rs"},
{Skill, "sk"},
};

const char* string::parse(const char* p, variantcol* source, unsigned& count) {
	count = 0;
	while(p[0] == '$' && p[1] == '$') {
		p += 2;
		char nm[3] = {p[0], p[1], 0};
		p += 2;
		auto pn = 0, ppn = 0, ppf = 0;
		while(stringbuilder::isnum(*p))
			pn = pn * 10 + (*p++) - '0';
		if(p[0] == '.') {
			p++;
			while(stringbuilder::isnum(*p))
				ppn = ppn * 10 + (*p++) - '0';
		}
		if(p[0] == '.') {
			p++;
			while(stringbuilder::isnum(*p))
				ppf = ppf * 10 + (*p++) - '0';
		}
		iconi* pi = 0;
		for(auto& e : icons) {
			if(nm[0] == e.nm[0] && nm[1] == e.nm[1]) {
				pi = &e;
				break;
			}
		}
		if(pi) {
			source[count].element.type = pi->variant;
			source[count].element.value = pn;
			source[count].count = ppn;
			source[count].format = ppf;
			count++;
		}
	}
	return p;
}

void string::addi(variant v, int value, int format) {
	for(auto& e : icons) {
		if(e.variant == v.type) {
			add("$$%3%1i.%2i.%4i", v.value, value, e.nm, format);
			break;
		}
	}
}

void string::addi(const costi& v) {
	for(auto i = FirstResource; i <= Gems; i = (resource_s)(i + 1)) {
		auto n = v.get(i);
		if(n<=0)
			continue;
		addi(i, n);
	}
}

void string::addh(const char* format, ...) {
	addsep('\n');
	add("##");
	addv(format, xva_start(format));
	add("\n");
}