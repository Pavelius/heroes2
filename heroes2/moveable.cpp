#include "view.h"

using namespace draw;

DECLBASE(moveablei, 8192);

static resource_s	frame2resource[] = {Ore, Sulfur, Crystal, Gems, Gold};

static unsigned char getroad(unsigned char object, unsigned char index) {
	switch(getres(object)) {
		// from sprite road
	case ROAD:
		switch(index) {
		case 0: case 4: case 5:case 13: case 26: return SH(Up) | SH(Down);
		case 2: case 21: case 28: return SH(Left) | SH(Right);
		case 17: case 29: return SH(LeftUp) | SH(RightDown);
		case 18: case 30: return SH(RightUp) | SH(LeftDown);
		case 3: return SH(Up) | SH(Down) | SH(Left) | SH(Right);
		case 6: return SH(Up) | SH(Down) | SH(Right);
		case 7: return SH(Up) | SH(Right);
		case 9: return SH(Down) | SH(Right);
		case 12: return SH(Down) | SH(Left);
		case 14: return SH(Up) | SH(Down) | SH(Left);
		case 16: return SH(Up) | SH(Left);
		case 19: return SH(LeftUp) | SH(RightDown);
		case 20: return SH(RightUp) | SH(LeftDown);
		default: return 0;
		}
		// castle and tower (gate)
	case OBJNTOWN:
		if(13 == index ||
			29 == index ||
			45 == index ||
			61 == index ||
			77 == index ||
			93 == index ||
			109 == index ||
			125 == index ||
			141 == index ||
			157 == index ||
			173 == index ||
			189 == index)
			return SH(Up) | SH(Down);
		return 0;
		// castle lands (gate)
	case OBJNTWBA:
		if(7 == index ||
			17 == index ||
			27 == index ||
			37 == index ||
			47 == index ||
			57 == index ||
			67 == index ||
			77 == index)
			return SH(Up) | SH(Down);
		return 0;
	default: return 0;
	}
}

moveablei* add_moveable(short unsigned index, object_s v, unsigned char subtype, generator& generate, unsigned short drawobj) {
	static unsigned char encode_resource[] = {12, 0, 2, 4, 6, 8, 10, 16};
	auto p = bsmeta<moveablei>::add();
	p->clear();
	p->set(v);
	p->setframe(subtype);
	p->setpos(index);
	p->setup(generate);
	if(v==ResourceObject)
		drawobj = drawobji::find(OBJNRSRC, encode_resource[subtype])->getid();
	else if(!drawobj)
		drawobj = drawobji::find(v)->getid();
	p->setdraw(drawobj);
	if(bsmeta<objecti>::elements[v].isvisitable())
		map::getvisit(index);
	return p;
}

moveablei* add_object(unsigned short index, unsigned char object, unsigned char frame, generator& generate) {
	static moveablei* last_object;
	const drawobji* pi = 0;
	auto icn = getres(object);
	switch(icn) {
	case OBJNTOWN: // �� ����� ��������� ������
	case OBJNTWBA: // �� ����� ��������� ���� �������
	case OBJNTWRD: // �� ����� �������� ��������� ������
		map::roads[index] = getroad(object, frame);
		return 0;
	case MINIHERO: // No heroes
	case SHADOW32: // No heroes shadows
	case FLAG32: // No player flags
	case OBJNTWSH: // No towns and castles shadow
	case OBJNARTI: // No artifacts tiles
	case MONS32: // No monster images
	case OBJNRSRC: // No resource images
		return 0;
	case OBJNMUL2:
		if(frame == 163) // No event signal (used only in editor)
			return 0;
		pi = drawobji::find(icn, frame);
		break;
	case EXTRAOVR:
		if(last_object) {
			// Abandone mine and Mountain Mines has overlay just after their objects
			if(last_object->is(Mines) || last_object->is(AbandoneMine))
				last_object->set(maptbl(frame2resource, frame));
		}
		return last_object;
	case STREAM:
		return add_moveable(index, Stream, frame, generate, 0);
	case ROAD:
		map::roads[index] = getroad(object, frame);
		return add_moveable(index, Road, frame, generate, 0);
	default:
		pi = drawobji::find(icn, frame);
		break;
	}
	if(pi) {
		// Skip all frame not zero frames
		if((pi->first + pi->shape.zero) != frame)
			return 0;
	}
	// � ����� ������ ����� ��������� ������
	if(pi) {
		last_object = add_moveable(index, pi->object, 0, generate, pi->getid());
		return last_object;
	}
	return 0;
}

