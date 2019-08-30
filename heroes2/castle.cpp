#include "main.h"
#include "stringbuilder.h"

castlei bsmeta<castlei>::elements[128];
unsigned bsmeta<castlei>::count;

static building_s buildings_upgrade[NoBuilding + 1] = {
	NoBuilding, NoBuilding, NoBuilding, NoBuilding, NoBuilding, NoBuilding,
	NoBuilding, NoBuilding, NoBuilding,
	NoBuilding, NoBuilding, NoBuilding, NoBuilding,
	Dwelving1u, Dwelving2u, Dwelving3u, Dwelving4u, Dwelving5u, Dwelving6u,
	MageGuild2, Castle,
	NoBuilding, NoBuilding, NoBuilding, NoBuilding, NoBuilding, Dwelving6u2, NoBuilding,
	MageGuild3, MageGuild4, MageGuild5, NoBuilding, NoBuilding
};
static building_s buildings_downgrade[NoBuilding + 1] = {
	NoBuilding, NoBuilding, NoBuilding, NoBuilding, NoBuilding, NoBuilding,
	NoBuilding, NoBuilding, NoBuilding,
	NoBuilding, NoBuilding, NoBuilding, NoBuilding,
	NoBuilding, NoBuilding, NoBuilding, NoBuilding, NoBuilding, NoBuilding,
	NoBuilding, Tent,
	Dwelving1, Dwelving2, Dwelving3, Dwelving4, Dwelving5, Dwelving6, Dwelving6u,
	MageGuild, MageGuild2, MageGuild3, MageGuild4, NoBuilding
};

castlei* castlei::find(const playeri* player, castlei* first) {
	for(unsigned i = 0; i < bsmeta<castlei>::count; i++) {
		auto& e = bsmeta<castlei>::elements[i];
		if(e.getplayer() == player)
			return &e;
	}
	return 0;
}

void castlei::clear() {
	memset(this, 0, sizeof(*this));
}

void castlei::random(bool castle) {
	clear();
	set(Tent);
	if(castle)
		set(Castle);
	set(Dwelving1);
	if(d100() < 40)
		set(Dwelving2);
	else if(d100() < 60)
		set(Well);
	else
		set(MarketPlace);
}

building_s castlei::getupgrade(building_s i) {
	return buildings_upgrade[i];
}

building_s castlei::getupgraded(building_s v) const {
	while(is(v)) {
		auto v1 = buildings_upgrade[v];
		if(v1 == NoBuilding || !is(v1)) {
			if(is(v))
				return v;
			return NoBuilding;
		}
		v = v1;
	}
	return NoBuilding;
}

int castlei::getgrowth(building_s id) const {
	auto m = getmonster(id, getkind());
	auto n = bsmeta<monsteri>::elements[m].grown;
	if(is(Well))
		n += 2;
	return n;
}

building_s castlei::getdowngrade(building_s v) {
	return buildings_downgrade[v];
}

building_s castlei::getbase(building_s v) {
	while(true) {
		auto v1 = getdowngrade(v);
		if(v1 == NoBuilding)
			return v;
		v = v1;
	}
}

void castlei::growth() {
	for(auto i = 0; i < 6; i++) {
		auto dw = (building_s)(Dwelving1 + i);
		if(!is(dw))
			continue;
		population[i] = getgrowth(dw);
	}
}

bool castlei::isallow(monster_s v) const {
	for(auto& e : armyi::units) {
		if(e.unit == v)
			return true;
	}
	for(auto& e : armyi::units) {
		if(!e.count)
			return true;
	}
	return false;
}

void castlei::recruit(building_s building) {
	auto unit = getmonster(building, getkind());
	if(unit == RandomMonster)
		return;
	auto player = getplayer();
	if(!player)
		return;
	auto count = 0;
	auto index = getbase(building) - Dwelving1;
	if(!isallow(unit))
		return;
	if(player->recruit(unit, count, population[index])) {
		population[index] -= count;
		armyi::add(unit, count);
		auto cost = bsmeta<monsteri>::elements[unit].cost * count;
		player->getresources() -= cost;
	}
}

int playeri::getbuildings(building_s v) const {
	int result = 0;
	for(auto& e : bsmeta<castlei>::elements) {
		if(!e)
			continue;
		if(e.getplayer() != this)
			continue;
		if(e.is(v))
			result++;
	}
	return result;
}

castlei* castlei::add() {
	return &bsmeta<castlei>::elements[bsmeta<castlei>::count++];
}

