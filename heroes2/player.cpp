#include "main.h"

static const char* random_names[] = {"Синий", "Зеленый", "Красный", "Желтый", "Оранжевый", "Фиолетовый"};

playeri bsmeta<playeri>::elements[LastPlayer + 1];

void playeri::clear() {
	memset(this, 0, sizeof(*this));
}

void playeri::initialize() {
	for(auto& e : bsmeta<playeri>::elements) {
		e.clear();
		if(e.getid() == RandomPlayer)
			continue;
		e.setname(random_names[e.getid()]);
		e.heroes[0] = RandomHero;
		e.heroes[1] = RandomHero;
	}
}

void playeri::endturn() {
	for(auto& e : bsmeta<playeri>::elements) {
		if(!e)
			continue;
		e.refresh();
		e.gainprofit();
	}
	for(unsigned i = 0; i < bsmeta<castlei>::count; i++) {
		auto& e = bsmeta<castlei>::elements[i];
		if(!e)
			continue;
		e.refresh();
	}
	for(unsigned i = FirstHero; i <= LastHero; i = hero_s(i+1)) {
		auto& e = bsmeta<heroi>::elements[i];
		if(!e.isadventure())
			continue;
		e.refresh();
	}
	map::day++;
}

void playeri::setup(difficult_s id) {
	resources = bsmeta<difficulti>::elements[id].resources;
	sethire(0);
	sethire(1);
}

int playeri::getmarket() const {
	return getbuildings(MarketPlace);
}

int playeri::getcastles() const {
	return getbuildings(Castle);
}

int playeri::gettowns() const {
	return getbuildings(NoBuilding) - getbuildings(Castle);
}

int playeri::getspies() const {
	return getbuildings(ThievesGuild);
}

int playeri::getadventurers() const {
	return getbuildings(Tavern);
}

int playeri::getrate(resource_s resf, resource_s rest, int markets) {
	const int max_count = 9;
	static int sale_uncostly[max_count + 1] = {0, 25, 37, 50, 62, 74, 87, 100, 112, 124};
	static int sale_costly[max_count + 1] = {0, 50, 74, 100, 124, 149, 175, 200, 224, 249};
	static int costly_costly[max_count + 1] = {0, 10, 7, 5, 4, 4, 3, 3, 3, 2};
	static int uncostly_costly[max_count + 1] = {0, 20, 14, 10, 8, 7, 6, 5, 5, 4};
	static int costly_uncostly[max_count + 1] = {0, 5, 4, 3, 2, 2, 2, 2, 2, 1};
	static int buy_costly[max_count + 1] = {0, 5000, 3334, 2500, 2000, 1667, 1429, 1250, 1112, 1000};
	static int buy_uncostly[max_count + 1] = {0, 2500, 1667, 1250, 1000, 834, 715, 625, 556, 500};
	if(!markets)
		return 0;
	if(resf == rest)
		return 0;
	if(markets > max_count)
		markets = max_count;
	switch(resf) {
	case Wood:
	case Ore:
		switch(rest) {
		case Gold:
			return sale_uncostly[markets];
		case Mercury:
		case Sulfur:
		case Crystal:
		case Gems:
			return uncostly_costly[markets];
		case Wood:
		case Ore:
			return costly_costly[markets];
		default:
			return 0;
		}
		break;
	case Mercury:
	case Sulfur:
	case Crystal:
	case Gems:
		switch(rest) {
		case Gold:
			return sale_costly[markets];
		case Mercury:
		case Sulfur:
		case Crystal:
		case Gems:
			return costly_costly[markets];
		case Wood:
		case Ore:
			return costly_uncostly[markets];
		default:
			return 0;
		}
	case Gold:
		switch(rest) {
		case Mercury:
		case Sulfur:
		case Crystal:
		case Gems:
			return buy_costly[markets];
		case Wood:
		case Ore:
			return buy_uncostly[markets];
		default:
			return 0;
		}
	default:
		return 0;
	}
}

void playeri::trade(resource_s rs, resource_s rt, int count, int market_count) {
	int rate = getrate(rs, rt, market_count);
	if(!rate)
		return;
	int cs = 0;
	int ct = 0;
	if(rt == Gold) {
		cs = count;
		ct = rate * count;
	} else {
		cs = rate * count;
		ct = count;
	}
	if(resources.data[rs] < cs)
		return;
	resources.data[rt] += ct;
	resources.data[rs] -= cs;
}

heroi* playeri::gethire(int index) const {
	if(heroes[index] == RandomHero)
		return 0;
	return bsmeta<heroi>::elements + heroes[index];
}

static bool already_hired(const heroi* hero) {
	for(auto& e : bsmeta<playeri>::elements) {
		if(!e)
			continue;
		if(e.gethire(0) == hero || e.gethire(1) == hero)
			return true;
	}
	return false;
}

