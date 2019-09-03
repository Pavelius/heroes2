#include "view.h"

using namespace draw;

const int				cell_wr = 22;
const int				cell_hr = 13;
const int				dialog_width = 304 - 54 - 16;
const int				padding = 16;

static unsigned char decode_ru[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//1
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,//2
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,//3
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,//4
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,//5
	64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,//6
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,//7
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//8
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//9
	0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0,//A
	0, 0, 0, 0, 0, 0, 0, 0, 161, 0, 0, 0, 0, 0, 0, 0,//B
	96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,//C
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
	129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
	145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
};
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

static animation		current_cursor;
static unsigned char	resources_frame[] = {6, 0, 1, 2, 3, 4, 5};
static unsigned char	bits[draw::width*draw::height];
rect					draw::clipping = {0, 0, draw::width, draw::height};
unsigned				draw::counter;
eventproc				draw::domodal;
res_s					draw::font = FONT;
unsigned char*			draw::font_color;
static const char*		skill_level_name[] = {"нет", "Базово", "Продвинуто", "Эксперт"};
static rect             status_rect;
static char             status_text[260];
static char             tooltips_text[260];
extern unsigned char    pallette[256][4];
extern unsigned char    pallette2[256];
extern unsigned char    pallette3[256];
extern unsigned char    pallette4[256];
int                     hot::param;
int                     hot::param2;
point                   hot::mouse;
int                     hot::key;
bool                    hot::pressed;
static bool				evil_interface;
static bool				break_modal;
static int				break_result;
unsigned char			draw::font_yellow[256];
unsigned char			draw::route_brown[256];
// System driver
bool					sys_create(const char* title, int milliseconds, bool fullscreen, unsigned char* bits, int width, int height); // Create system window
int						sys_input(bool wait); // Wait for system input
void*					sys_get_pallette(); // Get system pallette

static const char* zskipspcr(const char* p) {
	if(p)
		while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
			p++;
	return p;
}

const icn::record* draw::get(res_s res, int n) {
	auto p = (icn*)get(res);
	if(!p || !p->count)
		return 0;
	return p->records + (n % p->count);
}

int	draw::getwidth(res_s id, int n) {
	auto p = get(id, n);
	if(!p)
		return 0;
	return p->width;
}

int	draw::getheight(res_s id, int n) {
	auto p = get(id, n);
	if(!p)
		return 0;
	return p->height;
}

rect draw::get(res_s res, int n, int x, int y, unsigned flags) {
	icn* p = (icn*)get(res);
	if(!p || !p->count)
		return{0, 0, 0, 0};
	n = n % p->count;
	if((flags&AFNoOffset) == 0) {
		x += p->records[n].x;
		y += p->records[n].y;
	}
	return{x, y, x + p->records[n].width, y + p->records[n].height};
}

static unsigned char* skip_v1(unsigned char* src, int dy) {
	while(true) {
		unsigned char c = *src;
		if(c == 0) {
			++src;
			if(--dy <= 0)
				return src;
		} else if(c < 0x80)	// 0x7F - count data
			src += c + 1;
		else if(c == 0x80) // 0x80 - end data
			return src;
		else if(c < 0xC0) // 0xBF - skip data
			src++;
		else if(c == 0xC0)// 0xC0 - shadow
		{
			++src;
			unsigned char c = *src % 4 ? *src % 4 : *(++src);
			if(c) {
			}
			++src;
		} else if(c == 0xC1) // fill
			src += 3;
		else
			src += 2;
	}
}

// When NN is control byte then:
// 0x00 end line
// 0x01..0x7F copy NN bytes from 'src' to 'dst'
// 0x80 end of data
// 0x81..0xBF skip (NN-0x80) bytes on output
// 0xC0 shadow
// 0xC1 fill byte XX by value XX
// 0xC1..0xFF fill byte (NN-0xC0) by value XX
static void sprite_v1(unsigned char* dst, int scanline, const unsigned char* s, int height, const unsigned char* clip_x1, const unsigned char* clip_x2, unsigned char* change) {
	unsigned char* d = dst;
	unsigned char* de = dst + scanline * height;
	while(true) {
		register unsigned char c = *s;
		if(c == 0x00) {
			s++;
			dst += scanline;
			clip_x1 += scanline;
			clip_x2 += scanline;
			d = dst;
			if(d >= de)
				return;
		} else if(c < 0x80) // 0..0x7F - bytes copy from src
		{
			s++;
			// left clip part
			if(d + c <= clip_x1 || d > clip_x2) {
				d += c;
				s += c;
				continue;
			} else if(d < clip_x1) {
				unsigned char sk = clip_x1 - d;
				d += sk;
				s += sk;
				c -= sk;
			}
			// show visible part
			if(change) {
				do {
					if(d >= clip_x2)
						break;
					*d++ = change[*s++];
				} while(--c);
			} else {
				do {
					if(d >= clip_x2)
						break;
					*d++ = *s++;
				} while(--c);
			}
			// right clip part
			if(c) {
				s += c;
				d += c;
			}
		} else if(c == 0x80) // 0x80 - end data
			break;
		else if(c < 0xC0) // 0xBF - skip data
		{
			s++;
			d += c - 0x80;
		} else if(c == 0xC0)// 0xC0 - shadow
		{
			s++;
			c = *s % 4 ? *s % 4 : *(++s);
			s++;
			// left clip part
			if(d + c <= clip_x1 || d > clip_x2) {
				d += c;
				continue;
			} else if(d < clip_x1) {
				unsigned char sk = clip_x1 - d;
				d += sk;
				c -= sk;
			}
			// show visible part
			do {
				if(d >= clip_x2)
					break;
				//d++;
				*d++ = pallette4[*d];
			} while(--c);
		} else // fill
		{
			s++;
			if(c == 0xC1)
				c = *s++;
			else
				c -= 0xC0;
			unsigned char fill = *s++;
			if(change)
				fill = change[fill];
			// left clip part
			if(d + c <= clip_x1 || d > clip_x2) {
				d += c;
				continue;
			} else if(d < clip_x1) {
				unsigned char sk = clip_x1 - d;
				d += sk;
				c -= sk;
			}
			// show visible part
			do {
				if(d >= clip_x2)
					break;
				*d++ = fill;
			} while(--c);
		}
	}
}

