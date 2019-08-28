#include "view.h"

using namespace draw;

struct shapei {
	unsigned char	count;
	point			size;
	point			points[24];
	unsigned char	animation[24];
	unsigned char	content[24]; // 0 - shadow, 1 - passable, 2 - blocked
	unsigned char	indecies[24];
	point			offset;
	unsigned char	zero;
	unsigned char	initialized;
};
static shapei sh1x1 = {1, {1, 1}, {{0, 0}}, {0}, {2}};
static shapei sh1x1a6 = {1, {1, 1}, {{0, 0}}, {6}, {2}};
static shapei sh1x1a11 = {1, {1, 1}, {{0, 0}}, {11}, {2}};
//
static shapei sh2x1 = {2, {2, 1}, {{-1, 0}, {0, 0}}, {0}, {0, 2}};
static shapei sh2x1bk = {2, {2, 1}, {{-1, 0}, {0, 0}}, {0}, {2, 2}};
static shapei sh2x1a6 = {2, {2, 1}, {{-1, 0}, {0, 0}}, {6, 6}, {0, 2}};
static shapei sh2x1a6v2 = {2, {2, 1}, {{-1, 0}, {0, 0}}, {0, 6}, {0, 2}};
static shapei sh2x2 = {4, {2, 2}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {0}, {2, 2, 2, 2}};
static shapei sh2x2J = {3, {2, 2}, {{0, -1}, {-1, 0}, {0, 0}}, {0}, {1, 0, 2}};
static shapei sh2x2bk = {3, {2, 2}, {{0, -1}, {-1, 0}, {0, 0}}, {0}, {2, 2, 2}};
static shapei sh2x2Ja9 = {3, {2, 2}, {{0, -1}, {-1, 0}, {0, 0}}, {9, 9, 9}, {1, 0, 2}};
static shapei sh2x3 = {6, {2, 3}, {{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {-1, 1}, {0, 1}}, {0}, {0, 1, 1, 0, 2, 2}};
//
static shapei sh3x1 = {3, {3, 1}, {{-2, 0}, {-1, 0}, {0, 0}}, {0}, {0, 2, 2}};
static shapei sh3x1a6 = {3, {3, 1}, {{-1, 0}, {0, 0}, {1, 0}}, {6, 6, 6}, {0, 2, 2}};
static shapei sh3x2 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh3x2a3 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {3, 3, 3, 3, 3, 3}};
static shapei sh3x2u1a6 = {6, {3, 2}, {{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 0}}, {6, 6, 0, 6, 6, 0}};
static shapei sh3x2u1a6v2 = {5, {3, 2}, {{-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 0, 0, 0}};
static shapei sh3x2u2 = {4, {3, 2}, {{1, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh3x2a6 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {0, 0, 0, 6, 6, 6}};
static shapei sh3x2a15 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {15, 15, 15, 15, 0, 0}};
static shapei sh3x2u1 = {5, {3, 2}, {{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh3x2r1 = {5, {3, 2}, {{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh3x2u1r1 = {4, {3, 2}, {{0, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh3x2u2a10 = {4, {3, 2}, {{1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {10, 0, 10, 0}};
static shapei sh3x2u2a9 = {4, {3, 2}, {{1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {9, 9, 9, 0}};
static shapei sh3x3 = {9, {3, 3}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}};
static shapei sh3x3u1u1 = {7, {3, 3}, {{0, -1}, {1, -1}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}};
static shapei sh3x3u1r1 = {7, {3, 3}, {{0, 0}, {-1, 1}, {0, 1}, {1, 1}, {-1, 2}, {0, 2}, {1, 2}}};
static shapei sh3x3d1b1 = {7, {3, 3}, {{-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}, {0, 2}}};
static shapei sh3x3u1d1 = {7, {3, 3}, {{0, -2}, {1, -2}, {-1, -1}, {0, -1}, {1, -1}, {0, 0}, {1, 0}}};
static shapei sh3x3u1r1a6 = {7, {3, 3}, {{0, -2}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {0, 6, 6, 0, 6, 6, 0}};
static shapei sh3x3r1a6 = {8, {3, 3}, {{0, -2}, {1, -2}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {6, 0, 0, 6, 0, 0, 0, 0}};
static shapei sh3x3u2u2 = {5, {3, 3}, {{0, -2}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}};
static shapei sh3x3u2u2a6 = {5, {3, 3}, {{0, -2}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 0, 6}};
static shapei sh3x3u2m1 = {6, {3, 3}, {{0, -2}, {-2, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {0, 0, 9, 0, 0}};
static shapei sh3x2u2a5 = {4, {3, 2}, {{0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 6}};
static shapei sh3x2u2a5v2 = {4, {3, 2}, {{0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 0}};
//
static shapei sh4x1 = {4, {4, 1}, {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh4x2 = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0}, {0, 1, 1, 2, 0, 2, 2, 2}};
static shapei sh4x2u1 = {7, {4, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh4x2r1 = {7, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh4x2u2 = {6, {4, 2}, {{0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh4x2d1 = {7, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {0}, {2, 2, 2, 2, 1, 2, 2}};
static shapei sh4x2r1d1 = {6, {4, 2}, {{-1, -1}, {0, -1}, {1, -1}, {0, 0}, {1, 0}, {2, 0}}, {0}, {0, 2, 2, 0, 2, 2}};
static shapei sh4x2u1b1 = {6, {4, 2}, {{0, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}}, {0}, {0, 2, 2, 0, 2, 2}};
static shapei sh4x2а6 = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0, 0, 0, 0, 0, 0, 0, 6}};
static shapei sh4x2а6_rogcmp = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0, 0, 0, 0, 0, 0, 6, 0}};
static shapei sh4x3 = {12, {4, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}}};
static shapei sh4x3u1r1 = {10, {4, 3}, {{-1, -2}, {0, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapei sh4x3r1d1 = {10, {4, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}, {0}, {0, 1, 1, 0, 2, 2, 1, 0, 2, 2}};
static shapei sh4x3u1b1 = {10, {4, 3}, {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-2, 1}, {-1, 1}, {0, 1}}, {0}, {0, 1, 1, 0, 1, 2, 2, 0, 2, 2}};
static shapei sh4x3u2a3 = {10, {4, 3}, {{0, -2}, {1, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {3, 3, 3, 3, 3, 3, 3, 3, 3, 3}};
static shapei sh4x3u2r1d1a9 = {8, {4, 3}, {{-1, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {8, 8, 0, 0, 0, 0, 0, 0}};
//
static shapei sh5x2 = {10, {5, 2}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0}, {0, 0, 2, 2, 2, 0, 0, 2, 2, 2}};
static shapei sh5x2u1 = {9, {5, 2}, {{-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
static shapei sh5x2r1 = {9, {5, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
static shapei sh5x3 = {14, {5, 3}, {{-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}}};
static shapei sh5x3a6 = {9, {5, 3}, {{1, -2}, {-2, -1}, {-1, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {6, 6, 6, 6, 0, 0, 0, 0, 0}};
static shapei sh5x3a4 = {9, {5, 3}, {{1, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {10, 10, 10, 0, 10, 0, 0, 0, 0}};
static shapei sh5x3u2a6 = {13, {5, 3}, {{-1, -2}, {0, -2}, {1, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {6, 0, 0, 6, 0, 6, 6, 0, 0, 6, 6, 0, 0}};
//
static shapei sh6x2 = {12, {6, 2}, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}};
static shapei sh6x3r1d2 = {15, {6, 3}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}}};
static shapei sh6x3u1b2 = {15, {6, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}}};
static shapei sh6x3u1a10 = {17, {6, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}, {0, 0, 0, 0, 0, 10, 10, 10, 10, 10, 0, 0, 10, 10, 10, 10, 0}};
static shapei sh6x4 = {24, {6, 4}, {{-3, -3}, {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
static shapei sh6x4r1d2 = {21, {6, 4}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {-1, 2}, {0, 2}, {1, 2}, {2, 2}}, {0}, {0, 1, 1, 1, 1, 0, 2, 2, 2, 2, 1, 0, 2, 2, 2, 2, 2, 0, 2, 2, 2}};
static shapei sh6x4u1b2 = {21, {6, 4}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {-3, 2}, {-2, 2}, {-1, 2}, {0, 2}}, {0}, {0, 1, 1, 1, 2, 0, 1, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 2}};
//
static shapei sh7x3r1 = {20, {7, 3}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}};
static shapei sh7x4 = {22, {7, 4}, {{-2, -3}, {-1, -3}, {0, -3}, {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {-5, -1}, {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
//
static shapei sh8x3 = {24, {8, 3}, {{-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-4, 1}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}};
static shapei sh8x5a10 = {23, {8, 5}, {{0, -4}, {1, -4}, {2, -4}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {3, -3}, {-3, -2}, {0, -2}, {1, -2}, {2, -2}, {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0}};
// Monster animation
static const unsigned char monster_animation_cicle[] = {
	1, 2, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 4, 5, 4, 3, 0,
	0, 0, 0, 0,
};
// Monster another animation
static const unsigned char monster_animation_cicl1[] = {
	1, 2, 1, 0,
	3, 4, 5, 4, 3, 0,
};
// Map object description
struct mapitemi {
	map_object_s	object;
	shapei&			shape;
	unsigned char	first;
	unsigned char	last;
};
static mapitemi grass[] = {{AbandoneMine, sh4x2}, // Haunted mine
{Hole, sh2x1bk},
{Crack, sh4x2d1},
{Crack, sh3x3},
{Crack, sh2x2bk},
{Rock, sh3x1},
{Rock, sh3x1},
{Rock, sh3x2u1r1}, // Big boulder
{Rock, sh2x1}, // Big 
{Rock, sh1x1},
{Rock, sh2x1},
{Rock, sh2x1},
{Rock, sh2x1},
{Rock, sh2x1},
{Lake, sh5x3},
{Flowers, sh1x1}, // Flower
{Lake, sh4x2}, // Lake
{Lake, sh3x1}, // Lake
{Hill, sh3x1}, // Hill
{Trees, sh4x2u1},
{Trees, sh3x2u1},
{Trees, sh2x2J},
{Brush, sh4x1}, // Brush flowers
{Brush, sh4x1}, // Brush
{Brush, sh3x1}, // Brush small
{Brush, sh3x1}, // Brush small flowers
{Brush, sh3x1}, // Brush small to up
{Brush, sh2x1}, // Single brush
{Flowers, sh4x1}, // Flowers red
{Flowers, sh4x2u1}, // Flowers red
{Flowers, sh4x1}, // Flowers red another
{Flowers, sh4x1}, // Flowers blue
{Flowers, sh3x2u1}, // Flowers blue
{Flowers, sh1x1}, // Flowers tiny
{Flowers, sh3x1}, // Flowers
{Flowers, sh2x1}, // Flowers
{Flowers, sh2x1}, // Flowers to up
{Flowers, sh2x1}, // Flowers white
{Flowers, sh2x1}, // Flowers ultraviolet
{Hill, sh2x1}, // Hill
};
static mapitemi grass2[] = {{HillFort, sh3x2u1},
{HalflingHole, sh4x1},
{DiggingHole, sh1x1},
{Cliff, sh2x1},
{Cliff, sh2x1},
{Cliff, sh3x1},
{SpriteHouse, sh4x2u2},
{WindMill, sh4x3u2a3},
{ArcherHouse, sh3x2u2a5},
{GoblinHut, sh2x1},
{PeasantHut, sh3x2u2a5},
{Oracle, sh3x2},
{Obelisk, sh2x2J},
};
static mapitemi dirt[] = {{AbandoneMine, sh5x2},
{Hole, sh2x1},
{Hill, sh2x1},
{Hill, sh3x1},
{Crack, sh2x1},
{Crack, sh2x1},
{Crack, sh2x1},
{Lake, sh8x3},
{Lake, sh5x2},
{Lake, sh2x1},
{Brush, sh3x1},
{Brush, sh3x1},
{Brush, sh3x1},
{Brush, sh2x1},
{Brush, sh2x1},
{Flowers, sh3x1},
{Flowers, sh3x1},
{Flowers, sh3x1},
{Flowers, sh3x1},
{Flowers, sh2x1},
{Flowers, sh1x1},
{Flowers, sh2x1},
{Flowers, sh1x1},
{Flowers, sh1x1},
{Rock, sh3x1},
{Rock, sh3x2},
{Rock, sh3x1},
{Rock, sh2x1},
{Rock, sh1x1},
{Flowers, sh1x1},
{Flowers, sh2x1},
{Flowers, sh2x1},
{EmpthyObject, sh2x1},
{EmpthyObject, sh1x1},
{Trees, sh3x2},
{Trees, sh3x2u1},
{Trees, sh2x2J},
{Mushrooms, sh3x1},
{HillFort, sh3x2u1},
{HalflingHole, sh4x1},
{DiggingHole, sh1x1},
{Cliff, sh2x1},
{Cliff, sh2x1},
{Cliff, sh2x1},
{SpriteCity, sh4x2u2},
{WindMill, sh4x3u2a3},
{Oracle, sh3x2},
{Obelisk, sh2x2J},
};
static mapitemi snow[] = {{Hole, sh2x1}, // Hole in Earth
{CentautCave, sh2x1},
{CampFire, sh1x1a6},
{DiggingHole, sh1x1}, // Digging
{LeanTo, sh2x1},
{Cliff, sh2x1},
{Cliff, sh2x1},
{Cliff, sh3x1},
{Rock, sh2x1},
{Rock, sh3x2u1},
{Rock, sh1x1},
{Rock, sh2x1},
{Rock, sh2x1},
{Rock, sh3x1},
{Rock, sh2x1},
{Rock, sh2x1},
{EmpthyObject, sh2x1}, // Wood (пеньки)
{EmpthyObject, sh1x1}, // Wood (пеньки)
{EmpthyObject, sh1x1}, // Grass
{EmpthyObject, sh1x1}, // Grass
{EmpthyObject, sh1x1}, // Grass
{Trees, sh3x2u1},
{Trees, sh4x2u1},
{Trees, sh2x2J},
{Trees, sh3x2u1},
{Trees, sh2x2J},
{Trees, sh2x2J},
{Trees, sh2x2J},
{Trees, sh2x2J},
{Trees, sh2x2J},
{Lake, sh5x2r1},
{Lake, sh3x1},
{Lake, sh3x1},
{WindMill, sh4x3u2a3},
{ThatchedHut, sh2x2J},
{Obelisk, sh2x2J},
{Sign, sh2x1},
{AlchemyLab, sh4x2а6},
{Graveyard, sh3x1},
{WaterWheel, sh3x3u1r1a6},
{MagicWell, sh2x2J},
{EmpthyObject, sh4x2},
{Graveyard, sh4x2},
};
static mapitemi desert[] = {{EmpthyObject, sh2x1}, // Hole in Earth
{Trees, sh2x1}, // Tree
{Trees, sh2x2J}, // Tree
{Trees, sh2x2J}, // Tree
{Trees, sh2x2J}, // Tree
{Hill, sh3x1}, // Hill
{Hill, sh3x1}, // Hill
{Trees, sh4x1}, // Tree
{Trees, sh2x1}, // Tree
{Trees, sh2x1}, // Tree
{EmpthyObject, sh2x1},
{EmpthyObject, sh2x1},
{Cactus, sh1x1}, // Cactus
{Cactus, sh1x1}, // Cactus
{Cactus, sh2x1}, // Cactus
{Cactus, sh2x1}, // Cactuses
{Cactus, sh2x2J}, // Cactuses
{Cactus, sh1x1}, // Cactuses
{Cactus, sh2x1}, // Cactuses
{Cactus, sh2x2J}, // Cactuses
{Cactus, sh2x2J}, // Cactuses
{Cactus, sh1x1}, // Cactuses
{Cactus, sh2x1}, // Cactuses
{Cactus, sh2x1}, // Cactuses
{CampFire, sh2x1a6},
{EmpthyObject, sh1x1},
{DesertTent, sh3x2u1},
{EmpthyObject, sh2x2J},
{Pyramid, sh3x2},
{DeadSkeleton, sh2x1},
{Sphinx, sh3x2u1r1},
{CityDead, sh5x2},
{Excavation, sh3x1},
{Obelisk, sh2x2J},
{EmpthyObject, sh3x2u1},
{Hole, sh3x1},
{EmpthyObject, sh3x2u1},
{Sign, sh2x1},
{Graveyard, sh3x1},
{Mines, sh4x2},
};
static mapitemi water[] = {{Bottle, sh1x1a11},
{WaterChest, sh2x1a6},
{EmpthyObject, sh1x1a6}, // Error?
{EmpthyObject, sh3x2u1}, // Error?
{EmpthyObject, sh2x1a6}, // Woods
{WaterBoatStation, sh3x2a6}, // Map crafter woods
{EmpthyObject, sh3x1a6}, // Водросли
{EmpthyObject, sh2x1a6}, // Водросли
{EmpthyObject, sh1x1a6}, // Утопабщий
{Rock, sh3x2a15}, // Птицы
{EmpthyObject, sh2x1},
{EmpthyObject, sh2x1},
{EmpthyObject, sh2x1a6},
{EmpthyObject, sh3x2a3},
{ShipWreck, sh3x2u1a6},
};
static mapitemi water2[] = {{Rock, sh2x2J},
{ShipWreck, sh3x3r1a6},
{Boat, sh1x1},
};
static mapitemi swamp[] = {{WitchHut, sh3x3u2u2a6},
{Rock, sh2x1},
{EmpthyObject, sh3x1}, // Herbs
{Xanadu, sh5x3u2a6},
{Mushrooms, sh3x1},
{DiggingHole, sh1x1},
{Lake, sh7x3r1},
{Lake, sh4x2},
{Lake, sh4x3u1r1},
{Flowers, sh2x1},
{Flowers, sh3x1},
{Flowers, sh2x1},
{Flowers, sh4x1},
{Flowers, sh2x1},
{EmpthyObject, sh2x1},// Мох
{Sign, sh1x1},
{EmpthyObject, sh5x2u1}, // Заводь
{EmpthyObject, sh3x1}, // Заводь
{EmpthyObject, sh3x1}, // Заводь
{EmpthyObject, sh3x1}, // Заводь
{EmpthyObject, sh2x1}, // Кушинка
{Trees, sh3x3u1d1}, // Дерево
{Trees, sh3x2u1}, // Дерево
{Trees, sh3x2u1}, // Дерево
{EmpthyObject, sh2x1}, // Трава
{EmpthyObject, sh4x1}, // Трава
{EmpthyObject, sh3x1}, // Трава
{EmpthyObject, sh1x1}, // Трава
{EmpthyObject, sh2x1}, // Кувшинка
{EmpthyObject, sh2x1}, // Кувшинка
{EmpthyObject, sh1x1}, // Кувшинка
{EmpthyObject, sh2x1}, // Камыш
{EmpthyObject, sh2x1}, // Камыш
{EmpthyObject, sh2x1}, // Камыш (сверху)
{EmpthyObject, sh2x1}, // Лилии
{EmpthyObject, sh1x1}, // Лилии
{Rock, sh2x1},
{Rock, sh2x1},
{Rock, sh3x2u2},
{Rock, sh1x1},
{EmpthyObject, sh3x1}, // Кувшинка
{Obelisk, sh2x2J},
};
static mapitemi wasteland[] = {{ArtesianSpring, sh3x2u1},
{EmpthyObject, sh2x1}, // Hole
{Trees, sh3x2u1}, // Пальмы
{Cactus, sh2x2J},
{Brush, sh2x1},
{EmpthyObject, sh1x1}, // Череп рогатого скота
{Rock, sh1x1},
{Rock, sh3x2u1r1},
{Rock, sh3x1},
{Rock, sh3x2u1},
{Rock, sh2x1},
{Rock, sh3x1},
{Rock, sh3x1},
{Rock, sh3x1},
{Rock, sh2x1},
{Rock, sh2x2J},
{Rock, sh2x2J},
{Rock, sh2x2J},
{Rock, sh2x2J},
{EmpthyObject, sh2x1}, // Засохшая трава
{EmpthyObject, sh4x3}, // Трещина
{EmpthyObject, sh1x1}, // Digging
{EmpthyObject, sh1x1}, // Засохшая трава
{EmpthyObject, sh1x1}, // Засохшая трава
{Wagon, sh2x1},
{EmpthyObject, sh6x3u1a10},
{TrollBridge, sh4x2},
{EmpthyObject, sh3x2u2a10}, // Торговый порт
{EmpthyObject, sh4x2u1},
{EmpthyObject, sh3x2}, // Трещина
{EmpthyObject, sh2x3}, // Трещина
{EmpthyObject, sh3x1}, // Трещина
{Obelisk, sh2x2J},
{SawMill, sh4x2},
};
static mapitemi lava[] = {{EmpthyObject, sh2x1}, // Hole
{EmpthyObject, sh4x2}, // Куча камней
{EmpthyObject, sh2x1}, // Error???
{EmpthyObject, sh2x3}, // Stones
{EmpthyObject, sh2x2J}, // Stones and lava
{EmpthyObject, sh3x2u1}, // Stones and lava
{EmpthyObject, sh1x1}, // Digg
{Lake, sh6x2}, // Fire Lake
{Lake, sh3x2}, // Fire Lake
{Lake, sh4x2}, // Fire Lake
{EmpthyObject, sh2x2}, // Lava
{EmpthyObject, sh2x2J}, // Lava
{EmpthyObject, sh2x2}, // Lava
{EmpthyObject, sh3x2r1}, // Lava
{EmpthyObject, sh3x2r1}, // Lava
{Volcano, sh2x2},
{EmpthyObject, sh2x2Ja9}, // Steam
{Obelisk, sh2x2J},
{DemonCave, sh3x2u1},
{Sign, sh2x1},
{SawMill, sh4x2},
};
static mapitemi lava2[] = {{Volcano, sh5x3a6},
{Volcano, sh5x3a4},
};
static mapitemi lava3[] = {{Volcano, sh8x5a10},
};
static mapitemi trees[] = {{Trees, sh4x3r1d1},
{Trees, sh4x3u1b1},
{Trees, sh4x2r1d1},
{Trees, sh4x2u1b1},
{Trees, sh2x1},
{Trees, sh2x1},
};
static mapitemi mountains[] = {{Mountains, sh6x4r1d2},
{Mountains, sh6x4u1b2},
{Mountains, sh4x3r1d1},
{Mountains, sh4x3u1b1},
{Mountains, sh4x2r1d1},
{Mountains, sh4x2u1b1},
{Mines, sh5x2},
};
static mapitemi mountains2[] = {{Mountains, sh6x4r1d2},
{Mountains, sh6x4u1b2},
{Mountains, sh6x3r1d2},
{Mountains, sh6x3u1b2},
{Mountains, sh4x3r1d1},
{Mountains, sh4x3u1b1},
{Mountains, sh4x2r1d1},
{Mountains, sh4x2u1b1},
{Mines, sh5x2},
};
static mapitemi multiobj[] = {{Trees, sh2x2J},
{Trees, sh2x1},
{PeasantHut, sh3x2u2a9},
{Fort, sh4x3u2r1d1a9},
{Gazebo, sh2x2J},
{Flowers, sh3x1},
{DoctorHut, sh3x2u2},
{MercenaryCamp, sh3x1},
{Ruins, sh2x1},
{Shrine2, sh2x1},
{Shrine3, sh2x1},
{Shrine1, sh2x1},
{Idol, sh2x1},
{StandingStones, sh3x1},
{Temple, sh2x2},
{TradingPost, sh3x2u2a5v2},
{SpriteHouse, sh2x2J},
{ThatchedHut, sh2x1},
{TreeKnowledge, sh3x3u1u1},
{CampFire, sh2x1a6},
};
static mapitemi multiobj2[] = {{RiverDeltaDown, sh3x3u1r1},
{RiverDeltaDown, sh3x3d1b1},
{Fountain, sh2x1},
{Stumps, sh1x1},
{Stumps, sh2x1},
{Stumps, sh1x1},
{AlchemyLab, sh4x2а6},
{DragonCity, sh7x4},
{Graveyard, sh3x1}, // Not usable
{LitghHouse, sh3x3u2m1},
{SawMill, sh4x2}, // On brown dirt
{WaterWheel, sh3x3u1r1a6},
{Sign, sh2x1},
{Teleporter, sh2x1},
{Teleporter, sh2x2J},
{Teleporter, sh2x2J},
{RogueCamp, sh4x2а6_rogcmp},
{EmpthyObject, sh6x4}, // Error ??
{MagicWell, sh2x1},
{Event, sh1x1},
{MagicWell, sh2x1},
{FreemanFoundry, sh3x2u1a6v2},
{MagicGarden, sh2x1},
{MagicGarden, sh1x1a6}, // Lephrechaunt here
{WatchTower, sh3x2u2},
{Graveyard, sh4x2},
{SawMill, sh4x2}, // Green grass
};

enum landscape_s : unsigned char {
	Grass, Lava, Swamp, Sea, Waste, Desert, Dirt, Snow,
};

static struct mapobjectset {
	int			tile;
	res_s		icn;
	int			count;
	mapitemi*	objects;
} mapobjectsets[] = {{Grass, OBJNGRAS, sizeof(grass) / sizeof(grass[0]), grass},
{Grass, OBJNGRA2, sizeof(grass2) / sizeof(grass2[0]), grass2},
{Dirt, OBJNDIRT, sizeof(dirt) / sizeof(dirt[0]), dirt},
{Snow, OBJNSNOW, sizeof(snow) / sizeof(snow[0]), snow},
{Desert, OBJNDSRT, sizeof(desert) / sizeof(desert[0]), desert},
{Sea, OBJNWATR, sizeof(water) / sizeof(water[0]), water},
{Sea, OBJNWAT2, sizeof(water2) / sizeof(water2[0]), water2},
{Swamp, OBJNSWMP, sizeof(swamp) / sizeof(swamp[0]), swamp},
{Waste, OBJNCRCK, sizeof(wasteland) / sizeof(wasteland[0]), wasteland},
{Lava, OBJNLAVA, sizeof(lava) / sizeof(lava[0]), lava},
{Lava, OBJNLAV2, sizeof(lava2) / sizeof(lava2[0]), lava2},
{Lava, OBJNLAV3, sizeof(lava3) / sizeof(lava3[0]), lava3},
{Trees, TREDECI, sizeof(trees) / sizeof(trees[0]), trees},
{Trees, TREFALL, sizeof(trees) / sizeof(trees[0]), trees},
{Trees, TREJNGL, sizeof(trees) / sizeof(trees[0]), trees},
{Trees, TREEVIL, sizeof(trees) / sizeof(trees[0]), trees},
{Trees, TREFIR, sizeof(trees) / sizeof(trees[0]), trees},
{Trees, TRESNOW, sizeof(trees) / sizeof(trees[0]), trees},
{Mountains, MTNDIRT, sizeof(mountains2) / sizeof(mountains2[0]), mountains2},
{Mountains, MTNCRCK, sizeof(mountains2) / sizeof(mountains2[0]), mountains2},
{Mountains, MTNDSRT, sizeof(mountains) / sizeof(mountains[0]), mountains},
{Mountains, MTNGRAS, sizeof(mountains) / sizeof(mountains[0]), mountains},
{Mountains, MTNLAVA, sizeof(mountains) / sizeof(mountains[0]), mountains},
{Mountains, MTNMULT, sizeof(mountains) / sizeof(mountains[0]), mountains},
{Mountains, MTNSNOW, sizeof(mountains) / sizeof(mountains[0]), mountains},
{Mountains, MTNSWMP, sizeof(mountains) / sizeof(mountains[0]), mountains},
{EmpthyObject, OBJNMULT, sizeof(multiobj) / sizeof(multiobj[0]), multiobj},
{EmpthyObject, OBJNMUL2, sizeof(multiobj2) / sizeof(multiobj2[0]), multiobj2},
};

static void initialize_map_objects() {
	// Initialize shapes
	for(auto& ts : mapobjectsets) {
		int start_ex = 0;
		for(int i = 0; i < ts.count; i++) {
			if(ts.objects[i].shape.initialized)
				continue;
			auto& sh = ts.objects[i].shape;
			auto index = 0;
			auto mx = 0;
			auto my = 0;
			for(int i = 0; i < sh.count; i++) {
				auto x = sh.points[i].x;
				if(x < mx)
					mx = x;
				auto y = sh.points[i].y;
				if(y < my)
					my = y;
				sh.indecies[i] = index;
				if(sh.points[i].x == 0 && sh.points[i].y == 0)
					sh.zero = index;
				index += 1 + sh.animation[i];
			}
			sh.offset.x = mx;
			sh.offset.y = my;
			sh.initialized = 1;
		}
	}
	// Initialize map objects
	for(auto& ts : mapobjectsets) {
		int start_ex = 0;
		for(int i = 0; i < ts.count; i++) {
			ts.objects[i].first = start_ex;
			auto& sh = ts.objects[i].shape;
			ts.objects[i].last = ts.objects[i].first + sh.indecies[sh.count - 1] + sh.animation[sh.count - 1];
			start_ex += sh.indecies[sh.count - 1] + 1 + sh.animation[sh.count - 1];
		}
	}
}

static mapitemi* find_object(res_s icn, unsigned char frame) {
	for(auto& ts : mapobjectsets) {
		if(ts.icn != icn)
			continue;
		for(int i = 0; i < ts.count; i++) {
			if(ts.objects[i].first <= frame && frame <= ts.objects[i].last)
				return ts.objects + i;
		}
	}
	return 0;
}

//void tilesetsview() {
//	auto index = 0;
//	auto ts_index = 0;
//	char temp[64];
//	while(true) {
//		auto x1 = 640 / 2 - 16;
//		auto y1 = 480 / 2 - 16;
//		if(ts_index < 0)
//			ts_index = 0;
//		else if(ts_index >= sizeof(mapobjectsets) / sizeof(mapobjectsets[0]))
//			ts_index = sizeof(mapobjectsets) / sizeof(mapobjectsets[0]) - 1;
//		auto& ts = mapobjectsets[ts_index];
//		if(index > ts.count - 1)
//			index = ts.count - 1;
//		else if(index < 0)
//			index = 0;
//		auto icn = ts.icn;
//		auto& e = ts.objects[index];
//		auto& sh = e.shape;
//		draw::rectf({0, 0, draw::width - 1, draw::height - 1}, 0x12);
//		int e_count = sh.indecies[sh.count - 1] + 1 + sh.animation[sh.count - 1];
//		szprint(temp, "object %1i/%5i (start=%2i, count=%3i, next=%4i)", index, e.first, e_count, e.first + e_count, ts.count);
//		draw::text(0, 0, temp);
//		point center = {0, 0};
//		for(int i = 0; i < sh.count; i++) {
//			auto px = x1 + sh.points[i].x * 32;
//			auto py = y1 + sh.points[i].y * 32;
//			auto frame = e.first + sh.indecies[i];
//			if(sh.animation[i])
//				draw::image(px, py, icn, frame + 1 + (draw::counter % sh.animation[i]));
//			draw::image(px, py, icn, frame);
//			if(sh.points[i].x == 0 && sh.points[i].y == 0) {
//				center.x = px;
//				center.y = py;
//			}
//			draw::rectb(px, py, px + 32, py + 32, 0xC);
//			if(sh.content[i] == TypeBlock)
//				draw::rectb(px + 1, py + 1, px + 32 - 1, py + 32 - 1, 0xBF);
//		}
//		int rx1 = x1 + sh.offset.x * 32;
//		int ry1 = y1 + sh.offset.y * 32;
//		//draw::rectb(rx1, ry1, rx1 + sh.size.x * 32, ry1 + sh.size.y * 32, 0xBF);
//		draw::cursor(res::ADVMCO, 0);
//		draw::rectb(center.x + 2, center.y + 2, center.x + 32 - 2, center.y + 32 - 2, 0x72);
//		int id = draw::input();
//		switch(id) {
//		case KeyEscape:
//		case Cancel:
//			return;
//		case KeyRight:
//			index++;
//			break;
//		case KeyLeft:
//			index--;
//			break;
//		case KeyUp:
//			ts_index++;
//			break;
//		case KeyDown:
//			ts_index--;
//			break;
//		}
//	}
//}

//struct mapobject : public drawable {
//
//	res::tokens		icn;
//	tokens			type;
//	short unsigned	index;
//	short unsigned	count;
//	mapobjectinfo*	info;
//
//	int getid() const override;
//
//	point getpos() const {
//		auto x = map::i2x(index) * 32;
//		auto y = map::i2y(index) * 32;
//		if(type >= FirstMonster && type <= LastMonster) {
//			x += 16;
//			y += 30;
//		}
//		return{(short)x, (short)y};
//	}
//
//	rect getrect() const override {
//		auto pt = getpos();
//		if(info) {
//			short x1 = pt.x + info->shape.offset.x * 32;
//			short y1 = pt.y + info->shape.offset.y * 32;
//			return{x1, y1, x1 + info->shape.size.x * 32, y1 + info->shape.size.y * 32};
//		} else
//			return{pt.x - 32, pt.y - 32, pt.x + 32, pt.y + 32};
//	}
//
//	point getzpos() const override {
//		return getpos();
//	}
//
//	void painting(point camera, unsigned flags) const override {
//		auto pt = getpos() - camera;
//		if(info) {
//			auto& sh = info->shape;
//			for(int i = 0; i < sh.count; i++) {
//				auto px = pt.x + sh.points[i].x * 32;
//				auto py = pt.y + sh.points[i].y * 32;
//				auto frame = info->first + sh.indecies[i];
//				if(sh.animation[i])
//					draw::image(px, py, icn, frame + 1 + ((draw::counter + count) % sh.animation[i]));
//				draw::image(px, py, icn, frame);
//			}
//			switch(type) {
//			case MineOre:
//			case MineGold:
//			case MineSulfur:
//			case MineCrystal:
//			case MineGems:
//				draw::image(pt.x, pt.y, res::EXTRAOVR, type - FirstMine);
//				if(count)
//					draw::image(pt.x + 6, pt.y - 26, res::FLAG32, count - FirstPlayer);
//				break;
//			case SawMill:
//				if(count)
//					draw::image(pt.x + 12, pt.y - 48, res::FLAG32, count - FirstPlayer);
//				break;
//			}
//		} else if(type >= FirstResource && type <= LastResource) {
//			static int decode_resource[LastResource - FirstResource + 1] = {13 / 2, 1 / 2, 3 / 2, 5 / 2, 7 / 2, 9 / 2, 11 / 2};
//			auto id = decode_resource[type - FirstResource];
//			draw::image(pt.x - 32, pt.y, res::OBJNRSRC, id * 2);
//			draw::image(pt.x, pt.y, res::OBJNRSRC, id * 2 + 1);
//		} else if(type >= FirstMonster && type <= LastMonster) {
//			if(type >= MonsterRnd1)
//				draw::image(pt.x, pt.y, res::MONS32, type - FirstMonster);
//			else {
//				draw::image(pt.x, pt.y, res::MINIMON, (type - FirstMonster) * 9);
//				switch(bsget(type, AnimationType)) {
//				case 1:
//					draw::image(pt.x, pt.y, res::MINIMON, (type - FirstMonster) * 9
//						+ 1 + monster_animation_cicl1[(draw::counter + index * index) % (sizeof(monster_animation_cicl1) / sizeof(monster_animation_cicl1[0]))]);
//					break;
//				default:
//					draw::image(pt.x, pt.y, res::MINIMON, (type - FirstMonster) * 9
//						+ 1 + monster_animation_cicle[(draw::counter + index * index) % (sizeof(monster_animation_cicle) / sizeof(monster_animation_cicle[0]))]);
//					break;
//				}
//			}
//		} else if(type >= FirstArtifact && type <= LastArtifact) {
//			draw::image(pt.x - 32, pt.y, res::OBJNARTI, (type - FirstArtifact) * 2);
//			draw::image(pt.x, pt.y, res::OBJNARTI, (type - FirstArtifact) * 2 + 1);
//		} else if(type == TreasureChest) {
//			draw::image(pt.x - 32, pt.y, res::OBJNRSRC, 18);
//			draw::image(pt.x, pt.y, res::OBJNRSRC, 19);
//		} else if(type == AncientLamp) {
//			draw::image(pt.x - 32, pt.y, res::OBJNRSRC, 14);
//			draw::image(pt.x, pt.y, res::OBJNRSRC, 15);
//		} else
//			draw::image(pt.x, pt.y, icn, count);
//	}
//
//	int priority() const override {
//		return getid();
//	}
//
//};

//static unsigned char getroad(unsigned char object, unsigned char index) {
//	switch(res::map(object)) {
//		// from sprite road
//	case res::ROAD:
//		if(0 == index || 4 == index || 5 == index || 13 == index || 26 == index)
//			return map::Up | map::Down;
//		else if(2 == index || 21 == index || 28 == index)
//			return map::Left | map::Right;
//		else if(17 == index || 29 == index)
//			return map::LeftUp | map::RightDown;
//		else if(18 == index || 30 == index)
//			return map::RightUp | map::LeftDown;
//		else if(3 == index)
//			return map::Up | map::Down | map::Left | map::Right;
//		else if(6 == index)
//			return map::Up | map::Down | map::Right;
//		else if(7 == index)
//			return map::Up | map::Right;
//		else if(9 == index)
//			return map::Down | map::Right;
//		else if(12 == index)
//			return map::Down | map::Left;
//		else if(14 == index)
//			return map::Up | map::Down | map::Left;
//		else if(16 == index)
//			return map::Up | map::Left;
//		else if(19 == index)
//			return map::LeftUp | map::RightDown;
//		else if(20 == index)
//			return map::RightUp | map::LeftDown;
//		return 0;
//		// castle and tower (gate)
//	case res::OBJNTOWN:
//		if(13 == index ||
//			29 == index ||
//			45 == index ||
//			61 == index ||
//			77 == index ||
//			93 == index ||
//			109 == index ||
//			125 == index ||
//			141 == index ||
//			157 == index ||
//			173 == index ||
//			189 == index)
//			return map::Up | map::Down;
//		return 0;
//		// castle lands (gate)
//	case res::OBJNTWBA:
//		if(7 == index ||
//			17 == index ||
//			27 == index ||
//			37 == index ||
//			47 == index ||
//			57 == index ||
//			67 == index ||
//			77 == index)
//			return map::Up | map::Down;
//		return 0;
//	default:
//		return 0;
//	}
//}

//void add_object(unsigned short index, unsigned char object, unsigned char frame, unsigned char quantity) {
//	static mapobject* last_object = 0;
//	tokens type = Empthy;
//	mapitemi* pi = 0;
//	auto icn = res::map(object);
//	switch(icn) {
//	case res::OBJNTOWN: // Не будем добавлять города
//	case res::OBJNTWBA: // Не будем добавлять базу городов
//	case res::OBJNTWRD: // No random towns and castles
//		map::show::road[index] = getroad(object, frame);
//		return;
//	case res::MINIHERO: // No heroes
//	case res::SHADOW32: // No heroes shadows
//	case res::FLAG32: // No player flags
//	case res::OBJNTWSH: // No towns and castles shadow
//	case res::OBJNARTI: // No artifacts tiles
//	case res::MONS32: // No monster images
//	case res::OBJNRSRC: // No resource images
//		return;
//	case res::OBJNMUL2:
//		if(frame == 163) // No event signal (used only in editor)
//			return;
//		pi = find_object(icn, frame);
//		break;
//	case res::EXTRAOVR:
//		if(last_object) {
//			// Abandone mine and Mountain Mines has overlay just after their objects
//			assert((last_object->type == Mines) || (last_object->type == AbandoneMine));
//			last_object->type = (tokens)(MineOre + frame);
//			last_object->count = 0;
//		}
//		return;
//	case res::STREAM:
//		type = Stream;
//		quantity = frame;
//		break;
//	case res::ROAD:
//		type = Road;
//		quantity = frame;
//		map::show::road[index] = getroad(object, frame);
//		break;
//	default:
//		pi = find_object(icn, frame);
//		assert(pi);
//		break;
//	}
//	if(pi) {
//		// Skip all frame, tha are not zero point.
//		if((pi->first + pi->shape.zero) != frame)
//			return;
//		type = pi->object;
//		if(!type)
//			type = Resource;
//	}
//	// А можно ли добавить новый объект?
//	if(mapobjects.from + mapobjects.count >= mapobjects.to)
//		return;
//	auto& e = objects[mapobjects.count++];
//	e.icn = icn;
//	e.index = index;
//	e.count = quantity;
//	e.info = pi;
//	e.type = type;
//	last_object = &e;
//}

//COMMAND(map_block) {
//	// Другие объекты
//	for(int i = 0; i < mapobjects.count; i++) {
//		auto& e = objects[i];
//		switch(e.type) {
//			// Non-Blocking
//		case Cliff:
//		case Crack:
//		case RiverDeltaDown:
//		case RiverDeltaUp:
//		case Empthy:
//			continue;
//		}
//		if(e.info) {
//			auto& sh = e.info->shape;
//			int x = map::i2x(e.index);
//			int y = map::i2y(e.index);
//			for(int index = 0; index < sh.count; index++) {
//				int x1 = x + sh.points[index].x;
//				int y1 = y + sh.points[index].y;
//				if(x1 < 0 || y1 < 0 || x1 >= map::width || y1 >= map::height)
//					continue;
//				auto i = map::m2i(x1, y1);
//				if(sh.content[index] == TypeBlock)
//					map::show::type[i] = TypeBlock;
//			}
//		}
//		if((e.type >= FirstObject && e.type <= LastObject)
//			|| (e.type >= FirstMine && e.type <= LastMine)
//			|| (e.type >= FirstResource && e.type <= LastResource)
//			|| (e.type >= FirstArtifact && e.type <= LastArtifact)
//			|| (e.type == TreasureChest || e.type == AncientLamp))
//			map::show::type[e.index] = TypeAction;
//	}
//	// Пройдемся по монстрам
//	for(int i = 0; i < mapobjects.count; i++) {
//		auto& e = objects[i];
//		if(!e.icn)
//			continue;
//		if(e.type >= FirstMonster && e.type <= LastMonster) {
//			int x = map::i2x(e.index);
//			int y = map::i2y(e.index);
//			for(int y1 = y - 1; y1 <= y + 1; y1++) {
//				if(y1 < 0 || y1 >= map::height)
//					continue;
//				for(int x1 = x - 1; x1 <= x + 1; x1++) {
//					if(x1 < 0 || x1 >= map::width)
//						continue;
//					int i = map::m2i(x1, y1);
//					if(map::show::type[i] == 2)
//						continue;
//					map::show::type[i] = 4;
//				}
//			}
//		}
//	}
//}
//
//int map::getobject(short unsigned index) {
//	for(int i = 0; i < mapobjects.count; i++) {
//		if(objects[i].index == index) {
//			if(objects[i].type >= Volcano)
//				continue;
//			return FirstMapObject + i;
//		}
//	}
//	return 0;
//}
//
//static struct mapobject_drawable_plugin : public drawable::plugin {
//	void selecting(drawable** result, rect screen, unsigned flags) override {
//		if((flags&DWMask) != DWObjects)
//			return;
//		auto p = result;
//		for(int i = 0; i < mapobjects.count; i++) {
//			if(!objects[i].type)
//				continue;
//			if(!objects[i].getrect().intersect(screen))
//				continue;
//			*p++ = objects + i;
//		}
//		*p = 0;
//	}
//} drawable_plugin;