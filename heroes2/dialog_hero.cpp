#include "view.h"

using namespace draw;

static heroi* current_hero;

static void previous_hero() {}

static void next_hero() {}

static void dismiss_hero() {
	//if(dlgask(0, szt("", "�� ������������� ������ ������� ����� ����� �� ���� ������?"))) {
	//		bsset(rec, Player, 0);
	//		bsset(rec, Index, -1);
	//		breakmodal(0);
	//}
}

void heroi::open_artifact() {
	if(current_hero->artifacts[hot::param] == MagicBook)
		current_hero->showbook(AnySpell, 0);
}

void heroi::show(bool allow_change) const {
	char temp[260];
	while(ismodal()) {
		status({22, height - 17, 22 + getwidth(HSBTNS, 8), height - 1});
		button(0, draw::height - 20, HSBTNS, previous_hero, {4, 4, 5}, KeyLeft, "���������� �����");
		image(22, draw::height - 20, HSBTNS, 8);
		button(width - 22, height - 20, HSBTNS, next_hero, {6, 6, 7}, KeyRight, "��������� �����");
		image(0, 0, HEROBKG, 0);
		image(0, 0, isevil(HEROEXTE, HEROEXTG), 0);
		image(99 - 50, 31, getid(), true);
		zprint(temp, "%1 - %2 ( ������� %3i )",
			name,
			getstr(getkind()),
			level);
		text((width - textw(temp)) / 2, 3, temp);
		// Abilities
		for(auto i = Attack; i <= Knowledge; i = (ability_s)(i + 1)) {
			auto x = 152 + (i - Attack) * 88;
			auto y = 36;
			if(true) {
				draw::state push;
				font = SMALFONT;
				auto p = getstr(i);
				text(x + (88 - textw(p)) / 2, y, p);
			}
			zprint(temp, "%1i", get(i));
			text(x + (88 - draw::textw(temp)) / 2, y + 72, temp);
		}
		// Skills
		if(true) {
			draw::state push;
			font = SMALFONT;
			auto x = 40;
			auto y = 233;
			auto dx = getwidth(SECSKILL, 0) + 5;
			for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
				auto n = get(i);
				if(!n)
					continue;
				image(x - 37, y, i, n);
				x += dx;
			}
			int x2 = 3 + 40 + 8 * dx;
			while(x < x2) {
				image(x - 40 + 3, y, SECSKILL, 0, 0);
				x += dx;
			}
		}
		if(true) {
			const int dx = getwidth(ARTIFACT, 0) + 15;
			const int dy = getheight(ARTIFACT, 0) + 15;
			int x = 51;
			int y = 308;
			int x2 = x + 7 * dx;
			for(unsigned i = 0; i < sizeof(artifacts) / sizeof(artifacts[0]); i++) {
				auto n = artifacts[i];
				if(n) {
					image(x, y, ARTIFACT, n + 1);
					if(mousein({x, y, x + dx, y + dy})) {
						status("�������� %1", getstr(n));
						if(hot::key == MouseLeft && hot::pressed && allow_change) {
							current_hero = (heroi*)this;
							execute(open_artifact, i);
						}
					}
				} else
					image(x, y, ARTIFACT, 0);
				x += dx;
				if(x >= x2) {
					x = 51;
					y += dy;
				}
			}
		}
		// Morale/Luck
		if(true) {
			auto x = 512;
			auto y = 36;
			image(x + 8, y, HSICONS, 4);
			image(x + 44, y, HSICONS, 3);
			image(x, y + 50, HSICONS, 1);
			image(x + 38, y + 50, HSICONS, 8);
		}
		image(49, 130, CREST, player);
		armyi::paint(156, 130, this, allow_change);
		if(allow_change)
			button(5, 318, HSBTNS, dismiss_hero, {0, 0, 1}, Alpha + 'A', "������� �����");
		button(603, 318, HSBTNS, buttoncancel, {2, 2, 3}, KeyEscape, "������� ����");
		domodal();
	}
}

static void information_hero() {
	auto p = (heroi*)hot::param;
	p->show(true);
}

void heroi::input(const playeri* player) const {
	status(getname());
	if(hot::key == MouseLeftDBL && hot::pressed) {
		if(!player || getplayer() == player)
			execute(information_hero, (int)this);
	}
}

int heroi::learn(const char* format, const variantcol* v1, unsigned count) {
	switch(count) {
	case 1: case 2:
		return draw::message(format, v1, count, ButtonLearn);
	default:
		draw::message(format, 0, 0, ButtonOK);
		return -1;
	}
}

int	heroi::ask(const char* format) {
	return draw::message(format, 0, 0, ButtonYesNo);
}

int	heroi::ask(const char* format, const variantcol* source) {
	return draw::message(format, source, 2, ButtonYesNo);
}

void heroi::message(const char* format) {
	draw::message(format, 0, 0, ButtonOK);
}