static void sprite_v1m(unsigned char* dst, int scanline, const unsigned char* s, int height, const unsigned char* clip_x1, const unsigned char* clip_x2, unsigned char* change) {
	unsigned char* d = dst;
	unsigned char* de = dst + scanline * height;
	while(true) {
		register unsigned char c = *s;
		if(c == 0x00) {
			s++;
			dst += scanline;
			clip_x1 += scanline;
			clip_x2 += scanline;
			d = dst;
			if(d >= de)
				return;
		} else if(c < 0x80) // 0..0x7F - bytes copy from src
		{
			s++;
			// left clip part
			if(d - c > clip_x2 || d < clip_x1) {
				d -= c;
				s += c;
				continue;
			} else if(d > clip_x2) {
				unsigned char sk = d - clip_x2;
				d -= sk;
				s += sk;
				c -= sk;
			}
			// show visible part
			if(change) {
				do {
					if(d < clip_x1)
						break;
					*d-- = change[*s++];
				} while(--c);
			} else {
				do {
					if(d < clip_x1)
						break;
					*d-- = *s++;
				} while(--c);
			}
			// right clip part
			if(c) {
				s += c;
				d -= c;
			}
		} else if(c == 0x80) // 0x80 - end data
			break;
		else if(c < 0xC0) // 0xBF - skip data
		{
			s++;
			d -= c - 0x80;
		} else if(c == 0xC0)// 0xC0 - shadow
		{
			s++;
			c = *s % 4 ? *s % 4 : *(++s);
			s++;
			// left clip part
			if(d - c > clip_x2 || d < clip_x1) {
				d -= c;
				continue;
			} else if(d > clip_x2) {
				unsigned char sk = d - clip_x2;
				d -= sk;
				c -= sk;
			}
			// show visible part
			do {
				if(d < clip_x1)
					break;
				*d-- = pallette4[*d];
			} while(--c);
		} else // fill
		{
			s++;
			if(c == 0xC1)
				c = *s++;
			else
				c -= 0xC0;
			unsigned char fill = *s++;
			if(change)
				fill = change[fill];
			// left clip part
			if(d - c > clip_x2 || d < clip_x1) {
				d -= c;
				continue;
			} else if(d > clip_x2) {
				unsigned char sk = d - clip_x2;
				d -= sk;
				c -= sk;
			}
			// show visible part
			do {
				if(d < clip_x1)
					break;
				*d-- = fill;
			} while(--c);
		}
	}
}

static void sprite_v2(unsigned char* dst, int scanline, const unsigned char* s, int height, unsigned char* clip_x1, unsigned char* clip_x2) {
	unsigned char* d = dst;
	unsigned char* de = dst + scanline * height;
	while(true) {
		unsigned char c = *s;
		if(c == 0) {
			s++;
			dst += scanline;
			clip_x1 += scanline;
			clip_x2 += scanline;
			d = dst;
			if(d >= de)
				return;
		} else if(c < 0x80) // 0..0x7F - bytes from src
		{
			unsigned char c = *s++;
			// left clip part
			if(d + c <= clip_x1 || d > clip_x2) {
				d += c;
				continue;
			} else if(d < clip_x1) {
				unsigned char sk = clip_x1 - d;
				d += sk;
				c -= sk;
			}
			// show visible part
			do {
				if(d >= clip_x2)
					break;
				*d++ = pallette4[*d];
			} while(--c);
		} else if(c == 0x80) // 0x80 - end data
			break;
		else
			d += (*s++) - 0x80;
	}
}

static void sprite_v2m(unsigned char* dst, int scanline, const unsigned char* s, int height, unsigned char* clip_x1, unsigned char* clip_x2) {
	unsigned char* d = dst;
	unsigned char* de = dst + scanline * height;
	while(true) {
		unsigned char c = *s;
		if(c == 0) {
			s++;
			dst += scanline;
			clip_x1 += scanline;
			clip_x2 += scanline;
			d = dst;
			if(d >= de)
				return;
		} else if(c < 0x80) // 0..0x7F - bytes from src
		{
			unsigned char c = *s++;
			// left clip part
			if(d - c > clip_x2 || d < clip_x1) {
				d -= c;
				continue;
			} else if(d > clip_x2) {
				unsigned char sk = d - clip_x2;
				d -= sk;
				c -= sk;
			}
			// show visible part
			do {
				if(d < clip_x1)
					break;
				*d-- = pallette4[*d];
			} while(--c);
		} else if(c == 0x80) // 0x80 - end data
			break;
		else
			d -= (*s++) - 0x80;
	}
}

void draw::image(int x, int y, res_s res, int frame, unsigned flags, unsigned char* change) {
	auto p = (icn*)get(res);
	if(!p || !p->count)
		return;
	icn::record& r = p->records[frame % p->count];
	unsigned char* d = (unsigned char*)p->records + r.offset;
	if((flags&AFNoOffset) == 0) {
		if(flags&AFMirror)
			x -= r.x;
		else
			x += r.x;
		y += r.y;
	}
	if(flags&AFCenter)
		x -= r.width / 2;
	if(flags&AFCenterV)
		y -= r.height / 2;
	if(y + r.height < clipping.y1 || y >= clipping.y2)
		return;
	if(r.type == 0x20) {
		if(flags&AFMirror) {
			sprite_v2m(ptr(x, y), width, d, clipping.y2 - y,
				ptr(clipping.x1, 0),
				ptr(clipping.x2, height));
		} else {
			sprite_v2(ptr(x, y), width, d, clipping.y2 - y,
				ptr(clipping.x1, y),
				ptr(clipping.x2, y));
		}
	} else {
		if(y < clipping.y1) {
			d = skip_v1(d, clipping.y1 - y);
			y = clipping.y1;
		}
		if(flags&AFMirror) {
			sprite_v1m(ptr(x, y), width, d, clipping.y2 - y,
				ptr(clipping.x1, y),
				ptr(clipping.x2, y),
				change);
		} else {
			if(x + width < clipping.x1 || x >= clipping.x2)
				return;
			sprite_v1(ptr(x, y), width, d, clipping.y2 - y,
				ptr(clipping.x1, y),
				ptr(clipping.x2, y),
				change);
		}
	}
}