void moveablei::clear() {
	memset(this, 0, sizeof(*this));
	index = Blocked;
}

void draw::imags(int x, int y, unsigned short drawobj, unsigned short index) {
	auto& e = bsmeta<drawobji>::elements[drawobj];
	auto& sh = e.shape;
	for(int i = 0; i < sh.count; i++) {
		auto px = x + sh.points[i].x * 32;
		auto py = y + sh.points[i].y * 32;
		auto frame = e.first + sh.indecies[i];
		if(sh.animation[i])
			image(px, py, e.res, frame + 1 + ((draw::counter + index * index) % sh.animation[i]));
		image(px, py, e.res, frame);
	}
}

void draw::imagb(int x, int y, unsigned short drawobj) {
	auto& e = bsmeta<drawobji>::elements[drawobj];
	auto& sh = e.shape;
	for(int i = 0; i < sh.count; i++) {
		auto px = x + sh.points[i].x * 32;
		auto py = y + sh.points[i].y * 32;
		auto frame = e.first + sh.indecies[i];
		//switch(sh.content[i]) {
		//case 1: rectb({px + 1, py + 1, px + 31, py + 31}, 16); break;
		//case 2: rectb({px + 1, py + 1, px + 31, py + 31}, 189); break;
		//}
		if(px == x && py == y) {
			rectb({px + 2, py + 2, px + 30, py + 30}, 74);
			rectb({px + 3, py + 3, px + 29, py + 29}, 73);
		}
	}
}

bool gamei::isresource(unsigned char object) {
	return getres(object) == OBJNRSRC;
}

void moveablei::blockpath(unsigned* path) const {
	auto& e = bsmeta<drawobji>::elements[drawobj];
	auto& sh = e.shape;
	auto x = map::i2x(index);
	auto y = map::i2y(index);
	for(int i = 0; i < sh.count; i++) {
		if(sh.content[i] != 2)
			continue;
		auto px = x + sh.points[i].x;
		auto py = y + sh.points[i].y;
		if(px < 0 || px >= map::width || py < 0 || py >= map::height)
			continue;
		auto index = map::m2i(px, py);
		map::set(index, BlockedTile);
	}
	if(map::isinteract(type))
		map::set(index, ActionTile);
}

const shapei& moveablei::getshape() const {
	return bsmeta<drawobji>::elements[drawobj].shape;
}

player_s moveablei::getplayer() const {
	unsigned char t = 1;
	for(auto i = PlayerBlue; i <= LastPlayer; i = player_s(i+1), t<<=1) {
		if(player.data&t)
			return i;
	}
	return RandomPlayer;
}

moveablei* moveablei::find(short unsigned index) {
	for(auto& e : bsmeta<moveablei>()) {
		if(e.index == index)
			return &e;
	}
	return 0;
}

moveablei* moveablei::find(short unsigned index, object_s type) {
	for(auto& e : bsmeta<moveablei>()) {
		if(e.index == index && e.type==type)
			return &e;
	}
	return 0;
}

moveablei* moveablei::findnear(short unsigned i, object_s type) {
	static direction_s all[] = {Left, Right, Up, Down, LeftUp, LeftDown, RightUp, RightDown};
	auto p = find(i, type);
	if(p)
		return p;
	for(auto d : all) {
		auto i1 = map::to(i, d);
		if(i1 == Blocked)
			continue;
		auto p = find(i1, type);
		if(p)
			return p;
	}
	return 0;
}

bool moveablei::issingleuse() const {
	switch(type) {
	case CampFire:
	case TreasureChest:
	case ResourceObject:
	case MonsterObject:
	case ArtifactObject:
		return true;
	default: return false;
	}
}

interact_s moveablei::getinteract() const {
	return bsmeta<objecti>::elements[type].type;
}