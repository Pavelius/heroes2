#include "io.h"
#include "view.h"

namespace {
#pragma pack(push)
#pragma pack(1)
struct agg {
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