static void colorize(unsigned char* pallette, int index, int count) {
	unsigned char b1[4];
	unsigned char* p1 = &pallette[index * 4];
	b1[0] = p1[0];
	b1[1] = p1[1];
	b1[2] = p1[2];
	b1[3] = p1[3];
	//
	for(int i = 1; i < count; i++) {
		p1[(i - 1) * 4 + 0] = p1[i * 4 + 0];
		p1[(i - 1) * 4 + 1] = p1[i * 4 + 1];
		p1[(i - 1) * 4 + 2] = p1[i * 4 + 2];
		p1[(i - 1) * 4 + 3] = p1[i * 4 + 3];
	}
	p1[(count - 1) * 4 + 0] = b1[0];
	p1[(count - 1) * 4 + 1] = b1[1];
	p1[(count - 1) * 4 + 2] = b1[2];
	p1[(count - 1) * 4 + 3] = b1[3];
}

static void colorize() {
	auto pal = (unsigned char*)sys_get_pallette();
	colorize(pal, 0xD6, 4);
	colorize(pal, 0xDA, 4);
	colorize(pal, 0xEE, 4);
	colorize(pal, 0xE7, 4);
}

unsigned char* draw::ptr(int x, int y) {
	return &bits[y*draw::width + x];
}

void draw::hexagon(int x, int y, unsigned char m) {
	// border parts
	line(x - cell_wr, y - cell_hr, x - cell_wr, y + cell_hr - 1, m);
	line(x + cell_wr, y - cell_hr, x + cell_wr, y + cell_hr - 1, m);
	// left lines
	line(x - cell_wr, y - cell_hr, x, y - cell_hd / 2, m);
	line(x - cell_wr, y + cell_hr - 1, x, y + cell_hd / 2 - 1, m);
	// right lines
	line(x, y - cell_hd / 2, x + cell_wr, y - cell_hr, m);
	line(x, y + cell_hd / 2 - 1, x + cell_wr, y + cell_hr - 1, m);
}

void draw::hexagonf(int x, int y, unsigned char intense) {
	static int points[] = {1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2};
	shadow(x - cell_wr, y - cell_hr, x + cell_wr - 1, y + cell_hr - 1, intense);
	int j = 0;
	for(unsigned i = 0; i < sizeof(points) / sizeof(points[0]); i++) {
		j += points[i];
		shadow(x - cell_wr + j,
			y - cell_hr - 1 - i,
			x + cell_wr - j,
			y - cell_hr - 1 - i,
			intense);
	}
	j = 0;
	for(unsigned i = 1; i < sizeof(points) / sizeof(points[0]); i++) {
		j += points[i];
		shadow(x - cell_wr + j,
			y + cell_hr - 1 + i,
			x + cell_wr - j,
			y + cell_hr - 1 + i,
			intense);
	}
}

void draw::rectf(const rect& rc, unsigned char m) {
	int w1 = iabs(rc.x1 - rc.x2);
	unsigned char* p = ptr(imin(rc.x1, rc.x2), imin(rc.y1, rc.y2));
	int scan_line = width - w1 - 1;
	for(int h = iabs(rc.y1 - rc.y2); h >= 0; h--) {
		for(int w = w1; w >= 0; w--)
			*p++ = m;
		p += scan_line;
	}
}

void draw::rectb(const rect& rc, unsigned char m) {
	line(rc.x1, rc.y1, rc.x2, rc.y1, m);
	line(rc.x1, rc.y2, rc.x2, rc.y2, m);
	line(rc.x1, rc.y1, rc.x1, rc.y2, m);
	line(rc.x2, rc.y1, rc.x2, rc.y2, m);
}

static int spacewidth(res_s id) {
	return getwidth(id, 'i' - 0x20);
}

int draw::texth() {
	return getheight(font, 'I' - 0x20) + 2;
}

int draw::textbc(const char* string, int width) {
	int p = -1;
	int w = 0;
	const char* s1 = string;
	res_s id = font;
	while(true) {
		unsigned char s = *((unsigned char*)s1++);
		if(s == 0x20 || s == 9)
			p = s1 - string;
		else if(s == 0) {
			p = s1 - string - 1;
			break;
		} else if(s == 10 || s == 13) {
			p = s1 - string;
			break;
		}
		w += getwidth(id, decode_ru[s]);
		if(w > width)
			break;
	}
	if(p == -1)
		p = s1 - string;
	return p;
}

int draw::textw(const char* string, int count) {
	int result = 0;
	if(count == -1)
		count = zlen(string);
	res_s id = font;
	while(count) {
		unsigned char ch = *string++;
		if(ch <= 0x20)
			result += spacewidth(id);
		else {
			ch = decode_ru[ch];
			result += getwidth(id, ch);
		}
		count--;
	}
	return result;
}

void draw::text(int x, int y, const char* string, int count) {
	if(count == -1)
		count = zlen(string);
	res_s id = font;
	const char* pe = string + count;
	while(string < pe) {
		unsigned char ch = (unsigned char)*string++;
		if(ch <= 0x20)
			x += spacewidth(id);
		else {
			ch = decode_ru[ch];
			image(x, y, id, ch, 0, font_color);
			x += getwidth(id, ch);
		}
	}
}

