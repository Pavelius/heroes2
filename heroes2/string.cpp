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
{Spell, "sp"},
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
			source[count].type = pi->variant;
			source[count].value = pn;
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

void string::addi(const variantcol& v) {
	addi(v, v.count, v.format);
}

void string::addh(const char* format, ...) {
	addsep('\n');
	add("##");
	addv(format, xva_start(format));
	add("\n");
}

void string::addt(const costi& v) {
	auto n = 0;
	for(auto i = FirstResource; i <= Gems; i = resource_s(i + 1)) {
		if(!v.data[i])
			continue;
		if(n != 0)
			adds(" � ");
		adds("%1i %-2", v.data[i], bsmeta<resourcei>::elements[i].nameof);
	}
}

void string::addo(const heroi* p) {
	static ability_s abilities[] = {Attack, Defence, SpellPower, Knowledge};
	for(auto id : abilities)
		addn("%1: %2i", getstr(id), p->get(id));
	addn("\n");
	addn("������: %1", getstr((morale_s)(p->get(MoraleStat)+NormalMorale)));
	addn("�����: %1", getstr((luck_s)(p->get(LuckStat)+NormalLuck)));
	addn("���� �����: %1i/%2i", p->get(SpellPoints), p->getspmax());
}