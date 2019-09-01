#include "io.h"
#include "view.h"

namespace {
#pragma pack(push)
#pragma pack(1)
struct agg {
	struct namei {
		char			name[15];
	};
	struct record {
		unsigned		crc;
		unsigned		offset;
		unsigned		size;
	};
	unsigned short		count;
	record				records[1];
};
#pragma pack(pop)
}

static agg*				heroes2_agg;

static void* loadfile(const char* url) {
	io::file file(url, StreamRead);
	if(!file)
		return 0;
	unsigned size = file.getsize();
	auto p = new char[size];
	if(!p)
		return 0;
	memset(p, 0, size);
	file.read(p, size);
	return p;
}

const void* draw::get(res_s id) {
	if(id > LastResource)
		return 0;
	if(!heroes2_agg)
		heroes2_agg = (agg*)loadfile("data/heroes2.agg");
	if(!heroes2_agg)
		return 0;
	return (char*)heroes2_agg + heroes2_agg->records[id].offset;
}

const char* draw::getname(res_s id) {
	auto& e = heroes2_agg->records[heroes2_agg->count - 1];
	auto pn = (agg::namei*)((char*)heroes2_agg + e.offset + e.size);
	return pn[id].name;
}

void draw::extract(const char* url, res_s id) {
	auto pn = getname(id);
	if(!pn)
		return;
	char temp[512]; zprint(temp, "%1/%2", url, pn);
	io::file file(temp, StreamWrite);
	if(!file)
		return;
	auto& e = heroes2_agg->records[id];
	file.write((char*)heroes2_agg + e.offset, e.size);
}