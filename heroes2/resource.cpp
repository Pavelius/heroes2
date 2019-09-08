#include "main.h"

resourcei bsmeta<resourcei>::elements[] = {{"Золото", "Золота"},
{"Дерево", "Дерева"},
{"Ртуть", "Ртути"},
{"Руда", "Руды"},
{"Сера", "Серы"},
{"Кристаллы", "Кристаллов"},
{"Драгоценности", "Драгоценностей"},
};
assert_enum(resource, Gems);

costi costi::operator*(int v) {
	costi r;
	for(auto i = 0; i < sizeof(data) / sizeof(data[0]); i++)
		r.data[i] = data[i] * v;
	return r;
}

bool costi::operator==(const costi& e) const {
	for(auto i = 0; i <= Gems; i++) {
		if(data[i] != e.data[i])
			return false;
	}
	return true;
}

void costi::clear() {
	memset(this, 0, sizeof(*this));
}

bool costi::operator<=(const costi& e) const {
	for(auto i = 0; i <= Gems; i++) {
		if(data[i] > e.data[i])
			return false;
	}
	return true;
}

bool costi::operator>=(const costi& e) const {
	for(auto i = 0; i <= Gems; i++) {
		if(data[i] < e.data[i])
			return false;
	}
	return true;
}

void costi::operator-=(const costi& e) {
	for(auto i = 0; i <= Gems; i++)
		data[i] -= e.data[i];
	correct();
}

void costi::operator+=(const costi& e) {
	for(auto i = 0; i <= Gems; i++)
		data[i] += e.data[i];
	correct();
}

int costi::mindiv(const costi& e) const {
	int m = 0;
	for(auto i = 0; i <= Gems; i++) {
		if(!e.data[i])
			continue;
		auto r = data[i] / e.data[i];
		if(r > m)
			m = r;
	}
	return m;
}

costi costi::getonly(resource_s id) const {
	costi result;
	result.clear();
	result.data[id] = data[id];
	return result;
}

void costi::correct() {
	for(auto i = 0; i <= Gems; i++) {
		if(data[i] < 0)
			data[i] = 0;
	}
}

//char* cost::tostring(char* r) const {
//	for(int i = (int)FirstResource; i <= (int)LastResource; i++) {
//		int n = get(i);
//		if(!n)
//			continue;
//		szprint(r, "%%r%1i(%2i)", i - FirstResource, n);
//		r = zend(r);
//	}
//	return r;
//}