const costi& castlei::getcost(building_s v, kind_s k) {
	static costi buildings_general[32] = {{{5000, 20, 0, 20}}, {{750, 5}}, {{500, 5}}, {{2000, 20}}, {{500}}, {{1250, 0, 0, 5}},
	{{1500, 0, 0, 5}}, {{1500, 0, 0, 5}}, {{750}},
	{{500, 5}}, {{500}}, {{1000}}
	};
	static costi mage_guild[5] = {{{2000, 5, 0, 5}},
	{{1000, 5, 5, 4, 4, 4, 4}},
	{{1000, 5, 5, 6, 6, 6, 6}},
	{{1000, 5, 5, 8, 8, 8, 8}},
	{{1000, 5, 5, 10, 10, 10, 10}},
	};
	static costi specail_building[6] = {{{2000, 10, 0, 10}},
	{{1500, 5, 0, 5}},
	{{1000, 0, 10, 0, 10, 0, 0}},
	{{1500, 0, 0, 0, 0, 10, 0}},
	{{3000, 5, 0, 10, 0, 0, 0}},
	{{1500, 5, 5, 5, 5, 5, 5}},
	};
	static costi dwelving_base[6][6] = {{{{300}},
	{{800, 5}},
	{{1000}},
	{{2000, 10, 0, 10}},
	{{4000, 0, 0, 20}},
	{{6000, 0, 0, 20, 0, 20}}},
		// Knight
	{{{200}},
	{{1000, 5}},
	{{1000, 0, 0, 5}},
	{{2000, 10, 0, 10}},
	{{3000, 20}},
	{{5000, 20, 0, 0, 0, 20}}},
		// Necromancer
	{{{400}},
	{{1000}},
	{{1500, 0, 0, 10}},
	{{3000, 10}},
	{{4000, 0, 0, 10, 10}},
	{{10000, 10, 5, 10, 5, 5, 5}}},
		//Sorcerer
	{{{500, 5}},
	{{1000, 5}},
	{{1500}},
	{{2500, 0, 0, 10}},
	{{3000, 10, 0, 0, 0, 0, 10}},
	{{10000, 30, 20}}},
		//Warlock
	{{{500}},
	{{1000, 0, 0, 10}},
	{{2000}},
	{{3000, 0, 0, 0, 0, 0, 10}},
	{{4000, 0, 0, 0, 10, 0, 0}},
	{{15000, 0, 0, 30, 20}}},
		//Wizard
	{{{400}},
	{{800}},
	{{1500, 5, 0, 5}},
	{{3000, 5}},
	{{4000, 5, 5, 5, 5, 5, 5}},
	{{12500, 5, 0, 5, 0, 0, 20}}},
	};
	static costi dwelving_upgrade[6][7] = {{{},
	{{1200, 5}},
	{{}},
	{{3000, 5, 0, 5}},
	{{2000, 10}},
	{{}}},
		// Knight
	{{{}},
	{{1500, 5}},
	{{1500, 0, 0, 5}},
	{{2000, 5, 0, 5}},
	{{3000, 10}},
	{{5000, 10, 0, 0, 0, 10}}},
		// Necromancer
	{{{}},
	{{1000}},
	{{1500, 5}},
	{{3000, 5, 0, 0, 0, 10, 10}},
	{{3000, 5, 0, 0, 5}},
	{{}}},
		//Sorcerer
	{{{}},
	{{1500, 5}},
	{{1500, 5}},
	{{1500, 0, 5}},
	{{}},
	{{}}},
		//Warlock
	{{{}},
	{{}},
	{{}},
	{{2000, 0, 0, 0, 0, 0, 5}},
	{{5000, 0, 0, 5, 10}},
	{{5000, 0, 0, 5, 10}}},
		//Wizard
	{{{}},
	{{}},
	{{1500, 0, 5}},
	{{}},
	{{4000, 5, 0, 5}},
	{{12500, 5, 0, 5, 0, 0, 20}}},
	};
	int i;
	switch(v) {
	case MageGuild:
	case MageGuild2:
	case MageGuild3:
	case MageGuild4:
	case MageGuild5:
		if(v >= MageGuild2)
			i = v - MageGuild2 + 1;
		else
			i = 0;
		return mage_guild[i];
	case Dwelving1:
	case Dwelving2:
	case Dwelving3:
	case Dwelving4:
	case Dwelving5:
	case Dwelving6:
		return dwelving_base[k][v - Dwelving1];
	case Dwelving1u:
	case Dwelving2u:
	case Dwelving3u:
	case Dwelving4u:
	case Dwelving5u:
	case Dwelving6u:
	case Dwelving6u2:
		return dwelving_upgrade[k][v - Dwelving1u];
	case SpecialBuilding:
		return specail_building[k];
	default:
		return maptbl(buildings_general, v);
	}
}