int draw::textm(int x, int y, int width, align_s jf, const char* string) {
	int y1 = y;
	while(string[0]) {
		int c = textbc(string, width);
		if(!c)
			break;
		text(x, y1, width, jf, string, c);
		y1 += texth();
		string = zskipspcr(string + c);
	}
	return y1 - y;
}

int draw::texth(const char* string, int width) {
	int result = 0;
	while(string[0]) {
		int c = textbc(string, width);
		if(!c)
			break;
		result += texth();
		string = zskipspcr(string + c);
	}
	return result;
}

void draw::text(int x, int y, int width, align_s align, const char* string, int count) {
	switch(align) {
	case AlignLeft: return text(x, y, string, count);
	case AlignRight: return text(x + width - textw(string, count), y, string, count);
	case AlignCenter: return text(x + (width - textw(string, count)) / 2, y, string, count);
	}
}

inline void line_v1(unsigned char* p1, unsigned char* p2, unsigned char m, int scan_line) {
	while(p1 <= p2) {
		*p1 = m;
		p1 += scan_line;
	}
}

void draw::pixel(int x, int y, unsigned char a) {
	if(x >= clipping.x1 && x < clipping.x2 && y >= clipping.y1 && y < clipping.y2)
		*ptr(x, y) = a;
}

void draw::line(int x1, int y1, int x2, int y2, unsigned char m) {
	if(y1 == y2) {
		if(y1 >= clipping.y2 || y1 < clipping.y1)
			return;
		auto n1 = imin(x1, x2);
		auto n2 = imax(x1, x2);
		if(n1 < clipping.x1)
			n1 = clipping.x1;
		if(n2 >= clipping.x2)
			n2 = clipping.x2 - 1;
		if(n1 > n2)
			return;
		line_v1(ptr(n1, y1), ptr(n2, y1), m, 1);
	} else if(x1 == x2) {
		if(x1 >= clipping.x2 || x1 < clipping.x1)
			return;
		auto n1 = imin(y1, y2);
		auto n2 = imax(y1, y2);
		if(n1 < clipping.y1)
			n1 = clipping.y1;
		if(n2 >= clipping.y2)
			n2 = clipping.y2 - 1;
		if(n1 > n2)
			return;
		line_v1(ptr(x1, n1), ptr(x1, n2), m, width);
	} else {
		const int deltaX = iabs(x2 - x1);
		const int deltaY = iabs(y2 - y1);
		const int signX = x1 < x2 ? 1 : -1;
		const int signY = y1 < y2 ? 1 : -1;
		//
		int error = deltaX - deltaY;
		//
		pixel(x2, y2, m);
		while(x1 != x2 || y1 != y2) {
			pixel(x1, y1, m);
			const int error2 = error * 2;
			//
			if(error2 > -deltaY) {
				error -= deltaY;
				x1 += signX;
			}
			if(error2 < deltaX) {
				error += deltaX;
				y1 += signY;
			}
		}
	}
}

void draw::shadow(int x1, int y1, int x2, int y2, int intense) {
	int w1 = iabs(x1 - x2);
	unsigned char* p = ptr(imin(x1, x2), imin(y1, y2));
	int scan_line = width - w1 - 1;
	for(int h = iabs(y1 - y2); h >= 0; h--) {
		unsigned char* p1 = p + w1;
		switch(intense) {
		case 1:
			while(p <= p1) {
				*p = pallette4[*p];
				p++;
			}
			break;
		case 2:
			while(p <= p1) {
				*p = pallette2[pallette4[*p]];
				p++;
			}
			break;
		default:
			while(p <= p1) {
				*p = pallette3[*p];
				p++;
			}
			break;
		}
		p += scan_line;
	}
}

inline void sprite_v3(unsigned char* dst, unsigned char* src, int width, int height, int scan) {
	while(height) {
		for(int i = 0; i < width; i++)
			*dst++ = *src++;
		dst += scan;
		height--;
	}
}

inline void sprite_v3m(unsigned char* dst, unsigned char* src, int width, int height, int scan) {
	while(height) {
		for(int i = 0; i < width; i++)
			*dst-- = *src++;
		dst += scan;
		height--;
	}
}

void draw::imagt(int x, int y, res_s id, int n, int mode) {
	auto p = (til*)get(id);
	if(!p)
		return;
	if(n >= p->count)
		return;
	switch(mode) {
	case 1:
		sprite_v3(ptr(x, y + p->height - 1), (unsigned char*)p + sizeof(til) + p->width*p->height*n,
			p->width, p->height, -width - p->width);
		break;
	case 2:
		sprite_v3m(ptr(x + p->width - 1, y), (unsigned char*)p + sizeof(til) + p->width*p->height*n,
			p->width, p->height, width + p->width);
		break;
	case 3:
		sprite_v3m(ptr(x + p->width - 1, y + p->height - 1), (unsigned char*)p + sizeof(til) + p->width*p->height*n,
			p->width, p->height, -width + p->width);
		break;
	default:
		sprite_v3(ptr(x, y), (unsigned char*)p + sizeof(til) + p->width*p->height*n,
			p->width, p->height, width - p->width);
		break;
	}
}

void draw::setevil(bool value) {
	evil_interface = value;
}

res_s draw::isevil(res_s evil, res_s good) {
	return evil_interface ? evil : good;
}

void draw::image(int x, int y, monster_s id, int n, unsigned flags, int mode) {
	image(x, y, MINIMON, id * 9, flags);
	switch(mode) {
	case 1:
		image(x, y, MINIMON, id * 9 + 1 + monster_animation_cicl1[(counter / 2 + n * n) % (sizeof(monster_animation_cicl1) / sizeof(monster_animation_cicl1[0]))], flags);
		break;
	default:
		image(x, y, MINIMON, id * 9 + 1 + monster_animation_cicle[(counter / 2 + n * n) % (sizeof(monster_animation_cicle) / sizeof(monster_animation_cicle[0]))], flags);
		break;
	}
}