heroi* playeri::randomhire(kind_s kind, kind_s excude_kind) {
	heroi* heroes[LastHero - FirstHero + 1];
	unsigned count = heroi::select(heroes, zendof(heroes), 0, kind, excude_kind);
	if(count) {
		auto p = heroes;
		for(unsigned i = 0; i < count; i++) {
			if(already_hired(heroes[i]))
				continue;
			*p++ = heroes[i];
		}
		count = p - heroes;
	}
	if(!count)
		return 0;
	return heroes[rand()%count];
}

heroi* playeri::randomhire(int index) const {
	heroi* hero = 0;
	if(kind != MultiPlayer) {
		if(index == 0)
			hero = randomhire(kind, RandomKind);
		else
			hero = randomhire(RandomKind, kind);
	}
	if(!hero)
		hero = randomhire(RandomKind, RandomKind);
	return hero;
}

void playeri::sethire(int index) {
	auto hero = randomhire(index);
	if(hero)
		this->heroes[index] = hero->getid();
	else
		this->heroes[index] = RandomHero;
}

void playeri::prepare(difficult_s difficult) {
	// В зависимости от сложности создадим начальные условия
	auto difficult_computer = bsmeta<difficulti>::elements[difficult].opposition;
	for(auto& e : bsmeta<playeri>::elements) {
		if(!e)
			continue;
		if(e.activity == ComputerOnly)
			e.activity = Computer;
		switch(e.getactivity()) {
		case Human: e.setup(difficult); break;
		case Computer: e.setup(difficult_computer); break;
		}
	}
	// Первоначальный прирост
	for(unsigned i = 0; i < bsmeta<castlei>::count; i++) {
		auto& e = bsmeta<castlei>::elements[i];
		if(!e)
			continue;
		e.growth();
	}
}

void playeri::hire(heroi* hero) {
	hero->set(this);
	hero->set(Up);
	for(auto& e : bsmeta<playeri>::elements) {
		for(auto i = 0; i < 2; i++) {
			if(e.heroes[i] == hero->getid()) {
				e.heroes[i] = RandomHero;
				e.heroes[i] = randomhire(i)->getid();
			}
		}
	}
}

int	playeri::getmines(resource_s id) const {
	auto result = 0;
	auto player = getid();
	for(unsigned i = 0; i < bsmeta<moveablei>::count; i++) {
		auto& e = bsmeta<moveablei>::elements[i];
		if(!e)
			continue;
		if(!e.is(player))
			continue;
		result++;
	}
	return result;
}

void playeri::refresh() {
}

int	playeri::getmineincome(resource_s id) {
	static int data[] = {1000, 2, 1, 2, 1, 1, 1};
	return data[id];
}

costi playeri::getprofit() const {
	static int estates_income[] = {0, 100, 250, 500};
	costi result; result.clear();
	// Прибыль с замков
	auto castles_count = getcastles();
	auto town_count = gettowns();
	auto statue_count = getbuildings(Statue);
	auto dungeon_count = getbuildings(SpecialBuilding, Warlock);
	result.add(Gold, castles_count * 1000);
	result.add(Gold, town_count * 500);
	result.add(Gold, statue_count * castlei::getstatueincome());
	result.add(Gold, dungeon_count * castlei::getdungeonincome());
	// Прибыль с героев
	artifact_s artifact_source[32];
	aref<artifact_s> artifacts(artifact_source);
	for(auto i = FirstHero; i <= LastHero; i = (hero_s)(i+1)) {
		auto& e = bsmeta<heroi>::elements[i];
		if(!e || !e.isadventure())
			continue;
		if(e.getplayer() != this)
			continue;
		// Прибыль с артифактов
		artifacts.count = e.get(artifacts.data, sizeof(artifact_source) / sizeof(artifact_source[0]));
		for(auto a : artifacts) {
			auto& ae = bsmeta<artifacti>::elements[a];
			if(ae.type.type == Resource)
				result.add(ae.type.resource, ae.power);
		}
		// Прибыль с навыков
		result.add(Gold, estates_income[e.get(Estates)]);
	}
	// Прибыль с шахт
	auto player_id = getid();
	for(unsigned i = 0; i < bsmeta<moveablei>::count; i++) {
		auto& e = bsmeta<moveablei>::elements[i];
		if(!e)
			continue;
		if(!e.is(player_id))
			continue;
		switch(e.gettype()) {
		case Mines:
			result.add(e.getresource(), getmineincome(e.getresource()));
			break;
		case AlchemyLab:
			result.add(Mercury, getmineincome(Mercury));
			break;
		case SawMill:
			result.add(Wood, getmineincome(Wood));
			break;
		}
	}
	return result;
}

void playeri::gainprofit() {
	getresources() += getprofit();
}

heroi* playeri::gethero(heroi* current) const {
	auto p2 = bsmeta<heroi>::elements + LastHero;
	for(auto p = current ? (current + 1) : bsmeta<heroi>::elements; p < p2; p++) {
		if(p->getplayer() != this)
			continue;
		return p;
	}
	return 0;
}