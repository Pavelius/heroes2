#include "view.h"

using namespace draw;

static void scan_v1(unsigned* counter, const unsigned char* s) {
	while(true) {
		unsigned char c = *s;
		if(c == 0x00) {
			s++;
			// Next line
		} else if(c < 0x80) {// 0..0x7F - bytes copy from src
			s++;
			for(auto i = 0; i < c; i++)
				counter[s[i]]++;
			s += c;
			continue;
		} else if(c == 0x80) // 0x80 - end data
			break;
		else if(c < 0xC0) { // 0xBF - skip data
			s++;
		} else if(c == 0xC0) { // 0xC0 - shadow
			s++;
			c = *s % 4 ? *s % 4 : *(++s);
			s++;
		} else { // fill
			s++;
			if(c == 0xC1)
				c = *s++;
			else
				c -= 0xC0;
			counter[*s++] += c;
		}
	}
}

static void scan_v2(unsigned* counter, const unsigned char* s) {
}

static void scan_image(unsigned* counter, res_s res, int frame) {
	auto p = (icn*)get(res);
	if(!p || !p->count)
		return;
	auto& r = p->records[frame % p->count];
	auto d = (unsigned char*)p->records + r.offset;
	if(r.type == 0x20)
		scan_v2(counter, d);
	else
		scan_v1(counter, d);
}

unsigned calculate_pallette_use(unsigned char* indecies, res_s res) {
	unsigned counter[256] = {};
	auto pi = (icn*)get(res);
	for(auto i = 0; i < pi->count; i++)
		scan_image(counter, res, i);
	auto ps = indecies;
	for(unsigned i = 0; i < 256; i++) {
		if(counter[i] == 0)
			continue;
		*ps++ = i;
	}
	return ps - indecies;
}

static void test_font() {
	unsigned char indecies[256] = {};
	auto indecies_count = calculate_pallette_use(indecies, FONT);
	if(indecies_count == 0)
		return;
}

void util_main() {
	test_font();
}