void draw::image(int x, int y, hero_s id, bool large) {
	if(large)
		image(x, y, res_s(PORT0000 + id), 0, 0, 0);
	else
		image(x, y, MINIPORT, id, AFNoOffset);
}

void draw::image(int x, int y, skill_s id, int level) {
	auto w = getwidth(SECSKILL, 0);
	image(x, y, SECSKILL, id + 1);
	const char* p = getstr(id);
	draw::text(x + (w - draw::textw(p)) / 2, y + 5, p);
	p = skill_level_name[level];
	draw::text(x + (w - draw::textw(p)) / 2, y + 3 + 52, p);
}

void draw::image(const rect& rc, res_s id, int frame, const char* title, const char* tips) {
	auto p = get(id, frame);
	if(!p)
		return;
	auto w = rc.x2 - rc.x1;
	auto pf = font;
	font = SMALFONT;
	image(rc.x1 + (w - p->width) / 2, rc.y2 - texth() - p->height - 2, id, frame, AFNoOffset);
	if(title)
		text(rc.x1 + (w - textw(title)) / 2, rc.y2 - texth(), title);
	if(tips) {
		if(mousein(rc))
			status(tips);
	}
	font = pf;
}

void draw::image(const rect& rc, resource_s id, int count, const char* tips) {
	char temp[260]; zprint(temp, "%1i", count);
	image(rc, RESOURCE, resources_frame[id - FirstResource], temp, tips);
	//rectb(rc, 0x10);
}

static void show_tooltips() {
	message(tooltips_text, 0, 0, NoButtons);
}

void draw::tooltips(const char* header, const char* format, ...) {
	szprintv(tooltips_text, tooltips_text + sizeof(tooltips_text) - 1,
		format, xva_start(format));
	execute(show_tooltips);
}

void draw::button(int x, int y, res_s res, eventproc proc, const buttoni& decor, int key, const char* tips) {
	if(button(x, y, res, decor, key, tips))
		execute(proc);
}

bool draw::button(int x, int y, res_s res, const buttoni& decor, int key, const char* tips) {
	static point point_cashe;
	int i = decor.normal;
	rect rc = get(res, i, x, y, 0);
	auto need_execute = false;
	if(mousein(rc)) {
		i = decor.hilite;
		if(hot::pressed)
			i = decor.pressed;
		if(tips) {
			if(status_rect)
				status(tips);
			else if(hot::key == MouseRight && hot::pressed)
				tooltips("Информация", tips);
		}
		if(hot::key == MouseLeft) {
			if(!hot::pressed) {
				if(point_cashe.x == rc.x1 && point_cashe.y == rc.y1)
					need_execute = true;
			} else {
				point_cashe.x = rc.x1;
				point_cashe.y = rc.y1;
			}
		}
	}
	image(x, y, res, i, 0);
	if(key && hot::key == key)
		need_execute = true;
	return need_execute;
}

bool draw::mousein(const rect& rc) {
	if(!hot::mouse.in(clipping))
		return false;
	return hot::mouse.in(rc);
}

bool draw::create(const char* title, unsigned milliseconds, bool fullscreen) {
	// Set pallette
	auto pal3 = (unsigned char*)get(PalKB);
	if(!pal3)
		return false; // Not found AGG
	auto pal4 = (unsigned char*)sys_get_pallette();
	for(int i = 0; i < 256; i++) {
		int i3 = i * 3;
		int i4 = i * 4;
		pal4[i4 + 2] = pal3[i3 + 0] << 2;
		pal4[i4 + 1] = pal3[i3 + 1] << 2;
		pal4[i4 + 0] = pal3[i3 + 2] << 2;
	}
	// Set colors
	colorize();
	// Create Window
	return sys_create(title, milliseconds, fullscreen, bits, width, height);
}

void draw::status(const char* format, ...) {
	szprintv(status_text, status_text + sizeof(status_text) - 1, format, xva_start(format));
}

void draw::status(const rect& rc) {
	status_rect = rc;
}

void draw::setcursor(res_s res, unsigned char frame) {
	current_cursor.set(res, frame);
}

static void standart_domodal() {
	if(status_rect && status_text[0]) {
		text(status_rect.x1 + (status_rect.width() - textw(status_text)) / 2,
			status_rect.y1 + (status_rect.height() - texth()) / 2,
			status_text);
	}
	image(hot::mouse.x + current_cursor.pos.x,
		hot::mouse.y + current_cursor.pos.y,
		current_cursor.res, current_cursor.frame,
		current_cursor.flags);
#ifdef _DEBUG
	//static bool show_debug_view = true;
	//if(hot::key == Ctrl + Alpha + 'D')
	//	show_debug_view = !show_debug_view;
	//if(show_debug_view) {
	//	draw::state push;
	//	font = SMALFONT;
	//	char temp[100]; zprint(temp, "%1i, %2i", hot::mouse.x, hot::mouse.y);
	//	text(2, 2, temp);
	//}
#endif
	hot::key = sys_input(true);
	switch(hot::key) {
	case 0: exit(0); break;
	case InputTimer:
		colorize();
		counter++;
		break;
	}
}

void draw::breakmodal(int result) {
	break_modal = true;
	break_result = result;
}

void draw::buttoncancel() {
	breakmodal(0);
}

void draw::buttonok() {
	breakmodal(1);
}

int draw::getresult() {
	return break_result;
}

bool draw::ismodal() {
	status_rect.clear();
	status_text[0] = 0;
	current_cursor.set(ADVMCO, 0);
	domodal = standart_domodal;
	if(!break_modal)
		return true;
	break_modal = false;
	return false;
}

void draw::execute(eventproc proc, int param) {
	domodal = proc;
	hot::param = param;
	hot::key = 0;
}

