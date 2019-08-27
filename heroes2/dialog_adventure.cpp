#include "view.h"

using namespace draw;

static unsigned		info_stamp;
static char			info_text[512];
static rect			rcmap = {16, 16, 16 + 32 * 14, 16 + 32 * 14};

static struct castlec : public list {

	castlei*		data[128];

	void setup(const playeri* player) {
		maximum = 0;
		for(auto i = 0; i < 128; i++) {
			auto& e = bsmeta<castlei>::elements[i];
			if(!e)
				continue;
			if(e.getplayer() != player)
				continue;
			data[maximum++] = &e;
		}
	}

	void row(int x, int y, int index) const override {
		if(index < maximum && data[index]) {
			auto p = data[index];
			auto index_sprite = 1;
			auto iscastle = p->is(Castle);
			auto icn = isevil(LOCATORE, LOCATORS);
			switch(p->getkind()) {
			case Knight: index_sprite = iscastle ? 9 : 15; break;
			case Barbarian: index_sprite = iscastle ? 10 : 16; break;
			case Sorcerer: index_sprite = iscastle ? 11 : 17; break;
			case Warlock: index_sprite = iscastle ? 12 : 18; break;
			case Wizard: index_sprite = iscastle ? 13 : 19; break;
			case Necromancer: index_sprite = iscastle ? 14 : 20; break;
			}
			image(x - 1, y, icn, index_sprite);
			//if(bsget(rec, AlreadyMoved))
			//	draw::image(x - 1, y + 1, icn, 24);
			//handle_input(x, y, rec);
		} else
			image(x - 1, y, isevil(LOCATORE, LOCATORS), 1 + index);
	}

} castles;

static struct heroc : public list {

	heroi* data[LastHero - FirstHero + 1];

	void setup(const playeri* player) {
		maximum = 0;
		for(auto i = FirstHero; i <= LastHero; i = hero_s(i+1)) {
			auto& e = bsmeta<heroi>::elements[i];
			if(e.getplayer() != player)
				continue;
			data[maximum++] = &e;
		}
	}

	void row(int x, int y, int index) const override {
		if(index < maximum && data[index]) {
			auto p = data[index];
			image(x + 4, y + 5, PORTXTRA, 0);
			image(x - 1, y, MINIPORT, p->getid());
			image(x + 4, y + 5, MOBILITY, imin(1000/ 100, 25));
			image(x + 43, y + 5, MANA, imin(20 / 5, 25));
		} else
			image(x - 1, y, isevil(LOCATORE, LOCATORS), 1 + index);
	}

} heroes;

static void paint_tiles(rect screen, point camera) {
	draw::state push;
	draw::clipping = screen;
	for(int y = screen.y1; y < screen.y2; y += 32) {
		for(int x = screen.x1; x < screen.x2; x += 32) {
			int index = map::m2i((x + camera.x) / 32, (y + camera.y) / 32);
			imagt(x, y, TisGROUND32, map::tiles[index], map::flags[index] % 4);
		}
	}
}
static void paint_screen() {
	// After some time clear text info
	if(info_stamp < clock())
		info_text[0] = 0;
	image(0, 0, isevil(ADVBORDE, ADVBORD), 0, 0);
	//minimap(480, 16, 0);
	heroes.draw(481, 176, 32, 32);
	castles.draw(553, 176, 32, 32);
	//paint_information(480, 320, player);
	paint_tiles(rcmap, map::camera);
	//paint_objects(drawables, rcmap, map::camera);
	//paint_route(rcmap, map::camera);
}

static void setup_collections(const playeri* player) {
	castles.row_per_screen = 4;
	castles.setup(player);
	heroes.row_per_screen = 4;
	heroes.setup(player);
}

void playeri::adventure() {
	setup_collections(this);
	while(ismodal()) {
		paint_screen();
		cursor(ADVMCO, 0);
		domodal();
	}
}