#include "view.h"

using namespace draw;

//
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
static shapei sh3x3tr = {7, {3, 3}, {{-1, -2}, {0, -2}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {}, {0, 0, 0, 1, 0, 0, 2}};
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
static shapei sh4x2sm = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0}, {0, 1, 1, 1, 2, 2, 2, 2}};
static shapei sh4x2gr = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0}, {0, 1, 1, 1, 0, 2, 2, 2}};
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

drawobji bsmeta<drawobji>::elements[] = {{OBJNGRAS, AbandoneMine, sh4x2}, // Haunted mine
{OBJNGRAS, Hole, sh2x1bk},
{OBJNGRAS, Crack, sh4x2d1},
{OBJNGRAS, Crack, sh3x3},
{OBJNGRAS, Crack, sh2x2bk},
{OBJNGRAS, Rock, sh3x1},
{OBJNGRAS, Rock, sh3x1},
{OBJNGRAS, Rock, sh3x2u1r1}, // Big boulder
{OBJNGRAS, Rock, sh2x1}, // Big 
{OBJNGRAS, Rock, sh1x1},
{OBJNGRAS, Rock, sh2x1},
{OBJNGRAS, Rock, sh2x1},
{OBJNGRAS, Rock, sh2x1},
{OBJNGRAS, Rock, sh2x1},
{OBJNGRAS, Lake, sh5x3},
{OBJNGRAS, Flowers, sh1x1}, // Flower
{OBJNGRAS, Lake, sh4x2}, // Lake
{OBJNGRAS, Lake, sh3x1}, // Lake
{OBJNGRAS, Hill, sh3x1}, // Hill
{OBJNGRAS, Trees, sh4x2u1},
{OBJNGRAS, Trees, sh3x2u1},
{OBJNGRAS, Trees, sh2x2J},
{OBJNGRAS, Brush, sh4x1}, // Brush flowers
{OBJNGRAS, Brush, sh4x1}, // Brush
{OBJNGRAS, Brush, sh3x1}, // Brush small
{OBJNGRAS, Brush, sh3x1}, // Brush small flowers
{OBJNGRAS, Brush, sh3x1}, // Brush small to up
{OBJNGRAS, Brush, sh2x1}, // Single brush
{OBJNGRAS, Flowers, sh4x1}, // Flowers red
{OBJNGRAS, Flowers, sh4x2u1}, // Flowers red
{OBJNGRAS, Flowers, sh4x1}, // Flowers red another
{OBJNGRAS, Flowers, sh4x1}, // Flowers blue
{OBJNGRAS, Flowers, sh3x2u1}, // Flowers blue
{OBJNGRAS, Flowers, sh1x1}, // Flowers tiny
{OBJNGRAS, Flowers, sh3x1}, // Flowers
{OBJNGRAS, Flowers, sh2x1}, // Flowers
{OBJNGRAS, Flowers, sh2x1}, // Flowers to up
{OBJNGRAS, Flowers, sh2x1}, // Flowers white
{OBJNGRAS, Flowers, sh2x1}, // Flowers ultraviolet
{OBJNGRAS, Hill, sh2x1}, // Hill
{OBJNGRA2, HillFort, sh3x2u1},
{OBJNGRA2, HalflingHole, sh4x1},
{OBJNGRA2, DiggingHole, sh1x1},
{OBJNGRA2, Cliff, sh2x1},
{OBJNGRA2, Cliff, sh2x1},
{OBJNGRA2, Cliff, sh3x1},
{OBJNGRA2, SpriteHouse, sh4x2u2},
{OBJNGRA2, WindMill, sh4x3u2a3},
{OBJNGRA2, ArcherHouse, sh3x2u2a5},
{OBJNGRA2, GoblinHut, sh2x1},
{OBJNGRA2, PeasantHut, sh3x2u2a5},
{OBJNGRA2, Oracle, sh3x2},
{OBJNGRA2, Obelisk, sh2x2J},
{OBJNDIRT, AbandoneMine, sh5x2},
{OBJNDIRT, Hole, sh2x1},
{OBJNDIRT, Hill, sh2x1},
{OBJNDIRT, Hill, sh3x1},
{OBJNDIRT, Crack, sh2x1},
{OBJNDIRT, Crack, sh2x1},
{OBJNDIRT, Crack, sh2x1},
{OBJNDIRT, Lake, sh8x3},
{OBJNDIRT, Lake, sh5x2},
{OBJNDIRT, Lake, sh2x1},
{OBJNDIRT, Brush, sh3x1},
{OBJNDIRT, Brush, sh3x1},
{OBJNDIRT, Brush, sh3x1},
{OBJNDIRT, Brush, sh2x1},
{OBJNDIRT, Brush, sh2x1},
{OBJNDIRT, Flowers, sh3x1},
{OBJNDIRT, Flowers, sh3x1},
{OBJNDIRT, Flowers, sh3x1},
{OBJNDIRT, Flowers, sh3x1},
{OBJNDIRT, Flowers, sh2x1},
{OBJNDIRT, Flowers, sh1x1},
{OBJNDIRT, Flowers, sh2x1},
{OBJNDIRT, Flowers, sh1x1},
{OBJNDIRT, Flowers, sh1x1},
{OBJNDIRT, Rock, sh3x1},
{OBJNDIRT, Rock, sh3x2},
{OBJNDIRT, Rock, sh3x1},
{OBJNDIRT, Rock, sh2x1},
{OBJNDIRT, Rock, sh1x1},
{OBJNDIRT, Flowers, sh1x1},
{OBJNDIRT, Flowers, sh2x1},
{OBJNDIRT, Flowers, sh2x1},
{OBJNDIRT, EmpthyObject, sh2x1},
{OBJNDIRT, EmpthyObject, sh1x1},
{OBJNDIRT, Trees, sh3x2},
{OBJNDIRT, Trees, sh3x2u1},
{OBJNDIRT, Trees, sh2x2J},
{OBJNDIRT, Mushrooms, sh3x1},
{OBJNDIRT, HillFort, sh3x2u1},
{OBJNDIRT, HalflingHole, sh4x1},
{OBJNDIRT, DiggingHole, sh1x1},
{OBJNDIRT, Cliff, sh2x1},
{OBJNDIRT, Cliff, sh2x1},
{OBJNDIRT, Cliff, sh2x1},
{OBJNDIRT, SpriteCity, sh4x2u2},
{OBJNDIRT, WindMill, sh4x3u2a3},
{OBJNDIRT, Oracle, sh3x2},
{OBJNDIRT, Obelisk, sh2x2J},
{OBJNSNOW, Hole, sh2x1}, // Hole in Earth
{OBJNSNOW, CentautCave, sh2x1},
{OBJNSNOW, CampFire, sh1x1a6},
{OBJNSNOW, DiggingHole, sh1x1}, // Digging
{OBJNSNOW, LeanTo, sh2x1},
{OBJNSNOW, Cliff, sh2x1},
{OBJNSNOW, Cliff, sh2x1},
{OBJNSNOW, Cliff, sh3x1},
{OBJNSNOW, Rock, sh2x1},
{OBJNSNOW, Rock, sh3x2u1},
{OBJNSNOW, Rock, sh1x1},
{OBJNSNOW, Rock, sh2x1},
{OBJNSNOW, Rock, sh2x1},
{OBJNSNOW, Rock, sh3x1},
{OBJNSNOW, Rock, sh2x1},
{OBJNSNOW, Rock, sh2x1},
{OBJNSNOW, EmpthyObject, sh2x1}, // Wood (пеньки)
{OBJNSNOW, EmpthyObject, sh1x1}, // Wood (пеньки)
{OBJNSNOW, EmpthyObject, sh1x1}, // Grass
{OBJNSNOW, EmpthyObject, sh1x1}, // Grass
{OBJNSNOW, EmpthyObject, sh1x1}, // Grass
{OBJNSNOW, Trees, sh3x2u1},
{OBJNSNOW, Trees, sh4x2u1},
{OBJNSNOW, Trees, sh2x2J},
{OBJNSNOW, Trees, sh3x2u1},
{OBJNSNOW, Trees, sh2x2J},
{OBJNSNOW, Trees, sh2x2J},
{OBJNSNOW, Trees, sh2x2J},
{OBJNSNOW, Trees, sh2x2J},
{OBJNSNOW, Trees, sh2x2J},
{OBJNSNOW, Lake, sh5x2r1},
{OBJNSNOW, Lake, sh3x1},
{OBJNSNOW, Lake, sh3x1},
{OBJNSNOW, WindMill, sh4x3u2a3},
{OBJNSNOW, ThatchedHut, sh2x2J},
{OBJNSNOW, Obelisk, sh2x2J},
{OBJNSNOW, Sign, sh2x1},
{OBJNSNOW, AlchemyLab, sh4x2а6},
{OBJNSNOW, Graveyard, sh3x1},
{OBJNSNOW, WaterWheel, sh3x3u1r1a6},
{OBJNSNOW, MagicWell, sh2x2J},
{OBJNSNOW, SawMill, sh4x2sm},
{OBJNSNOW, Graveyard, sh4x2gr},
{OBJNDSRT, EmpthyObject, sh2x1}, // Hole in Earth
{OBJNDSRT, Trees, sh2x1}, // Tree
{OBJNDSRT, Trees, sh2x2J}, // Tree
{OBJNDSRT, Trees, sh2x2J}, // Tree
{OBJNDSRT, Trees, sh2x2J}, // Tree
{OBJNDSRT, Hill, sh3x1}, // Hill
{OBJNDSRT, Hill, sh3x1}, // Hill
{OBJNDSRT, Trees, sh4x1}, // Tree
{OBJNDSRT, Trees, sh2x1}, // Tree
{OBJNDSRT, Trees, sh2x1}, // Tree
{OBJNDSRT, EmpthyObject, sh2x1},
{OBJNDSRT, EmpthyObject, sh2x1},
{OBJNDSRT, Cactus, sh1x1}, // Cactus
{OBJNDSRT, Cactus, sh1x1}, // Cactus
{OBJNDSRT, Cactus, sh2x1}, // Cactus
{OBJNDSRT, Cactus, sh2x1}, // Cactuses
{OBJNDSRT, Cactus, sh2x2J}, // Cactuses
{OBJNDSRT, Cactus, sh1x1}, // Cactuses
{OBJNDSRT, Cactus, sh2x1}, // Cactuses
{OBJNDSRT, Cactus, sh2x2J}, // Cactuses
{OBJNDSRT, Cactus, sh2x2J}, // Cactuses
{OBJNDSRT, Cactus, sh1x1}, // Cactuses
{OBJNDSRT, Cactus, sh2x1}, // Cactuses
{OBJNDSRT, Cactus, sh2x1}, // Cactuses
{OBJNDSRT, CampFire, sh2x1a6},
{OBJNDSRT, EmpthyObject, sh1x1},
{OBJNDSRT, DesertTent, sh3x2u1},
{OBJNDSRT, EmpthyObject, sh2x2J},
{OBJNDSRT, Pyramid, sh3x2},
{OBJNDSRT, DeadSkeleton, sh2x1},
{OBJNDSRT, Sphinx, sh3x2u1r1},
{OBJNDSRT, CityDead, sh5x2},
{OBJNDSRT, Excavation, sh3x1},
{OBJNDSRT, Obelisk, sh2x2J},
{OBJNDSRT, EmpthyObject, sh3x2u1},
{OBJNDSRT, Hole, sh3x1},
{OBJNDSRT, EmpthyObject, sh3x2u1},
{OBJNDSRT, Sign, sh2x1},
{OBJNDSRT, Graveyard, sh3x1},
{OBJNDSRT, Mines, sh4x2},
{OBJNWATR, Bottle, sh1x1a11},
{OBJNWATR, WaterChest, sh2x1a6},
{OBJNWATR, EmpthyObject, sh1x1a6}, // Error?
{OBJNWATR, EmpthyObject, sh3x2u1}, // Error?
{OBJNWATR, EmpthyObject, sh2x1a6}, // Woods
{OBJNWATR, WaterBoatStation, sh3x2a6}, // Map crafter woods
{OBJNWATR, EmpthyObject, sh3x1a6}, // Водросли
{OBJNWATR, EmpthyObject, sh2x1a6}, // Водросли
{OBJNWATR, EmpthyObject, sh1x1a6}, // Утопабщий
{OBJNWATR, Rock, sh3x2a15}, // Птицы
{OBJNWATR, EmpthyObject, sh2x1},
{OBJNWATR, EmpthyObject, sh2x1},
{OBJNWATR, EmpthyObject, sh2x1a6},
{OBJNWATR, EmpthyObject, sh3x2a3},
{OBJNWATR, ShipWreck, sh3x2u1a6},
{OBJNWAT2, Rock, sh2x2J},
{OBJNWAT2, ShipWreck, sh3x3r1a6},
{OBJNWAT2, Boat, sh1x1},
{OBJNSWMP, WitchHut, sh3x3u2u2a6},
{OBJNSWMP, Rock, sh2x1},
{OBJNSWMP, EmpthyObject, sh3x1}, // Herbs
{OBJNSWMP, Xanadu, sh5x3u2a6},
{OBJNSWMP, Mushrooms, sh3x1},
{OBJNSWMP, DiggingHole, sh1x1},
{OBJNSWMP, Lake, sh7x3r1},
{OBJNSWMP, Lake, sh4x2},
{OBJNSWMP, Lake, sh4x3u1r1},
{OBJNSWMP, Flowers, sh2x1},
{OBJNSWMP, Flowers, sh3x1},
{OBJNSWMP, Flowers, sh2x1},
{OBJNSWMP, Flowers, sh4x1},
{OBJNSWMP, Flowers, sh2x1},
{OBJNSWMP, EmpthyObject, sh2x1},// Мох
{OBJNSWMP, Sign, sh1x1},
{OBJNSWMP, EmpthyObject, sh5x2u1}, // Заводь
{OBJNSWMP, EmpthyObject, sh3x1}, // Заводь
{OBJNSWMP, EmpthyObject, sh3x1}, // Заводь
{OBJNSWMP, EmpthyObject, sh3x1}, // Заводь
{OBJNSWMP, EmpthyObject, sh2x1}, // Кушинка
{OBJNSWMP, Trees, sh3x3u1d1}, // Дерево
{OBJNSWMP, Trees, sh3x2u1}, // Дерево
{OBJNSWMP, Trees, sh3x2u1}, // Дерево
{OBJNSWMP, EmpthyObject, sh2x1}, // Трава
{OBJNSWMP, EmpthyObject, sh4x1}, // Трава
{OBJNSWMP, EmpthyObject, sh3x1}, // Трава
{OBJNSWMP, EmpthyObject, sh1x1}, // Трава
{OBJNSWMP, EmpthyObject, sh2x1}, // Кувшинка
{OBJNSWMP, EmpthyObject, sh2x1}, // Кувшинка
{OBJNSWMP, EmpthyObject, sh1x1}, // Кувшинка
{OBJNSWMP, EmpthyObject, sh2x1}, // Камыш
{OBJNSWMP, EmpthyObject, sh2x1}, // Камыш
{OBJNSWMP, EmpthyObject, sh2x1}, // Камыш (сверху)
{OBJNSWMP, EmpthyObject, sh2x1}, // Лилии
{OBJNSWMP, EmpthyObject, sh1x1}, // Лилии
{OBJNSWMP, Rock, sh2x1},
{OBJNSWMP, Rock, sh2x1},
{OBJNSWMP, Rock, sh3x2u2},
{OBJNSWMP, Rock, sh1x1},
{OBJNSWMP, EmpthyObject, sh3x1}, // Кувшинка
{OBJNSWMP, Obelisk, sh2x2J},
{OBJNCRCK, ArtesianSpring, sh3x2u1},
{OBJNCRCK, EmpthyObject, sh2x1}, // Hole
{OBJNCRCK, Trees, sh3x2u1}, // Пальмы
{OBJNCRCK, Cactus, sh2x2J},
{OBJNCRCK, Brush, sh2x1},
{OBJNCRCK, EmpthyObject, sh1x1}, // Череп рогатого скота
{OBJNCRCK, Rock, sh1x1},
{OBJNCRCK, Rock, sh3x2u1r1},
{OBJNCRCK, Rock, sh3x1},
{OBJNCRCK, Rock, sh3x2u1},
{OBJNCRCK, Rock, sh2x1},
{OBJNCRCK, Rock, sh3x1},
{OBJNCRCK, Rock, sh3x1},
{OBJNCRCK, Rock, sh3x1},
{OBJNCRCK, Rock, sh2x1},
{OBJNCRCK, Rock, sh2x2J},
{OBJNCRCK, Rock, sh2x2J},
{OBJNCRCK, Rock, sh2x2J},
{OBJNCRCK, Rock, sh2x2J},
{OBJNCRCK, EmpthyObject, sh2x1}, // Засохшая трава
{OBJNCRCK, EmpthyObject, sh4x3}, // Трещина
{OBJNCRCK, EmpthyObject, sh1x1}, // Digging
{OBJNCRCK, EmpthyObject, sh1x1}, // Засохшая трава
{OBJNCRCK, EmpthyObject, sh1x1}, // Засохшая трава
{OBJNCRCK, Wagon, sh2x1},
{OBJNCRCK, EmpthyObject, sh6x3u1a10},
{OBJNCRCK, TrollBridge, sh4x2},
{OBJNCRCK, EmpthyObject, sh3x2u2a10}, // Торговый порт
{OBJNCRCK, EmpthyObject, sh4x2u1},
{OBJNCRCK, EmpthyObject, sh3x2}, // Трещина
{OBJNCRCK, EmpthyObject, sh2x3}, // Трещина
{OBJNCRCK, EmpthyObject, sh3x1}, // Трещина
{OBJNCRCK, Obelisk, sh2x2J},
{OBJNCRCK, SawMill, sh4x2sm},
{OBJNLAVA, EmpthyObject, sh2x1}, // Hole
{OBJNLAVA, EmpthyObject, sh4x2}, // Куча камней
{OBJNLAVA, EmpthyObject, sh2x1}, // Error???
{OBJNLAVA, EmpthyObject, sh2x3}, // Stones
{OBJNLAVA, EmpthyObject, sh2x2J}, // Stones and lava
{OBJNLAVA, EmpthyObject, sh3x2u1}, // Stones and lava
{OBJNLAVA, EmpthyObject, sh1x1}, // Digg
{OBJNLAVA, Lake, sh6x2}, // Fire Lake
{OBJNLAVA, Lake, sh3x2}, // Fire Lake
{OBJNLAVA, Lake, sh4x2}, // Fire Lake
{OBJNLAVA, EmpthyObject, sh2x2}, // Lava
{OBJNLAVA, EmpthyObject, sh2x2J}, // Lava
{OBJNLAVA, EmpthyObject, sh2x2}, // Lava
{OBJNLAVA, EmpthyObject, sh3x2r1}, // Lava
{OBJNLAVA, EmpthyObject, sh3x2r1}, // Lava
{OBJNLAVA, Volcano, sh2x2},
{OBJNLAVA, EmpthyObject, sh2x2Ja9}, // Steam
{OBJNLAVA, Obelisk, sh2x2J},
{OBJNLAVA, DemonCave, sh3x2u1},
{OBJNLAVA, Sign, sh2x1},
{OBJNLAVA, SawMill, sh4x2sm},
{OBJNLAV2, Volcano, sh5x3a6},
{OBJNLAV2, Volcano, sh5x3a4},
{OBJNLAV3, Volcano, sh8x5a10},
{TREDECI, Trees, sh4x3r1d1},
{TREDECI, Trees, sh4x3u1b1},
{TREDECI, Trees, sh4x2r1d1},
{TREDECI, Trees, sh4x2u1b1},
{TREDECI, Trees, sh2x1},
{TREDECI, Trees, sh2x1},
{TREFALL, Trees, sh4x3r1d1},
{TREFALL, Trees, sh4x3u1b1},
{TREFALL, Trees, sh4x2r1d1},
{TREFALL, Trees, sh4x2u1b1},
{TREFALL, Trees, sh2x1},
{TREFALL, Trees, sh2x1},
{TREJNGL, Trees, sh4x3r1d1},
{TREJNGL, Trees, sh4x3u1b1},
{TREJNGL, Trees, sh4x2r1d1},
{TREJNGL, Trees, sh4x2u1b1},
{TREJNGL, Trees, sh2x1},
{TREJNGL, Trees, sh2x1},
{TREEVIL, Trees, sh4x3r1d1},
{TREEVIL, Trees, sh4x3u1b1},
{TREEVIL, Trees, sh4x2r1d1},
{TREEVIL, Trees, sh4x2u1b1},
{TREEVIL, Trees, sh2x1},
{TREEVIL, Trees, sh2x1},
{TREFIR, Trees, sh4x3r1d1},
{TREFIR, Trees, sh4x3u1b1},
{TREFIR, Trees, sh4x2r1d1},
{TREFIR, Trees, sh4x2u1b1},
{TREFIR, Trees, sh2x1},
{TREFIR, Trees, sh2x1},
{TRESNOW, Trees, sh4x3r1d1},
{TRESNOW, Trees, sh4x3u1b1},
{TRESNOW, Trees, sh4x2r1d1},
{TRESNOW, Trees, sh4x2u1b1},
{TRESNOW, Trees, sh2x1},
{TRESNOW, Trees, sh2x1},
{MTNDIRT, Mountains, sh6x4r1d2},
{MTNDIRT, Mountains, sh6x4u1b2},
{MTNDIRT, Mountains, sh6x3r1d2},
{MTNDIRT, Mountains, sh6x3u1b2},
{MTNDIRT, Mountains, sh4x3r1d1},
{MTNDIRT, Mountains, sh4x3u1b1},
{MTNDIRT, Mountains, sh4x2r1d1},
{MTNDIRT, Mountains, sh4x2u1b1},
{MTNDIRT, Mines, sh5x2},
{MTNCRCK, Mountains, sh6x4r1d2},
{MTNCRCK, Mountains, sh6x4u1b2},
{MTNCRCK, Mountains, sh6x3r1d2},
{MTNCRCK, Mountains, sh6x3u1b2},
{MTNCRCK, Mountains, sh4x3r1d1},
{MTNCRCK, Mountains, sh4x3u1b1},
{MTNCRCK, Mountains, sh4x2r1d1},
{MTNCRCK, Mountains, sh4x2u1b1},
{MTNCRCK, Mines, sh5x2},
{MTNDSRT, Mountains, sh6x4r1d2},
{MTNDSRT, Mountains, sh6x4u1b2},
{MTNDSRT, Mountains, sh4x3r1d1},
{MTNDSRT, Mountains, sh4x3u1b1},
{MTNDSRT, Mountains, sh4x2r1d1},
{MTNDSRT, Mountains, sh4x2u1b1},
{MTNDSRT, Mines, sh5x2},
{MTNGRAS, Mountains, sh6x4r1d2},
{MTNGRAS, Mountains, sh6x4u1b2},
{MTNGRAS, Mountains, sh4x3r1d1},
{MTNGRAS, Mountains, sh4x3u1b1},
{MTNGRAS, Mountains, sh4x2r1d1},
{MTNGRAS, Mountains, sh4x2u1b1},
{MTNGRAS, Mines, sh5x2},
{MTNLAVA, Mountains, sh6x4r1d2},
{MTNLAVA, Mountains, sh6x4u1b2},
{MTNLAVA, Mountains, sh4x3r1d1},
{MTNLAVA, Mountains, sh4x3u1b1},
{MTNLAVA, Mountains, sh4x2r1d1},
{MTNLAVA, Mountains, sh4x2u1b1},
{MTNLAVA, Mines, sh5x2},
{MTNMULT, Mountains, sh6x4r1d2},
{MTNMULT, Mountains, sh6x4u1b2},
{MTNMULT, Mountains, sh4x3r1d1},
{MTNMULT, Mountains, sh4x3u1b1},
{MTNMULT, Mountains, sh4x2r1d1},
{MTNMULT, Mountains, sh4x2u1b1},
{MTNMULT, Mines, sh5x2},
{MTNSNOW, Mountains, sh6x4r1d2},
{MTNSNOW, Mountains, sh6x4u1b2},
{MTNSNOW, Mountains, sh4x3r1d1},
{MTNSNOW, Mountains, sh4x3u1b1},
{MTNSNOW, Mountains, sh4x2r1d1},
{MTNSNOW, Mountains, sh4x2u1b1},
{MTNSNOW, Mines, sh5x2},
{MTNSWMP, Mountains, sh6x4r1d2},
{MTNSWMP, Mountains, sh6x4u1b2},
{MTNSWMP, Mountains, sh4x3r1d1},
{MTNSWMP, Mountains, sh4x3u1b1},
{MTNSWMP, Mountains, sh4x2r1d1},
{MTNSWMP, Mountains, sh4x2u1b1},
{MTNSWMP, Mines, sh5x2},
{OBJNMULT, Trees, sh2x2J},
{OBJNMULT, Trees, sh2x1},
{OBJNMULT, PeasantHut, sh3x2u2a9},
{OBJNMULT, Fort, sh4x3u2r1d1a9},
{OBJNMULT, Gazebo, sh2x2J},
{OBJNMULT, Flowers, sh3x1},
{OBJNMULT, DoctorHut, sh3x2u2},
{OBJNMULT, MercenaryCamp, sh3x1},
{OBJNMULT, Ruins, sh2x1},
{OBJNMULT, Shrine2, sh2x1},
{OBJNMULT, Shrine3, sh2x1},
{OBJNMULT, Shrine1, sh2x1},
{OBJNMULT, Idol, sh2x1},
{OBJNMULT, StandingStones, sh3x1},
{OBJNMULT, Temple, sh2x2},
{OBJNMULT, TradingPost, sh3x2u2a5v2},
{OBJNMULT, SpriteHouse, sh2x2J},
{OBJNMULT, ThatchedHut, sh2x1},
{OBJNMULT, TreeKnowledge, sh3x3tr},
{OBJNMULT, CampFire, sh2x1a6},
{OBJNMUL2, StreamDelta, sh3x3u1r1},
{OBJNMUL2, StreamDelta, sh3x3d1b1},
{OBJNMUL2, Fountain, sh2x1},
{OBJNMUL2, Stumps, sh1x1},
{OBJNMUL2, Stumps, sh2x1},
{OBJNMUL2, Stumps, sh1x1},
{OBJNMUL2, AlchemyLab, sh4x2а6},
{OBJNMUL2, DragonCity, sh7x4},
{OBJNMUL2, Graveyard, sh3x1}, // Not usable
{OBJNMUL2, LitghHouse, sh3x3u2m1},
{OBJNMUL2, SawMill, sh4x2sm}, // On brown dirt
{OBJNMUL2, WaterWheel, sh3x3u1r1a6},
{OBJNMUL2, Sign, sh2x1},
{OBJNMUL2, Teleporter, sh2x1},
{OBJNMUL2, Teleporter, sh2x2J},
{OBJNMUL2, Teleporter, sh2x2J},
{OBJNMUL2, RogueCamp, sh4x2а6_rogcmp},
{OBJNMUL2, EmpthyObject, sh6x4}, // Error ??
{OBJNMUL2, MagicWell, sh2x1},
{OBJNMUL2, Event, sh1x1},
{OBJNMUL2, MagicWell, sh2x1},
{OBJNMUL2, FreemanFoundry, sh3x2u1a6v2},
{OBJNMUL2, MagicGarden, sh2x1},
{OBJNMUL2, MagicGarden, sh1x1a6}, // Lephrechaunt here
{OBJNMUL2, WatchTower, sh3x2u2},
{OBJNMUL2, Graveyard, sh4x2gr},
{OBJNMUL2, SawMill, sh4x2sm}, // Green grass
};

