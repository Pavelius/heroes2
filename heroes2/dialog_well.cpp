#include "view.h"

using namespace draw;

static int field(int x, int y, const char* short_name, const char* value) {
	if(!value || value[0] == 0)
		return 0;
	if(short_name) {
		char temp[128]; zprint(temp, "%1:", short_name);
		textm(x, y, 50, AlignRight, temp);
		text(x + 52, y, value);
	} else
		text(x + (80 - textw(value)) / 2, y, value);
	return draw::texth();
}

static int field(int x, int y, squadi& m, ability_s id) {
	auto v = m.get(id);
	if(!v)
		return 0;
	char temp[32]; zprint(temp, "%1i", v);
	return field(x, y, getstr(id), temp);
}

static int field(int x, int y, squadi& m, ability_s i1, ability_s i2, const char* short_name, const char* format = "%1i/%2i") {
	auto v1 = m.get(i1);
	auto v2 = m.get(i2);
	char temp[32]; zprint(temp, format, v1, v2);
	return field(x, y, short_name, temp);
}

static int field(int x, int y, const char* short_name, const char* format, int value) {
	char temp[32];
	if(!value)
		return 0;
	zprint(temp, format, value);
	return field(x, y, short_name, temp);
}

void castlei::paint_monster(int x, int y, int height, int width, int level) {
	char temp[32];
	draw::state push;
	font = SMALFONT;
	auto type = kind;
	auto dwl = building_s(Dwelving1 + level - 1);
	auto dwn = getupgraded(dwl);
	auto bld = getbuildings(type);
	auto allow = true;
	if(dwn == NoBuilding) {
		dwn = dwl;
		allow = false;
	}
	auto monster = getmonster(dwn, getkind());
	auto icn = (res_s)(MONH0000 + monster - FirstMonster);
	auto name = getstr(monster);
	auto frame = getframe(dwn);
	textm(x, y + 2, 200, AlignCenter, name);
	image(x + 1, y + 18, bld, frame);
	name = getstr(dwn, type);
	textm(x, y + 85, 138, AlignCenter, name);
	if(allow) {
		textm(x, y + 102, 70, AlignRight, "Доступно:");
		auto pf = font;
		font = FONT;
		zprint(temp, "%1i", population[level - 1]);
		draw::text(x + 80, y + 100, temp);
		font = pf;
	}
	if((level & 1) == 0)
		image(x + 130 + 85, y + 20, icn, 0, AFMirror);
	else
		image(x + 130 + 2, y + 20, icn, 0);
	squadi sq = {monster, 0};
	y += 4;
	y += field(x + 210, y, sq, Attack);
	y += field(x + 210, y, sq, Defence);
	y += field(x + 210, y, sq, DamageMin, DamageMax, "Урон", "%1i-%2i");
	y += field(x + 210, y, sq, HitPointsMax);
	//y += field(x + 210, y, monster, Rating, szt("Rate", "Рейт"));
	y += draw::texth();
	y += field(x + 210, y, 0, bsmeta<speedi>::elements[sq.get(Speed)].name);
	if(allow)
		y += field(x + 210, y, 0, "Прирост +%1i", getgrowth(dwl));
}

void castlei::well() {
	auto type = kind;
	auto player = getplayer();
	while(ismodal()) {
		image(0, 0, WELLBKG, 0);
		status({21, height - 16, 21 + getwidth(SMALLBAR, 0), height - 1});
		paint_monster(20, 18, 288, 124, 1);
		paint_monster(20, 168, 288, 124, 3);
		paint_monster(20, 318, 288, 124, 5);
		paint_monster(334, 18, 288, 124, 2);
		paint_monster(334, 168, 288, 124, 4);
		paint_monster(334, 318, 288, 124, 6);
		button(578, 461, WELLXTRA, buttoncancel, {0, 0, 1}, KeyEscape);
		cursor(ADVMCO, 0);
		domodal();
	}
}