int draw::textf(int x, int y, int width, const char* p) {
	state push;
	auto y4 = y;
	p = zskipspcr(p);
	auto start = p;
	while(*p) {
		if(p[0] == '#' && p[1] == '#' && (p == start || p[-1] == '\n')) {
			p += 2;
			font_color = font_yellow;
		}
		if(p[0] == '$' && p[1] == '$') {
			y += texth() / 2;
			variantcol variants[32]; unsigned count;
			p = string::parse(p, variants, count);
			y += imagex(x, y, width, variants, count) + texth() / 2;
		} else {
			auto c = textbc(p, width);
			text(x, y, width, AlignCenter, p, c);
			y += texth();
			p += c;
		}
		if(p != start && p[-1] == '\n')
			font_color = 0;
		p = zskipspcr(p);
	}
	return y - y4;
}

int draw::textf(int width, const char* string) {
	draw::state push;
	draw::clipping.clear();
	return textf(0, 0, width, string);
}

static int current_border;

static void increase() {
	*((int*)hot::param) = *((int*)hot::param) + 1;
	if(*((int*)hot::param) > current_border)
		*((int*)hot::param) = current_border;
}

static void decrease() {
	*((int*)hot::param) = *((int*)hot::param) - 1;
	if(*((int*)hot::param) <= current_border)
		*((int*)hot::param) = current_border;
}

static void maximize() {
	*((int*)hot::param) = current_border;
}

void draw::edit(int x, int y, int width, int& value, int maximum, int minimum, const controli& up, const controli& down, const controli& max) {
	if(hot::key == KeyBackspace)
		value = value / 10;
	else if(hot::key == InputSymbol && hot::param >= '0' && hot::param <= '9')
		value = value * 10 + hot::param - '0';
	if(value < minimum)
		value = minimum;
	if(value > maximum)
		value = maximum;
	char temp[32]; zprint(temp, "%1i", value);
	text(x + (width - textw(temp)) / 2, y, temp, -1);
	if(max) {
		if(button(max.x, max.y, max.res, {max.normal, max.hilited, max.pressed}, Alpha + Ctrl + 'M', 0)) {
			current_border = maximum;
			execute(maximize, (int)&value);
		}
	}
	if(up) {
		if(button(up.x, up.y, up.res, {up.normal, up.hilited, up.pressed}, KeyUp, 0)) {
			current_border = maximum;
			execute(increase, (int)&value);
		}
	}
	if(down) {
		if(button(down.x, down.y, down.res, {down.normal, down.hilited, down.pressed}, KeyDown, 0)) {
			current_border = minimum;
			execute(decrease, (int)&value);
		}
	}
	//rectb({x, y, x + width, y + texth()}, 0x10);
}

int draw::dialog(int height, int* bottom_position) {
	auto icn = isevil(BUYBUILE, BUYBUILD);
	int h4 = getheight(icn, 4); // top
	int h5 = getheight(icn, 5); // middle
	int h6 = getheight(icn, 6); // bottom
	int p1 = h4 - h5 / 2;
	int p2 = h6 - h5 / 2;
	int w4 = getwidth(icn, 4); // top
	int w0 = getwidth(icn, 0); // middle
	int count = height / h5;
	if(count < 0)
		count = 0;
	int x = (draw::width - (w4 + w0)) / 2 - 8;
	int y = (draw::height - (h4 + h5 * count + h6 - 8)) / 2;
	if(icn == BUYBUILD) {
		image(x + 1, y, icn, 4, AFNoOffset);
		image(x + w4 + 1, y, icn, 0, AFNoOffset);
	} else {
		image(x, y, icn, 4, AFNoOffset);
		image(x + w4, y, icn, 0, AFNoOffset);
	}
	y += h4;
	//line(x + 16, y - 1, x + w4 * 2 - 16, y - 1, 0xF0);
	int y1 = y - h5 / 2;
	for(int i = 0; i < count; i++) {
		draw::image(x, y, icn, 5, AFNoOffset);
		draw::image(x + getwidth(icn, 5), y, icn, 1, AFNoOffset);
		y += h5;
	}
	//line(x + 16, y - 1, x + w4 * 2 - 16, y - 1, 0xF0);
	if(bottom_position)
		*bottom_position = y + h5 / 2;
	draw::image(x, y, icn, 6, AFNoOffset);
	draw::image(x + getwidth(icn, 6), y, icn, 2, AFNoOffset);
	//rectb({x, y1, x + w4 * 2, y1 + height}, 0x40);
	return y1;
}

void picture::setsize(res_s res, unsigned char frame) {
	size.x = getwidth(res, frame) + 8;
	size.y = getheight(res, frame);
}

void picture::set(const variant e, int value) {
	format = 0;
	switch(e.type) {
	case Artifact:
		res = ARTIFACT; frame = e.artifact;
		setsize(RESOURCE, 7);
		break;
	case Building:
		res = getbuildings(kind_s(value));
		frame = castlei::getframe(e.building);
		setsize();
		size.y = getheight(BLDGXTRA, 0);
		break;
	case Monster:
		res = res_s(MONH0000 + e.monster - FirstMonster); frame = 0;
		setsize(STRIP, 12);
		break;
	case Resource:
		res = RESOURCE; frame = resources_frame[e.resource];
		setsize();
		size.y += getheight(SMALFONT, 'I' - 0x20) + 4;
		format = "%1i";
		break;
	case Hero:
		res = res_s(PORT0000 + e.hero); frame = 0;
		setsize();
		break;
	case Ability:
		res = PRIMSKIL; frame = e.ability;
		setsize(PRIMSKIL, 4);
		break;
	case Skill:
		res = SECSKILL; frame = e.skill + 1;
		setsize(SECSKILL, 15);
		break;
	default:
		res = NoRes; frame = 0;
		size.x = 0;
		size.y = 0;
		break;
	}
}

int picture::gettotalheight(unsigned count, int width_per_line) const {
	auto p = this;
	auto pe = p + count;
	auto r = 0;
	while(p < pe) {
		int width = 0, height = 0;
		auto c = getsize(count, width, height, width_per_line);
		p += c;
		count -= c;
		r += height;
	}
	return r;
}

