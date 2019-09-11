#include "main.h"

heroi*				battle::attacker;
heroi*				battle::defender;
bool				battle::setting::movement = true;
bool				battle::setting::cursor = true;
bool				battle::setting::distance = true;
bool				battle::setting::grid = true;
bool				battle::setting::index = true;

static short unsigned position_wide[2][5] = {{0, 22, 44, 66, 88}, {10, 32, 54, 76, 98}};

unsigned battle::getcost(short unsigned index) {
	return Blocked;
}

bool battle::isattacker(const heroi* hero) {
	return hero == attacker;
}

static int getside(const heroi* leader) {
	return (battle::attacker == leader) ? 0 : 1;
}

void battle::add(armyi& army, heroi* leader) {
	auto index = 0;
	auto side = getside(leader);
	for(auto& e : army.units) {
		if(!e)
			continue;
		add(position_wide[side][index], e, leader);
		index++;
		if(index >= 5)
			break;
	}
}