cflags<building_s> castlei::getprereqisit(building_s v, kind_s k) {
	switch(v) {
	case Moat:
	case Captain:
	case LeftTurret:
	case RightTurret:
		return {Castle};
	case MageGuild2: return {MageGuild};
	case MageGuild3: return {MageGuild2};
	case MageGuild4: return {MageGuild3};
	case MageGuild5: return {MageGuild4};
	case SpecialBuilding:
		switch(k) {
		case Sorcerer: return {MageGuild};
		case Necromancer: return {MageGuild};
		case Wizard: return {MageGuild};
		default: return {Castle};
		}
	case Dwelving1: return {};
	case Dwelving2:
		switch(k) {
		case Barbarian: return {Dwelving1};
		case Knight: return {Dwelving1};
		case Necromancer: return {Dwelving1};
		case Sorcerer: return {Dwelving1, Tavern};
		case Warlock: return {Dwelving1};
		case Wizard: return {Dwelving1};
		default: return {};
		}
	case Dwelving2u:
		switch(k) {
		case Barbarian: return {Dwelving1, Dwelving2, Dwelving3, Dwelving4};
		case Knight: return {Dwelving1, Dwelving2, Dwelving3, Dwelving4};
		case Necromancer: return {Dwelving1, MageGuild};
		case Sorcerer: return {Dwelving2, Well, Tavern};
		default: return {};
		}
	case Dwelving3:
		switch(k) {
		case Barbarian: return {Dwelving1};
		case Knight: return {Dwelving1, Well};
		case Necromancer: return {Dwelving1};
		case Sorcerer: return {Dwelving1};
		case Warlock: return {Dwelving2};
		case Wizard: return {Dwelving1};
		default: return {};
		}
	case Dwelving3u:
		switch(k) {
		case Knight: return {Dwelving3, Dwelving4};
		case Necromancer: return {Dwelving3};
		case Sorcerer: return {Dwelving3, Dwelving4};
		case Wizard: return {Dwelving3, Well};
		default: return {};
		}
	case Dwelving4:
		switch(k) {
		case Barbarian: return {Dwelving1};
		case Knight: return {Dwelving1, Tavern};
		case Necromancer: return {Dwelving3, ThievesGuild};
		case Sorcerer: return {Dwelving3, MageGuild};
		case Warlock: return {Dwelving3};
		case Wizard: return {Dwelving2};
		default: return {};
		}
	case Dwelving4u:
		switch(k) {
		case Barbarian: return {Dwelving2, Dwelving3, Dwelving4};
		case Knight: return {Dwelving3, Dwelving4};
		case Necromancer: return {Dwelving4};
		case Sorcerer: return {Dwelving4};
		default: return {};
		}
	case Dwelving5:
		switch(k) {
		case Barbarian: return {Dwelving2, Dwelving3, Dwelving4};
		case Knight: return {Dwelving3, Dwelving4};
		case Necromancer: return {Dwelving2, Dwelving3, MageGuild};
		case Sorcerer: return {Dwelving4};
		case Warlock: return {Dwelving3};
		case Wizard: return {Dwelving3, MageGuild};
		default: return {};
		}
	case Dwelving5u:
		switch(k) {
		case Barbarian: return {Dwelving5};
		case Knight: return {Dwelving5};
		case Necromancer: return {Dwelving5, MageGuild2};
		case Wizard: return {Dwelving5, SpecialBuilding};
		default: return {};
		}
	case Dwelving6:
		switch(k) {
		case Barbarian: return {Dwelving5};
		case Knight: return {Dwelving3, Dwelving4};
		case Necromancer: return {Dwelving5};
		case Sorcerer: return {Dwelving5};
		case Warlock: return {Dwelving4, Dwelving5};
		case Wizard: return {Dwelving4, Dwelving5};
		default: return {};
		}
	case Dwelving6u:
		switch(k) {
		case Knight: return {Dwelving3, Dwelving4};
		case Warlock: return {Dwelving6};
		case Wizard: return {Dwelving6};
		default: return {};
		}
	case Dwelving6u2: return {Dwelving6u};
	default: return {};
	}
}

