#include "view.h"

using namespace draw;

static spell_type_s mode;

static int compare(const void* p1, const void* p2) {
	auto s1 = *((spell_s*)p1);
	auto s2 = *((spell_s*)p2);
	int m1 = bsmeta<spelli>::elements[s1].level;
	int m2 = bsmeta<spelli>::elements[s2].level;
	if(m1 != m2)
		return m1 - m2;
	return strcmp(bsmeta<spelli>::elements[s1].name, bsmeta<spelli>::elements[s2].name);
}

static int select_spells(spell_s* result, heroi& sb, spell_type_s mode) {
	auto p = result;
	for(auto i = FirstSpell; i <= LastSpell; i = (spell_s)(i+1)) {
		if(i == Stone)
			continue;
		if(mode && bsmeta<spelli>::elements[i].type!=mode)
			continue;
		*p++ = i;
	}
	return p - result;
}

static void adventure_page() {
	mode = AdventureSpell;
}

static void combat_page() {
	mode = CombatSpell;
}

static void cast_spell() {}

static void next_page() {
	*((int*)hot::param) = *((int*)hot::param) + 1;
}

static void prev_page() {
	*((int*)hot::param) = *((int*)hot::param) - 1;
}

void heroi::showbook(spell_type_s original_mode) {
	char temp[32];
	const int cw1 = 82;
	const int cw2 = 42;
	const int ch1 = 76;
	const int spell_per_page = 12;
	spell_s names[LastSpell - FirstSpell + 1];
	int names_count;
	const auto back = BOOK;
	auto w1 = getwidth(back, 0);
	auto h1 = getheight(back, 0);
	auto x = width / 2;
	auto y = (height - h1) / 2 - 32;
	auto current_page = 0;
	draw::screenshoot surface;
	draw::state push;
	draw::font = SMALFONT;
	if(original_mode)
		mode = original_mode;
	if(!mode)
		mode = CombatSpell;
	while(ismodal()) {
		// fill knowing spells
		names_count = select_spells(names, *this, mode);
		qsort(names, names_count, sizeof(names[0]), compare);
		int x1 = x - w1 - 1;
		int y1 = y;
		surface.restore();
		// Open book
		image(x, y, back, 0);
		image(x, y, back, 0, AFMirror);
		image(x1 + 125, y1 + 275, back, 6); // information bookmark
		if(original_mode == AdventureSpell || original_mode == AnySpell)
			button(x1 + 270, y1 + 270 + ((mode != AdventureSpell) ? 14 : 0), back, adventure_page, {3, 3, 3}); // adventure bookmark
		if(original_mode == CombatSpell || original_mode == AnySpell)
			button(x1 + 304, y1 + 270 + ((mode != CombatSpell) ? 14 : 0), back, combat_page, {4, 4, 4}); // combat bookmark
		button(x1 + 420, y1 + 284, back, buttoncancel, {5, 5, 5}, KeyEscape); // exit bookmark
		rect r1 = {x1 + 410, y1 + 10, x1 + 410 + 30, y1 + 10 + 20};
		if(hot::key==MouseLeft && mousein(r1) && hot::pressed) {
			if(current_page < names_count / spell_per_page)
				execute(next_page, (int)&current_page);
		}
		rect r2 = {x1 + 30, y1 + 10, x1 + 30 + 30, y1 + 10 + 20};
		if(hot::key == MouseLeft && mousein(r2) && hot::pressed) {
			if(current_page > 0)
				execute(prev_page, (int)&current_page);
		}
		zprint(temp, "%1i", spell_points);
		text(x1 + 125 + 18 - draw::textw(temp) / 2, y1 + 308, temp);
		y1 += 23;
		x1 += 46;
		int ox = x1;
		int oy = y1;
		for(int i = 0; i + current_page * spell_per_page < names_count; i++) {
			if(i >= spell_per_page)
				break;
			auto sid = names[i + current_page * spell_per_page];
			auto side = i / (spell_per_page / 2); // 0 - left, 1 - right
			auto column = i % 2; // 0 - left column, 1 - right column
			auto row = (i / 2) % 3;
			auto spr = bsmeta<spelli>::elements[sid].portrait;
			rect rc;
			rc.x1 = ox;
			if(column)
				rc.x1 += cw1;
			if(side)
				rc.x1 += cw1 * 2 + 50;
			rc.y1 = oy;
			rc.y1 += row * ch1;
			if(side) {
				rc.x1 += 2 * row;
				if(column)
					rc.y1 += 8;
			} else {
				rc.x1 -= 2 * row;
				if(!column)
					rc.y1 += 8;
			}
			auto cost = getcost(sid);
			rc.x2 = rc.x1 + cw1 - 1;
			rc.y2 = rc.y1 + ch1 - 1;
			draw::image(rc.x1 + (cw1 - getwidth(SPELLS, spr)) / 2, rc.y1 + (cw2 - getheight(SPELLS, spr)) / 2, SPELLS, spr, AFNoOffset);
			zprint(temp, "%1 (%2i)", getstr(sid), cost);
			textm(rc.x1, rc.y1 + cw2 + 1, cw1, AlignCenter, temp);
			if(spell_points >= cost) {
				if(draw::mousein(rc)) {
					if(hot::key == MouseLeft && hot::pressed)
						execute(cast_spell);
					//else if(hot::key == MouseRight && hot::pressed)
					//	execute(Information, names[i]);
				}
			} else {
				if(draw::mousein(rc)) {
					//if(hot::key == MouseRight && hot::pressed)
					//	draw::execute(Information, names[i]);
				}
			}
		}
		cursor(ADVMCO, 0);
		domodal();
	}
}