res_s draw::getres(unsigned char object) {
	switch(object) {
	case 0: return NoRes;
	case 0x11: return MANA; // manual
	case 0x12:
		//	return TELEPORT2;
	case 0x13:
		//	return TELEPORT3;
	case 0x14:
		//	return FOUNTAIN;
	case 0x15: return TREASURY;
	case 0x2C: case 0x2D: case 0x2E: case 0x2F: return OBJNARTI; // artifact
	case 0x30: case 0x31: case 0x32: case 0x33: return MONS32; // monster
	case 0x38: case 0x39: case 0x3A: case 0x3B: return FLAG32; // castle flags
	case 0x54: case 0x55: case 0x56: case 0x57: return MINIHERO; // heroes
	case 0x58: case 0x59: case 0x5A: case 0x5B: return MTNSNOW; // relief: snow
	case 0x5C: case 0x5D: case 0x5E: case 0x5F: return MTNSWMP; // relief: swamp
	case 0x60: case 0x61: case 0x62: case 0x63: return MTNLAVA; // relief: lava
	case 0x64: case 0x65: case 0x66: case 0x67: return MTNDSRT; // relief: desert
	case 0x68: case 0x69: case 0x6A: case 0x6B: return MTNDIRT; // relief: dirt
	case 0x6C: case 0x6D: case 0x6E: case 0x6F: return MTNMULT; // relief: others
	case 0x74: return EXTRAOVR; // mines
	case 0x78: case 0x79: case 0x7A: case 0x7B: return ROAD; // road
	case 0x7C: case 0x7D: case 0x7E: case 0x7F: return MTNCRCK; // relief: crck
	case 0x80: case 0x81: case 0x82: case 0x83: return MTNGRAS; // relief: gras
	case 0x84: case 0x85: case 0x86: case 0x87: return TREJNGL; // trees jungle
	case 0x88: case 0x89: case 0x8A: case 0x8B: return TREEVIL; // trees evil
	case 0x8C: case 0x8D: case 0x8E: case 0x8F: return OBJNTOWN; // castle and tower
	case 0x90: case 0x91: case 0x92: case 0x93: return OBJNTWBA; // castle lands
	case 0x94: case 0x95: case 0x96: case 0x97: return OBJNTWSH; // castle shadow
	case 0x98: case 0x99: case 0x9A: case 0x9B: return OBJNTWRD; // random castle
	case 0xA0: case 0xA1: case 0xA2: case 0xA3: return OBJNWAT2; // water object
	case 0xA4: case 0xA5: case 0xA6: case 0xA7: return OBJNMUL2; // object other
	case 0xA8: case 0xA9: case 0xAA: case 0xAB: return TRESNOW; // trees snow
	case 0xAC: case 0xAD: case 0xAE: case 0xAF: return TREFIR; // trees trefir
	case 0xB0: case 0xB1: case 0xB2: case 0xB3: return TREFALL; // trees
	case 0xB4: case 0xB5: case 0xB6: case 0xB7: return STREAM; // river
	case 0xB8: case 0xB9: case 0xBA: case 0xBB: return OBJNRSRC; // resource
	case 0xC0: case 0xC1: case 0xC2: case 0xC3: return OBJNGRA2; // gras object
	case 0xC4: case 0xC5: case 0xC6: case 0xC7: return TREDECI; // trees tredeci
	case 0xC8: case 0xC9: case 0xCA: case 0xCB: return OBJNWATR; // sea object
	case 0xCC: case 0xCD: case 0xCE: case 0xCF: return OBJNGRAS; // vegetation gras
	case 0xD0: case 0xD1: case 0xD2: case 0xD3: return OBJNSNOW; // object on snow
	case 0xD4: case 0xD5: case 0xD6: case 0xD7: return OBJNSWMP; // object on swamp
	case 0xD8: case 0xD9: case 0xDA: case 0xDB: return OBJNLAVA; // object on lava
	case 0xDC: case 0xDD: case 0xDE: case 0xDF: return OBJNDSRT; // object on desert
	case 0xE0: case 0xE1: case 0xE2: case 0xE3: return OBJNDIRT; // object on dirt
	case 0xE4: case 0xE5: case 0xE6: case 0xE7: return OBJNCRCK; // object on crck
	case 0xE8: case 0xE9: case 0xEA: case 0xEB: return OBJNLAV3; // object on lava
	case 0xEC: case 0xED: case 0xEE: case 0xEF: return OBJNMULT; // object on earth
	case 0xF0: case 0xF1: case 0xF2: case 0xF3: return OBJNLAV2; //  object on lava
	default: return NoRes;
	}
}