const char* castlei::getdescription(building_s v, kind_s k) {
	int i;
	static char temp[512];
	switch(v) {
	case Captain: return "��������� �������� ��������� ������ ������������ ������ ����� � ���������� �����.";
	case Castle: return "����� ��������� ��� ������������� ������ ������ � ������.";
	case LeftTurret: return "��������� ������� �� ����� �����, ������� ������� ���� �����.";
	case MageGuild:
	case MageGuild2:
	case MageGuild3:
	case MageGuild4:
	case MageGuild5:
		if(v >= MageGuild2)
			i = v - MageGuild2 + 2;
		else
			i = 1;
		return zprint(temp, "��������� ����� �������� ����� ���������� � ������� ���������� %1i ������.", i);
	case Moat: return "����� � ��� ���� ����������� �������� � ����� -3 � ������.";
	case RightTurret: return "��������� ������� �� ������ �����, ������� ������� ���� �����.";
	case Shipyard: return "��������� ������� �������.";
	case Statue: return zprint(temp, "����� %1i ������� � ����.", getstatueincome());
	case Tavern: return zprint(temp, "������������ �� %1i ������ ������, ���������� �����. ����� ����� ����� ������ �����.", gettavernmorale());
	case Well: return zprint(temp, "����������� �� +%1i ������� ���� ���������.", getwellgrow());
	case Well2:
		return zprint(temp, "����������� ������� %-1 �� %2i � ������.",
			bsmeta<monsteri>::elements[getmonster(Dwelving1, k)].multiname,
			getwell2grow());
	case SpecialBuilding:
		switch(k) {
		case Barbarian: return "����������� �� 2 ������ ������, ���������� �����.";
		case Knight: return "��������� ����� ����� ������ ���� �� ����������.";
		case Necromancer: return "����������� �� 2 ���� ����� �����, ����������� �����.";
		case Sorcerer: return "����������� �� 2 ����� ������, ���������� �����.";
		case Warlock: return "����� 500 ������� � ����.";
		default: return "��������� � ������� ����� �� ������ ��������������� ���������� �� ������ ������.";
		}
	case Dwelving1:
	case Dwelving1u:
	case Dwelving2:
	case Dwelving2u:
	case Dwelving3:
	case Dwelving3u:
	case Dwelving4:
	case Dwelving4u:
	case Dwelving5:
	case Dwelving5u:
	case Dwelving6:
	case Dwelving6u:
	case Dwelving6u2:
		return zprint(temp, "��������� �������� %-1.",
			bsmeta<monsteri>::elements[getmonster(v, k)].multiname);
	default: return 0;
	}
}

const char* getstr(building_s v, kind_s k) {
	//static const char* rome_number[] = {"", "I", "II", "III", "IV", "V", "VI"};
	int i;
	static char temp[512];
	switch(v) {
	case Captain: return "������� ������";
	case Castle: return "�����";
	case LeftTurret: return "����� �����";
	case MageGuild:
	case MageGuild2:
	case MageGuild3:
	case MageGuild4:
	case MageGuild5:
		if(v >= MageGuild2)
			i = v - MageGuild2 + 2;
		else
			i = 1;
		return zprint(temp, "������� ����� %1i", i);
	case Moat: return "���";
	case RightTurret: return "������ �����";
	case Shipyard: return "�����";
	case Statue: return "������";
	case Tavern: return "�������";
	case Well: return "�������";
	case MarketPlace: return "�����";
	case ThievesGuild: return "������� �����";
	case Well2:
		switch(k) {
		case Barbarian: return "���� ������";
		case Knight: return "�����";
		case Necromancer: return "���� �������";
		case Sorcerer: return "����������� ���";
		case Warlock: return "�������";
		default: return "���������";
		}
	case SpecialBuilding:
		switch(k) {
		case Barbarian: return "��������";
		case Knight: return "����������";
		case Necromancer: return "����";
		case Warlock: return "����������";
		case Sorcerer: return "������";
		default: return "����������";
		}
	case Dwelving1:
		switch(k) {
		case Barbarian: return "������ ��������";
		case Knight: return "��� �������";
		case Necromancer: return "�����������";
		case Warlock: return "������";
		case Sorcerer: return "���� �� ��������";
		default: return "����";
		}
	case Dwelving2:
		switch(k) {
		case Barbarian: return "������ �����";
		case Knight: return "�������";
		case Necromancer: return "��������";
		case Sorcerer: return "�������";
		case Warlock: return "������";
		default: return "�����";
		}
	case Dwelving3:
		switch(k) {
		case Barbarian: return "������";
		case Knight: return "�������";
		case Necromancer: return "��������";
		case Sorcerer: return "�������";
		case Warlock: return "������";
		default: return "�������";
		}
	case Dwelving4:
		switch(k) {
		case Barbarian: return "�������";
		case Knight: return "���������";
		case Necromancer: return "��������";
		case Sorcerer: return "���������";
		case Warlock: return "��������";
		default: return "������";
		}
	case Dwelving5:
		switch(k) {
		case Barbarian: return "����";
		case Knight: return "�����";
		case Necromancer: return "��������";
		case Sorcerer: return "�����";
		case Warlock: return "������";
		default: return "����� �����";
		}
	case Dwelving6:
		switch(k) {
		case Barbarian: return "��������";
		case Knight: return "�����";
		case Necromancer: return "�����������";
		case Sorcerer: return "������� �����";
		case Warlock: return "������� �����";
		default: return "�������� �����";
		}
	case Dwelving2u:
	case Dwelving3u:
	case Dwelving4u:
	case Dwelving5u:
	case Dwelving6u:
		if(v == Dwelving6u && k == Warlock)
			return "������� �����";
		return zprint(temp, "��. %1", getstr(building_s(Dwelving2 + (v - Dwelving2u)), k));
	case Dwelving6u2: return "������ �����";
	default: return "��� ��������";
	}
}