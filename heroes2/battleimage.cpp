#include "view.h"

using namespace draw;

struct monsterai {
	res_s			icn;
	unsigned char	idle[2];
	unsigned char	move[2];
	unsigned char	fly1[2];
	unsigned char	fly2[2];
	unsigned char	fly3[2];
	unsigned char	shot0[2];
	unsigned char	shot1[2];
	unsigned char	shot2[2];
	unsigned char	shot3[2];
	unsigned char	attk0[2];
	unsigned char	attk1[2];
	unsigned char	attk2[2];
	unsigned char	attk3[2];
	unsigned char	wcne[2];
	unsigned char	kill[2];
	res_s			wav_attk;
	res_s			wav_kill;
	res_s			wav_move;
	res_s			wav_wnce;
};
static monsterai monsters[] = {{PEASANT, {1, 4}, {5, 8}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{0, 0}, {0, 0}, {0, 0}, {0, 0}, /*Attack*/{0, 0}, {16, 6}, {22, 6}, {28, 6}, {13, 3}, {34, 4}, WavPSNTATTK, WavPSNTKILL, WavPSNTMOVE, WavPSNTWNCE},
{ARCHER, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 3}, {39, 3}, {42, 3}, {13, 3}, {45, 6}, WavARCHATTK, WavARCHKILL, WavARCHMOVE, WavARCHWNCE},
{ARCHER2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 3}, {39, 3}, {42, 3}, {13, 3}, {45, 6}, WavARCHATTK, WavARCHKILL, WavARCHMOVE, WavARCHWNCE},
{PIKEMAN, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {17, 3}, {20, 4}, {24, 6}, {13, 4}, {30, 6}, WavPIKEATTK, WavPIKEKILL, WavPIKEMOVE, WavPIKEWNCE},
{PIKEMAN2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {17, 3}, {20, 4}, {24, 6}, {13, 4}, {30, 6}, WavPIKEATTK, WavPIKEKILL, WavPIKEMOVE, WavPIKEWNCE},
{SWORDSMN, {39, 6}, {2, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {12, 2}, {19, 5}, {14, 5}, {24, 5}, {36, 3}, {29, 7}, WavSWDMATTK, WavSWDMKILL, WavSWDMMOVE, WavSWDMWNCE},
{SWORDSM2, {39, 6}, {2, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {12, 2}, {19, 5}, {14, 5}, {24, 5}, {36, 3}, {29, 7}, WavSWDMATTK, WavSWDMKILL, WavSWDMMOVE, WavSWDMWNCE},
{CAVALRYR, {19, 4}, {1, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {8, 1}, {12, 3}, {9, 3}, {15, 2}, {28, 2}, {23, 5}, WavCAVLATTK, WavCAVLKILL, WavCAVLMOVE, WavCAVLWNCE},
{CAVALRYB, {19, 4}, {1, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {8, 1}, {12, 3}, {9, 3}, {15, 2}, {28, 2}, {23, 5}, WavCAVLATTK, WavCAVLKILL, WavCAVLMOVE, WavCAVLWNCE},
{PALADIN, {1, 11}, {12, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 2}, {22, 3}, {25, 3}, {28, 3}, {31, 2}, {34, 5}, WavPLDNATTK, WavPLDNKILL, WavPLDNMOVE, WavPLDNWNCE},
{PALADIN2, {1, 11}, {12, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 2}, {22, 3}, {25, 3}, {28, 3}, {31, 2}, {34, 5}, WavPLDNATTK, WavPLDNKILL, WavPLDNMOVE, WavPLDNWNCE},
// Barbarian
{GOBLIN, {33, 7}, {1, 9}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{0, 0}, {0, 0}, {0, 0}, {0, 0}, /*Attack*/{10, 3}, {17, 4}, {13, 4}, {21, 4}, {25, 4}, {29, 4}, WavGBLNATTK, WavGBLNKILL, WavGBLNMOVE, WavGBLNWNCE},
{ORC, {1, 4}, {5, 8}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{16, 13}, {0, 0}, {0, 0}, {0, 0}, /*Attack*/{28, 2}, {30, 3}, {33, 4}, {37, 3}, {13, 3}, {40, 4}, WavORC_ATTK, WavORC_KILL, WavORC_MOVE, WavORC_WNCE},
{ORC2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {16, 13}, {0, 0}, {0, 0}, {0, 0}, {28, 2}, {30, 3}, {33, 4}, {37, 3}, {13, 3}, {40, 4}, WavORC_ATTK, WavORC_KILL, WavORC_MOVE, WavORC_WNCE},
{WOLF, {20, 6}, {7, 6}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 3}, {2, 5}, {16, 4}, {26, 3}, {28, 5}, WavWOLFATTK, WavWOLFKILL, WavWOLFMOVE, WavWOLFWNCE},
{OGRE, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 6}, {22, 7}, {29, 8}, {13, 3}, {37, 4}, WavOGREATTK, WavOGREKILL, WavOGREMOVE, WavOGREWNCE},
{OGRE2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 6}, {22, 7}, {29, 8}, {13, 3}, {37, 4}, WavOGREATTK, WavOGREKILL, WavOGREMOVE, WavOGREWNCE},
{TROLL, {16, 7}, {1, 15}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{23, 5}, {0, 0}, {0, 0}, {0, 0}, /*Attack*/{0, 0}, {48, 6}, {23, 6}, {29, 5}, {54, 3}, {57, 9}, WavTRLLATTK, WavTRLLKILL, WavTRLLMOVE, WavTRLLWNCE},
{TROLL2, {16, 7}, {1, 15}, {0, 0}, {0, 0}, {0, 0}, {23, 5}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {48, 6}, {23, 6}, {29, 5}, {54, 3}, {57, 9}, WavTRLLATTK, WavTRLLKILL, WavTRLLMOVE, WavTRLLWNCE},
{CYCLOPS, {30, 9}, {1, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 2}, {11, 2}, {21, 2}, {0, 0}, {14, 3}, {8, 3}, {18, 3}, {23, 2}, {25, 5}, WavCYCLATTK, WavCYCLKILL, WavCYCLMOVE, WavCYCLWNCE},
// Sorcerer
{SPRITE, {16, 9}, {0, 0}, {1, 3}, {4, 4}, {7, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {30, 6}, {25, 5}, {36, 5}, {9, 2}, {11, 5}, WavSPRTATTK, WavSPRTKILL, WavSPRTMOVE, WavSPRTWNCE},
{DWARF, {44, 5}, {1, 9}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 7}, {10, 10}, {27, 9}, {36, 8}, {49, 7}, WavDWRFATTK, WavDWRFKILL, WavDWRFMOVE, WavDWRFWNCE},
{DWARF2, {44, 5}, {1, 9}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 7}, {10, 10}, {27, 9}, {36, 8}, {49, 7}, WavDWRFATTK, WavDWRFKILL, WavDWRFMOVE, WavDWRFWNCE},
{ELF, {42, 6}, {1, 12}, {0, 0}, {0, 0}, {0, 0}, {13, 10}, {0, 0}, {0, 0}, {0, 0}, {23, 3}, {26, 3}, {29, 3}, {32, 4}, {36, 2}, {38, 4}, WavELF_ATTK, WavELF_KILL, WavELF_MOVE, WavELF_WNCE},
{ELF2, {42, 6}, {1, 12}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{13, 10}, {0, 0}, {0, 0}, {0, 0}, {23, 3}, {26, 3}, {29, 3}, {32, 4}, {36, 2}, {38, 4}, WavELF_ATTK, WavELF_KILL, WavELF_MOVE, WavELF_WNCE},
{DRUID, {46, 4}, {3, 11}, {0, 0}, {0, 0}, {0, 0}, {14, 8}, {35, 4}, {22, 4}, {39, 5}, {14, 8}, {35, 4}, {22, 4}, {39, 4}, {44, 2}, {26, 9}, WavDRUIATTK, WavDRUIKILL, WavDRUIMOVE, WavDRUIWNCE},
{DRUID2, {46, 4}, {3, 11}, {0, 0}, {0, 0}, {0, 0}, {14, 8}, {35, 4}, {22, 4}, {39, 5}, {14, 8}, {35, 4}, {22, 4}, {39, 4}, {44, 2}, {26, 9}, WavDRUIATTK, WavDRUIKILL, WavDRUIMOVE, WavDRUIWNCE},
{UNICORN, {1, 9}, {10, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {18, 9}, {27, 7}, {34, 7}, {41, 4}, {45, 7}, WavUNICATTK, WavUNICKILL, WavUNICMOVE, WavUNICWNCE},
{PHOENIX, {30, 5}, {0, 0}, {1, 3}, {4, 6}, {0, 0}, {0, 0}, {25, 3}, {23, 2}, {28, 2}, {0, 0}, {13, 4}, {10, 4}, {17, 4}, {35, 2}, {37, 11}, WavPHOEATTK, WavPHOEKILL, WavPHOEMOVE, WavPHOEWNCE},
// Warlock
{CENTAUR, {65, 6}, {2, 6}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{18, 4}, {27, 3}, {22, 5}, {30, 3}, {33, 1}, {38, 4}, {34, 4}, {42, 4}, {46, 2}, {48, 7}, WavCNTRATTK, WavCNTRKILL, WavCNTRMOVE, WavCNTRWNCE},
{GARGOYLE, {1, 4}, {0, 0}, /*Fly*/{5, 2}, {7, 4}, {11, 1}, /*Shoot*/{0, 0}, {0, 0}, {0, 0}, {0, 0}, {14, 1}, {15, 4}, {19, 4}, {23, 4}, {12, 2}, {27, 5}, WavGARGATTK, WavGARGKILL, WavGARGMOVE, WavGARGWNCE},
{GRIFFIN, {16, 9}, {0, 0}, {1, 3}, {3, 3}, {5, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {7, 1}, {8, 2}, {14, 2}, {11, 3}, {9, 2}, {25, 9}, WavGRIFATTK, WavGRIFKILL, WavGRIFMOVE, WavGRIFWNCE},
{MINOTAUR, {1, 5}, {6, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 3}, {20, 3}, {16, 4}, {23, 4}, {27, 2}, {29, 6}, WavMINOATTK, WavMINOKILL, WavMINOMOVE, WavMINOWNCE},
{MINOTAU2, {1, 5}, {6, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 3}, {20, 3}, {16, 4}, {23, 4}, {27, 2}, {29, 6}, WavMINOATTK, WavMINOKILL, WavMINOMOVE, WavMINOWNCE},
{HYDRA, {28, 15}, {1, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {9, 8}, {9, 8}, {9, 8}, {17, 5}, {21, 7}, WavHYDRATTK, WavHYDRKILL, WavHYDRMOVE, WavHYDRWNCE},
{DRAGGREE, {41, 13}, {0, 0}, {1, 4}, {5, 3}, {8, 5}, {13, 1}, {17, 3}, {23, 3}, {30, 2}, {13, 1}, {14, 3}, {20, 3}, {26, 4}, {32, 3}, {35, 6}, WavDRGNATTK, WavDRGNKILL, WavDRGNMOVE, WavDRGNWNCE},
{DRAGRED, {41, 13}, {0, 0}, {1, 4}, {5, 3}, {8, 5}, {13, 1}, {17, 3}, {23, 3}, {30, 2}, {13, 1}, {14, 3}, {20, 3}, {26, 4}, {32, 3}, {35, 6}, WavDRGNATTK, WavDRGNKILL, WavDRGNMOVE, WavDRGNWNCE},
{DRAGBLAK, {41, 13}, {0, 0}, {1, 4}, {5, 3}, {8, 5}, {13, 1}, {17, 3}, {23, 3}, {30, 2}, {13, 1}, {14, 3}, {20, 3}, {26, 4}, {32, 3}, {35, 6}, WavDRGNATTK, WavDRGNKILL, WavDRGNMOVE, WavDRGNWNCE},
// Wizard
{HALFLING, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {17, 5}, {22, 2}, {26, 1}, {27, 2}, {29, 1}, {30, 2}, {32, 3}, {35, 2}, {13, 4}, {37, 4}, WavHALFATTK, WavHALFKILL, WavHALFMOVE, WavHALFWNCE},
{BOAR, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {15, 1}, {16, 3}, {19, 3}, {22, 2}, {13, 2}, {24, 5}, WavBOARATTK, WavBOARKILL, WavBOARMOVE, WavBOARWNCE},
{GOLEM, {34, 6}, {1, 12}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {22, 4}, {13, 5}, {18, 4}, {32, 4}, {26, 6}, WavGOLMATTK, WavGOLMKILL, WavGOLMMOVE, WavGOLMWNCE},
{GOLEM2, {34, 6}, {1, 12}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {22, 4}, {13, 5}, {18, 4}, {32, 4}, {26, 6}, WavGOLMATTK, WavGOLMKILL, WavGOLMMOVE, WavGOLMWNCE},
{ROC, {18, 7}, {0, 0}, {1, 3}, {4, 3}, {7, 1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {14, 3}, {8, 3}, {11, 3}, {25, 3}, {25, 9}, WavROC_ATTK, WavROC_KILL, WavROC_MOVE, WavROC_WNCE},
{MAGE1, {1, 7}, {43, 8}, {0, 0}, {0, 0}, {0, 0}, {18, 1}, {21, 2}, {19, 2}, {23, 4}, {27, 1}, {34, 3}, {28, 6}, {37, 6}, {8, 3}, {11, 7}, WavMAGEATTK, WavMAGEKILL, WavMAGEMOVE, WavMAGEWNCE},
{MAGE2, {1, 7}, {43, 8}, {0, 0}, {0, 0}, {0, 0}, {18, 1}, {21, 2}, {19, 2}, {23, 4}, {27, 1}, {34, 3}, {28, 6}, {37, 6}, {8, 3}, {11, 7}, WavMAGEATTK, WavMAGEKILL, WavMAGEMOVE, WavMAGEWNCE},
{TITANBLU, {1, 6}, {7, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {19, 5}, {14, 5}, {24, 5}, {29, 2}, {31, 7}, WavTITNATTK, WavTITNKILL, WavTITNMOVE, WavTITNWNCE},
{TITANBLA, {1, 6}, {7, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {45, 5}, {38, 5}, {50, 5}, {0, 0}, {19, 5}, {14, 5}, {24, 5}, {29, 2}, {31, 7}, WavTITNATTK, WavTITNKILL, WavTITNMOVE, WavTITNWNCE},
// Necromancer
{SKELETON, {35, 4}, {3, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 4}, {12, 4}, {16, 4}, {24, 3}, {27, 8}, WavSKELATTK, WavSKELKILL, WavSKELMOVE, WavSKELWNCE},
{ZOMBIE, {14, 12}, {1, 13}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {26, 5}, {31, 5}, {36, 5}, {40, 3}, {47, 7}, WavZOMBATTK, WavZOMBKILL, WavZOMBMOVE, WavZOMBWNCE},
{ZOMBIE2, {14, 12}, {1, 13}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {26, 5}, {31, 5}, {36, 5}, {40, 3}, {47, 7}, WavZOMBATTK, WavZOMBKILL, WavZOMBMOVE, WavZOMBWNCE},
{MUMMYW, {1, 4}, {5, 12}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {21, 3}, {24, 3}, {27, 3}, {17, 4}, {30, 10}, WavMUMYATTK, WavMUMYKILL, WavMUMYMOVE, WavMUMYWNCE},
{MUMMY2, {1, 4}, {5, 12}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {21, 3}, {24, 3}, {27, 3}, {17, 4}, {30, 10}, WavMUMYATTK, WavMUMYKILL, WavMUMYMOVE, WavMUMYWNCE},
{VAMPIRE, {1, 4}, {0, 0}, {5, 4}, {9, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {15, 4}, {19, 4}, {23, 5}, {13, 3}, {28, 8}, WavVAMPATTK, WavVAMPKILL, WavVAMPMOVE, WavVAMPWNCE},
{VAMPIRE2, {1, 4}, {0, 0}, {5, 4}, {9, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {15, 4}, {19, 4}, {23, 5}, {13, 3}, {28, 8}, WavVAMPATTK, WavVAMPKILL, WavVAMPMOVE, WavVAMPWNCE},
{LICH, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 5}, {18, 5}, {23, 5}, {0, 0}, {13, 5}, {18, 5}, {23, 5}, {28, 3}, {31, 7}, WavLICHATTK, WavLICHKILL, WavLICHMOVE, WavLICHWNCE},
{LICH2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 5}, {18, 8}, {23, 5}, {0, 0}, {13, 5}, {18, 5}, {23, 5}, {28, 3}, {31, 7}, WavLICHATTK, WavLICHKILL, WavLICHMOVE, WavLICHWNCE},
{DRAGBONE, {22, 7}, {0, 0}, {1, 4}, {5, 3}, {8, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {47, 5}, {12, 4}, {16, 4}, {20, 2}, {41, 6}, WavBONEATTK, WavBONEKILL, WavBONEMOVE, WavBONEWNCE},
// Neutral
{ROGUE, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 8}, {24, 10}, {34, 9}, {13, 3}, {43, 7}, WavROGUATTK, WavROGUKILL, WavROGUMOVE, WavROGUWNCE},
{NOMAD, {1, 9}, {10, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {18, 2}, {26, 5}, {20, 6}, {31, 4}, {35, 2}, {37, 5}, WavNMADATTK, WavNMADKILL, WavNMADMOVE, WavNMADWNCE},
{GHOST, {1, 3}, {0, 0}, {0, 0}, {4, 5}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {10, 3}, {13, 3}, {16, 4}, {20, 2}, {20, 8}, WavGHSTATTK, WavGHSTKILL, WavGHSTMOVE, WavGHSTWNCE},
{GENIE, {1, 9}, {0, 0}, {10, 1}, {11, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {26, 5}, {16, 5}, {21, 5}, {31, 2}, {33, 7}, WavGENIATTK, WavGENIKILL, WavGENIMOVE, WavGENIWNCE},
{MEDUSA, {17, 7}, {1, 16}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {65, 10}, {41, 13}, {54, 11}, {36, 2}, {24, 12}, WavMEDSATTK, WavMEDSKILL, WavMEDSMOVE, WavMEDSWNCE},
{EELEM, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 5}, {25, 6}, {13, 3}, {31, 11}, WavEELMATTK, WavEELMKILL, WavEELMMOVE, WavEELMWNCE},
{AELEM, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 5}, {25, 6}, {13, 3}, {31, 11}, WavAELMATTK, WavAELMKILL, WavAELMMOVE, WavAELMWNCE},
{FELEM, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 5}, {25, 6}, {13, 3}, {31, 11}, WavFELMATTK, WavFELMKILL, WavFELMMOVE, WavFELMWNCE},
{WELEM, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 5}, {25, 6}, {13, 3}, {31, 11}, WavWELMATTK, WavWELMKILL, WavWELMMOVE, WavWELMWNCE},
// Random
{NoRes, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, NoRes, NoRes, NoRes, NoRes},
{NoRes, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, NoRes, NoRes, NoRes, NoRes},
{NoRes, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, NoRes, NoRes, NoRes, NoRes},
{NoRes, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, NoRes, NoRes, NoRes, NoRes},
{NoRes, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, NoRes, NoRes, NoRes, NoRes},
};

void battleimage::clear() {
	memset(this, 0, sizeof(*this));
	index = Blocked;
}

static int getbarframe() {
	bool isboosted = false;
	bool ispenalized = false;
	if(isboosted && ispenalized)
		return 13;
	else if(isboosted)
		return 12;
	else if(ispenalized)
		return 14;
	return 10;
}

void battleimage::stroke() const {
	draw::stroke(pos.x, pos.y, res, frame, flags, 1, 219);
}

void battleimage::paint() const {
	image(pos.x, pos.y, res, frame, flags);
	if(type == Hero && hero != RandomHero) {
		auto player = bsmeta<heroi>::elements[hero].getplayer()->getid();
		auto icn = res_s(HEROFL00 + player - PlayerBlue);
		draw::image(pos.x, pos.y, icn, draw::counter % 5, flags);
	} else if(type == Monster) {
		auto count = this->uniti::count;
		if(count) {
			state push;
			font = SMALFONT;
			char temp[32]; zprint(temp, "%1i", count);
			auto x = pos.x;
			auto y = pos.y;
			auto frame = getbarframe();
			auto x1 = x;
			auto y1 = y;
			if((flags&AFMirror)==0) {
				x1 += 12;
				y1 -= draw::getheight(TEXTBAR, frame);
			} else {
				x1 -= 32;
			}
			image(x1, y1, TEXTBAR, frame);
			text(x1 + (draw::getwidth(TEXTBAR, frame) - textw(temp)) / 2, y1 + 2, temp);
		}
	}
}

void battleimage::set(action_s action, int param) {
	start = 0;
	animation::count = 0;
	wait = 0;
	if(type == Monster) {
		auto& e = monsters[monster];
		res = e.icn;
		switch(action) {
		case Wait:
			switch(monster) {
			case Zombie:
			case MutantZombie:
				start = e.idle[0] + 7;
				animation::count = 5;
				break;
			default:
				if(d100() < 60) {
					start = e.idle[0];
					animation::count = e.idle[1] / 2;
				} else {
					animation::count = e.idle[1] / 2;
					start = e.idle[0] + e.idle[1] - animation::count;
				}
				wait = xrand(4, 10);
				break;
			}
			break;
		case Move:
			start = e.move[0];
			animation::count = e.move[1];
			break;
		case Damaged:
			start = e.wcne[0];
			animation::count = e.wcne[1];
			break;
		case Killed:
			start = e.kill[0];
			animation::count = e.kill[1];
			break;
		case Dead:
			start = e.kill[0] + e.kill[1] - 1;
			animation::count = 1;
			break;
		case AttackAction:
			switch(param) {
			case 0:
				start = e.attk0[0];
				animation::count = e.attk0[1];
				break;
			case 1:
				start = e.attk1[0];
				animation::count = e.attk1[1];
				break;
			case 2:
				start = e.attk2[0];
				animation::count = e.attk2[1];
				break;
			default:
				start = e.attk3[0];
				animation::count = e.attk3[1];
				break;
			}
			break;
		case Shoot:
			switch(param) {
			case 0:
				start = e.shot0[0];
				animation::count = e.shot0[1];
				break;
			case 1:
				start = e.shot1[0];
				animation::count = e.shot1[1];
				break;
			case 2:
				start = e.shot2[0];
				animation::count = e.shot2[1];
				break;
			default:
				start = e.shot3[0];
				animation::count = e.shot3[1];
				break;
			}
			break;
		case FlyAction:
			switch(param) {
			case 0:
				start = e.fly1[0];
				animation::count = e.fly1[1];
				break;
			case 1:
				start = e.fly2[0];
				animation::count = e.fly2[1];
				break;
			case 2:
				start = e.fly3[0];
				animation::count = e.fly3[1];
				break;
			}
			break;
		default:
			break;
		}
	} else if(type == Hero) {
		auto kind = bsmeta<heroi>::elements[hero].getkind();
		switch(kind) {
		case Barbarian:
			res = CMBTHROB;
			switch(value) {
			case Wait: start = xrand(15, 18); animation::count = 1; break;
			case PalmFace: start = 1; animation::count = 5; break;
			case Cast: start = 6; animation::count = 9; break;
			default: break;
			}
			break;
		case Knight:
			res = CMBTHROK;
			switch(action) {
			case Wait: start = 15; animation::count = 5; break;
			case PalmFace: start = 1; animation::count = 5; break;
			case Cast: start = 12; animation::count = 2; break;
			default: break;
			}
			break;
		case Necromancer:
			res = CMBTHRON;
			switch(action) {
			case Wait: start = xrand(17, 19); animation::count = 1; break;
			case PalmFace: start = 1; animation::count = 5; break;
			case Cast: start = 6; animation::count = 9; break;
			default: break;
			}
			break;
		case Sorcerer:
			res = CMBTHROS;
			switch(action) {
			case Wait: start = 13; animation::count = 4; break;
			case PalmFace: start = 1; animation::count = 5; break;
			case Cast: start = 6; animation::count = 7; break;
			default: break;
			}
			break;
		case Warlock:
			res = CMBTHROW;
			switch(action) {
			case Wait: start = 14; animation::count = 3; break;
			case PalmFace: start = 1; animation::count = 5; break;
			case Cast: start = 6; animation::count = 8; break;
			default: break;
			}
			break;
		case Wizard:
			res = CMBTHROZ;
			switch(action) {
			case Wait: start = 16; animation::count = 3; break;
			case PalmFace: start = 1; animation::count = 5; break;
			case Cast: start = 12; animation::count = 7; break;
			default: break;
			}
			break;
		default: res = NoRes; start = 0; animation::count = 0; break;
		}
		if(action==Wait)
			wait = xrand(3, 8);
	}
	frame = start;
}

void battleimage::setdefault() {
	if(iskilled())
		set(Dead);
	else
		set(Wait);
}

void battleimage::update() {
	if(animation::update())
		setdefault();
}

void battleimage::set(direction_s d) {
	if(d == Left || d == LeftDown || d == LeftUp)
		flags = AFMirror;
	else
		flags = 0;
}

bool battleimage::iskilled() const {
	return frame >= monsters[unit].kill[0]
		&& frame < monsters[unit].kill[0] + monsters[unit].kill[1];
}

bool battleimage::iswait() const {
	return frame >= monsters[unit].idle[0]
		&& frame <= monsters[unit].idle[0] + monsters[unit].idle[1];
}