void drawobji::initialize() {
	// Initialize shapes
	for(auto& e : bsmeta<drawobji>::elements) {
		auto& sh = e.shape;
		if(sh.initialized)
			continue;
		auto start_ex = 0;
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
	// Initialize map objects
	auto start_ex = 0;
	res_s res = NoRes;
	for(auto& e : bsmeta<drawobji>::elements) {
		if(res != e.res) {
			res = e.res;
			start_ex = 0;
		}
		e.first = start_ex;
		auto& sh = e.shape;
		auto count = sh.indecies[sh.count - 1] + sh.animation[sh.count - 1];
		e.last = e.first + count;
		start_ex += count + 1;
	}
}

const drawobji* drawobji::find(res_s res, unsigned char frame) {
	for(auto& e : bsmeta<drawobji>::elements) {
		if(e.res == res && frame >= e.first && frame <= e.last)
			return &e;
	}
	return 0;
}

void object_view() {
	char temp[64];
	const int maximum_count = sizeof(bsmeta<drawobji>::elements) / sizeof(bsmeta<drawobji>::elements[0]);
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
		zprint(temp, "%6 %1i/%5i (начало=%2i, к-во=%3i, следущий=%4i)", index, e.first, e_count, e.first + e_count, maximum_count, getstr(e.object));
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
			if(px==x1 && py==y1)
				rectb({px + 2, y1 + 2, px + 30, py + 30}, 75);
		}
		cursor(ADVMCO, 0);
		domodal();
		switch(hot::key) {
		case KeyEscape:
			buttoncancel();
			break;
		case KeyRight: index++; break;
		case KeyLeft: index--; break;
		case KeyUp: index+=10; break;
		case KeyDown: index -= 10; break;
		}
	}
}

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