unsigned picture::getsize(unsigned count, int& width, int& height, int width_per_line) const {
	auto p = this;
	auto pe = p + count;
	while(p < pe) {
		if(width_per_line && (width + p->size.x) > width_per_line)
			break;
		width += p->size.x;
		if(height < p->size.y)
			height = p->size.y;
		p++;
	}
	return p - this;
}

static unsigned getsize(const variantcol* source, const variantcol* pe, int& width, int& height, int width_per_line) {
	auto p = source;
	while(p < pe) {
		picture a; a.set(p->element, p->count);
		if(width_per_line && (width + a.size.x) > width_per_line)
			break;
		width += a.size.x;
		if(height < a.size.y)
			height = a.size.y;
		p++;
	}
	return p - source;
}

static int getsize(const variantcol* source, unsigned count, int width_per_line) {
	state push;
	font = SMALFONT;
	int y = 0;
	while(count > 0) {
		auto h1 = 0;
		auto w1 = 0;
		auto line_count = getsize(source, source + count, w1, h1, width_per_line);
		y += h1;
		count -= line_count;
	}
	return y;
}

void unit_info_monster();
int index_by_type(kind_s id);

void picture::render(int x, int y, res_s res, int frame) const {
	image(x + (size.x - getwidth(res, frame)) / 2, y, res, frame, AFNoOffset);
}

void picture::clear() {
	memset(this, 0, sizeof(*this));
}

void picture::paint(int x, int y, int h1, variant element, int count) const {
	auto pf = font;
	font = SMALFONT;
	char temp[64];
	auto z = y + h1 - size.y;
	switch(element.type) {
	case Artifact:
		render(x, z, RESOURCE, 7);
		render(x, z + 6, res, frame + 1);
		break;
	case Monster:
		render(x, z + 6, STRIP, index_by_type(bsmeta<monsteri>::elements[element.monster].type));
		render(x, y + h1 - getheight(res, frame) - 10, res, frame);
		render(x, z, STRIP, 12);
		if(count) {
			zprint(temp, "%1i", count);
			draw::state push;
			font = FONT;
			auto w1 = draw::textw(temp);
			auto x1 = x + (size.x + getwidth(STRIP, 12)) / 2;
			text(x1 - textw(temp) - 10, y + h1 - texth() - 8, temp);
		}
		break;
	case Building:
		render(x, z, BLDGXTRA, 0);
		render(x, z + 2, res, frame);
		if(true) {
			auto p = getstr(element.building, kind_s(count));
			text(x + (size.x - textw(p)) / 2, y + 61, p);
			font = pf;
		}
		break;
	case Ability:
		render(x, z, PRIMSKIL, 4);
		render(x, z + 6, res, frame);
		break;
	case Skill:
		render(x, z, SECSKILL, 15);
		render(x, z + 2, res, frame);
		if(count) {
			const char* p = getstr(element.skill);
			text(x + (size.x - textw(p)) / 2, z + 6, p);
			p = skill_level_name[count];
			text(x + (size.x - textw(p)) / 2, z + 57, p);
		}
		break;
	default:
		render(x, z, res, frame);
		break;
	}
	if(format) {
		auto t = texth();
		zprint(temp, format, count);
		text(x + (size.x - textw(temp)) / 2, y + h1 - t, temp);
	}
	font = pf;
}

int draw::imagex(int x, int y, int width, const variantcol* source, unsigned count) {
	state push;
	font = SMALFONT;
	auto y1 = y;
	while(count > 0) {
		auto h1 = 0;
		auto w1 = 0;
		auto line_count = getsize(source, source + count, w1, h1, width);
		auto pe = source + line_count;
		auto x1 = x + (width - w1) / 2;
		for(auto p = source; p < pe; p++) {
			picture a; a.set(p->element, p->count);
			a.paint(x1, y, h1, p->element, p->count);
			x1 += a.size.x;
		}
		y += h1;
		source += line_count;
		count -= line_count;
	}
	return y - y1;
}

static int addcost(variantcol* result, const costi& source) {
	int count = 0;
	for(auto i = Gold; i <= Gems; i = (resource_s)(i + 1)) {
		if(!source.data[i])
			continue;
		result[count].element = i;
		result[count].count = source.data[i];
		count++;
	}
	return count;
}

int draw::image(int x, int y, int width, const costi& source) {
	variantcol col[Gems + 1];
	auto count = addcost(col, source);
	return imagex(x, y, width, col, count);
	//rectb({x, y, x + width, y + h}, 0x10);
}

static void buttonbreak() {
	breakmodal(hot::param);
}

