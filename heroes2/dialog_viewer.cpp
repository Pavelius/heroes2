#include "view.h"

using namespace draw;

// 1 - water / вода (30)
// 2 - grass / трава (62)
// 3 - snow / снег (62)
// 4 - swamp / болото (62)
// 5 - lava / лава (62)
// 6 - desert / пустыня (62)
// 7 - dirt / грязь (38)
// 8 - canion / каньон (62)
// 9 - beach / пляж (17)

// 1) Каждая местность имеет тайлы границ с пляжем и грязью. Причем в одном тайле может быть одновременно и то и другое.
// 2) Вода граничит только с пляжем.
// 3) Границу считать пляжем для: вода, пустыня, каньон.
//
// 4 - Земля сверху. Зеркально по верикали - снизу.
// 4 - Земля сверху и справа. Зеркально по вертикале - снизу и справа. Зеркально по горизонтале - сверху и слева. Зеркально по горизонтале и вертикале - снизу и слева.
// 4 - Земля справа. Зеркально по горизонтале - слева.
// 4 - Земля наискосок сверху-справа. Зеркально по аналогично.
// Блок земли (16). Блок песка (16). Блок земли и песка (6). Блок середины (8+8+8=24). Итого 62 на каждую местность.
// 8 - Это тайлы середины
// 8 - Это тайлы середины высоких объектов (холмы)
// 8 - Это тайлы середины низких объектов (цветы, трещинки)

void object_view() {
	char temp[64];
	const int maximum_count = bsmeta<drawobji>::count;
	auto index = 0;
	while(ismodal()) {
		auto x1 = 640 / 2 - 16;
		auto y1 = 480 / 2 - 16;
		if(index < 0)
			index = 0;
		else if(index >= maximum_count)
			index = maximum_count - 1;
		auto& e = bsmeta<drawobji>::elements[index];
		auto icn = e.res;
		auto& sh = e.shape;
		rectf({0, 0, draw::width - 1, draw::height - 1}, 0x12);
		int e_count = sh.indecies[sh.count - 1] + 1 + sh.animation[sh.count - 1];
		auto rn = getname(e.res);
		zprint(temp, "%6 %1i/%5i (start=%2i, count=%3i, next=%4i)",
			index, e.first, e_count, e.first + e_count, maximum_count, getstr(e.object));
		text(0, 0, temp);
		point center = {0, 0};
		for(int i = 0; i < sh.count; i++) {
			auto px = x1 + sh.points[i].x * 32;
			auto py = y1 + sh.points[i].y * 32;
			auto frame = e.first + sh.indecies[i];
			if(sh.animation[i])
				image(px, py, icn, frame + 1 + (draw::counter % sh.animation[i]));
			image(px, py, icn, frame);
			if(sh.points[i].x == 0 && sh.points[i].y == 0) {
				center.x = px;
				center.y = py;
			}
			switch(sh.content[i]) {
			case 0: rectb({px + 1, py + 1, px + 31, py + 31}, 26); break;
			case 1: rectb({px + 1, py + 1, px + 31, py + 31}, 10); break;
			case 2: rectb({px + 1, py + 1, px + 31, py + 31}, 190); break;
			}
			if(px == x1 && py == y1)
				rectb({px + 2, y1 + 2, px + 30, py + 30}, 75);
		}
		domodal();
		switch(hot::key) {
		case KeyEscape:
			buttoncancel();
			break;
		case KeyRight: index++; break;
		case KeyLeft: index--; break;
		case KeyUp: index += 10; break;
		case KeyDown: index -= 10; break;
		}
	}
}

void animation_view() {
	auto speed = 6;
	int x = 0;
	battleimage pic;
	pic.type = Monster;
	pic.monster = Peasant;
	pic.unit = pic.monster;
	pic.uniti::count = 13;
	pic.set(Move);
	pic.set(Right);
	pic.flags |= AFMoving;
	while(ismodal()) {
		pic.pos.x = 640 / 2 - 16;
		pic.pos.y = 480 / 2 - 16;
		rectf({0, 0, draw::width - 1, draw::height - 1}, 0x12);
		auto step = pic.frame - pic.start;
		line(pic.pos.x + step * speed, pic.pos.y, pic.pos.x + step * speed, pic.pos.y - 100, 0x30);
		hexagon(pic.pos.x, pic.pos.y, 0x50);
		hexagon(pic.pos.x + cell_wr*2, pic.pos.y, 0x50);
		pic.paint();
		char temp[260];
		zprint(temp, "frames: %1i-%2i, count = %3i\nSpeed: %4i", pic.start, pic.start + pic.animation::count, pic.animation::count, speed);
		textm(10, 10, 600, AlignLeft, temp);
		domodal();
		switch(hot::key) {
		case KeyEscape:
		case KeySpace:
			buttoncancel();
			break;
		case KeyRight:
			if(pic.monster < WaterElement)
				pic.value++;
			pic.unit = pic.monster;
			pic.set(Move);
			break;
		case KeyLeft:
			if(pic.value)
				pic.value--;
			pic.unit = pic.monster;
			pic.set(Move);
			break;
		case KeyUp: speed++; break;
		case KeyDown: speed--; break;
		case InputTimer:
			if(pic.increment())
				pic.frame = pic.start;
			break;
		}
	}
}