int draw::message(const char* format, const variantcol* source, unsigned count, button_s mode) {
	draw::screenshoot surface;
	draw::state push;
	font = FONT;
	auto bld_h = getheight(BLDGXTRA, 0);
	auto bld_w = getwidth(BLDGXTRA, 0);
	auto ic1 = isevil(SYSTEME, SYSTEM);
	auto th = textf(dialog_width, format);
	auto x = (width - dialog_width) / 2;
	auto h1 = th;
	if(mode != NoButtons)
		h1 += getheight(ic1, 1) + 8;
	auto variant_height = 0;
	if(count == 0)
		count = 1;
	if(source) {
		variant_height = getsize(source, count, dialog_width);
		h1 += variant_height + 8;
	}
	auto vw = dialog_width / count;
	while(ismodal()) {
		surface.restore();
		auto y2 = 0;
		auto y1 = dialog(h1, &y2);
		textf((width - dialog_width) / 2, y1, dialog_width, format);
		y1 = y2 - getheight(ic1, 1);
		if(source) {
			auto n = x;
			for(unsigned i = 0; i < count; i++) {
				picture vr;
				vr.set(source[i].element, source[i].count);
				vr.paint(n + (vw - vr.size.x) / 2, y1 - variant_height - 8, variant_height,
					source[i].element, source[i].count);
				n += vw;
			}
			if(count == 2) {
				auto fp = font;
				font = SMALFONT;
				auto p = "или";
				text((width - textw(p)) / 2, y1 - variant_height / 2, p);
				font = fp;
			}
		}
		int bw, x2;
		switch(mode) {
		case ButtonYesNo:
			button(x + 10, y1, ic1, buttonok, {5, 5, 6}, KeyEnter);
			button(x + dialog_width - 10 - getwidth(ic1, 7), y1, ic1, buttoncancel, {7, 7, 8}, KeyEscape);
			break;
		case ButtonOK:
			button((width - getwidth(ic1, 1)) / 2, y1, ic1, buttonok, {1, 1, 2}, KeyEnter);
			break;
		case ButtonLearn:
			bw = getwidth(ic1, 9);
			x2 = x;
			for(unsigned i = 0; i < count; i++) {
				if(button(x2 + (vw - bw) / 2, y1, ic1, {9, 9, 10}))
					execute(buttonbreak, i);
				x2 += vw;
			}
			break;
		}
		if(mode == NoButtons)
			setcursor(NoRes, 0);
		domodal();
		if(mode == NoButtons) {
			if(hot::key == MouseLeft || hot::key == MouseRight)
				buttoncancel();
		}
	}
	return getresult();
}

void playeri::message(const char* format) {
	draw::message(format, 0, 0, ButtonOK);
}

bool playeri::confirm(const char* format) {
	return draw::message(format, 0, 0, ButtonYesNo) != 0;
}

void playeri::tooltips(const char* format) {
	draw::message(format, 0, 0, NoButtons);
}

void draw::quicktips(int x, int y, const char* format) {
	state push;
	screenshoot screen;
	const auto res = QWIKINFO;
	font = SMALFONT;
	auto sx = getwidth(res, 0);
	auto sy = getheight(res, 0);
	auto x1 = x - sx / 2 - 6;
	auto y1 = y - sy / 2 + 6;
	auto tw = sx - 13 - 24;
	auto th = textf(tw, format);
	if(x1 < 0)
		x1 = 0;
	if(x1 + sx >= width)
		x1 = width - sx - 1;
	if(y1 < 0)
		y1 = 0;
	if(y1 + sy >= height)
		y1 = height - sy - 1;
	while(ismodal()) {
		screen.restore();
		image(x1, y1, res, 0);
		textf(x1 + 6 + (sx - tw) / 2, y1 - 6 + (sy - th) / 2, tw, format);
		domodal();
		if(hot::key == MouseRight || hot::key == MouseLeft || hot::key == MouseLeftDBL)
			buttoncancel();
	}
}

void draw::splitter(int x, int y, res_s res, int& value, int minimum, int maximum) {
	static int drag_id;
	unsigned char body = 1, left = 3, right = 5, bar = 2, pressed = 1;
	auto right_offset = 0, left_offset = 6, bar_offset = 4;
	if(value < minimum)
		value = minimum;
	if(value > maximum)
		value = maximum;
	if(minimum == maximum)
		return;
	auto bar_width = getwidth(res, bar);
	auto body_width = getwidth(res, body);
	auto height = getheight(res, body);
	auto mx1 = body_width - left_offset - getwidth(res, left) - getwidth(res, right) - bar_width - bar_offset * 2;
	if(mx1 < 0)
		return;
	auto mx = maximum - minimum;
	if(mx <= 0)
		return;
	auto p0 = getwidth(res, left) + left_offset + bar_offset;
	auto px = x + p0 + value * mx1 / mx;
	auto h1 = getheight(res, body);
	image(x, y, res, body);
	image(px, y + bar_offset, res, bar);
	if(drag_id) {
		if(hot::key == MouseLeft) {
			if(!hot::pressed)
				drag_id = 0;
		} else if(drag_id == (int)&value && hot::key == MouseMove) {
			if(hot::mouse.x < px) {
				current_border = minimum;
				execute(decrease, (int)&value);
			} else if(hot::mouse.x > px + (mx1 + mx - 1) / mx) {
				current_border = maximum;
				execute(increase, (int)&value);
			}
		}
	} else {
		if(hot::key == MouseLeft && hot::pressed) {
			if(mousein({x + p0, y, px, y + height})) {
				current_border = minimum;
				execute(decrease, (int)&value);
			} else if(mousein({px + bar_width, y, x + p0 + mx1 + bar_width, y + height})) {
				current_border = maximum;
				execute(increase, (int)&value);
			} else if(mousein({px, y, px + bar_width, y + height}))
				drag_id = (int)&value;
		} else if(hot::key == MouseWheelUp || hot::key == MouseWheelDown) {
			if(mousein({x, y, x + body_width, y + height})) {
				if(hot::key == MouseWheelUp) {
					current_border = minimum;
					execute(decrease, (int)&value);
				} else {
					current_border = maximum;
					execute(increase, (int)&value);
				}
			}
		}
	}
	if(button(x + left_offset, y + 1, res, {left, left, (unsigned char)(left + pressed)}, KeyLeft, 0)) {
		current_border = minimum;
		execute(decrease, (int)&value);
	}
	if(button(x + getwidth(res, body) - getwidth(res, right) - 1, y + 1, res, {right, right, (unsigned char)(right + pressed)}, KeyRight, 0)) {
		current_border = maximum;
		execute(increase, (int)&value);
	}
}

static void createpal(unsigned char* pallette) {
	for(auto i = 0; i < 256; i++)
		pallette[i] = (unsigned char)i;
}

static void changepal(unsigned char* pallette, unsigned char start, unsigned count, unsigned char source) {
	memcpy(pallette + start, pallette + source, count);
}

void draw::initialize() {
	createpal(font_yellow);
	changepal(font_yellow, 10, 16, 108);
	createpal(route_brown);
	changepal(route_brown, 85, 22, 42);
}

void draw::updatescreen() {
	sys_input(false);
}