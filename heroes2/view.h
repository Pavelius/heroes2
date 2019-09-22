#include "main.h"
#include "resources.h"

#pragma once

enum event_s {
	// input events
	ControlLocal = 0xD000,
	InputSymbol = 0xE000, InputTimer, InputEdit, InputEditPlus, InputEditMinus, InputExit, InputResize, InputIdle,
	InputChoose, InputDropDown, InputMenu, InputSet,
	ScrollV, ScrollH, SplitterV, SplitterH,
	TreeType, TreeFlags,
	HtmlLink, HtmlControl, TabsControl, TabsCloseControl,
	Executed,
	// control keys
	MouseLeft = 0xE800, MouseLeftDBL, MouseRight,
	MouseMove, MouseWheelUp, MouseWheelDown,
	KeyLeft, KeyRight, KeyUp, KeyDown, KeyPageUp, KeyPageDown, KeyHome, KeyEnd,
	KeyBackspace, KeyEnter, KeyDelete, KeyEscape, KeySpace, KeyTab,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	// named keys range
	Alpha,
	FirstKey = MouseLeft,
	FirstMouse = MouseLeft, LastMouse = MouseWheelDown,
	// support
	CommandMask = 0x0000FFFF,
	// misc events can be combination with previous
	Ctrl = 0x00010000,
	Alt = 0x00020000,
	Shift = 0x00040000,
	// state flags
	Focused = 0x10000000, // Control has keyboard input and can change visual form.
	Checked = 0x20000000, // Use in background virtual method.
	Disabled = 0x40000000, // Control not grant any input.
	FirstInput = InputSymbol,
};
enum image_flags {
	AFMirror = 1, AFNoOffset = 2, AFCenter = 4, AFCenterV = 8,
	AFMoving = 16,
};
enum align_s : unsigned char {
	AlignLeft, AlignCenter, AlignRight,
};
enum button_s : unsigned char {
	NoButtons, ButtonOK, ButtonYesNo, ButtonLearn,
};
namespace hot {
void						clear();
extern int					key; // [in] if pressed key or mouse this field has key
extern point				mouse; // current mouse coordinates
extern bool					pressed; // flag if any of mouse keys is pressed
extern int					param; // command or input event parameter
extern int					param2;
}
namespace draw {
struct buttoni {
	unsigned char			normal;
	unsigned char			hilite;
	unsigned char			pressed;
};
struct picture {
	res_s					res;
	unsigned char			frame;
	point					size;
	const char*				format;
	//
	void					clear();
	int						gettotalheight(unsigned count, int width_per_line) const;
	unsigned				getsize(unsigned count, int& width, int& height, int width_per_line) const;
	void					paint(int x, int y, int h1, variant element, int count) const;
	void					render(int x, int y, res_s res, int frame) const;
	void					set(const variant id, int value);
	void					setsize(res_s res, unsigned char frame);
	void					setsize() { setsize(res, frame); }
};
struct animation {
	point					pos;
	res_s					res;
	unsigned				flags;
	unsigned char			frame, start, count, wait;
	constexpr animation() : pos{0, 0}, res(NoRes), wait(0), frame(0), start(0), count(0), flags(0) {}
	bool					increment();
	static int				getanimate(res_s id, int start, int ticket, bool quantity);
	point					getbreast() const;
	point					getcenter() const;
	point					getlaunch(monster_s id, direction_s d) const;
	static res_s			getmissile(monster_s id);
	static int				getmissiledy(monster_s id, int param);
	static int				getparam(direction_s d);
	int						getwidth() const;
	void					paint(int x, int y, unsigned flags = 0) const;
	void					set(res_s id, unsigned char frame, point pos = point());
	void					update();
};
struct battleimage : animation, variant, uniti {
	constexpr explicit operator bool() const { return uniti::operator bool(); }
	void					animate(point pos, int velocity);
	void					animate(int frames = -1, int speed = -1);
	void					clear();
	int						getz() const;
	void					paint() const;
	bool					iskilled() const;
	bool					ismoved() const;
	bool					iswait() const;
	void					set(action_s action, int param = 0);
	void					set(direction_s dir);
	void					setdefault();
	void					setpos(short unsigned v);
	void					stroke() const;
	void					update();
};
struct screenshoot {
	screenshoot();
	~screenshoot();
	void					blend(screenshoot& source);
	void					restore();
private:
	unsigned char*			bits;
};
#pragma pack(push)
#pragma pack(1)
struct icn {
	struct record {
		short int			x;
		short int			y;
		short int			width;
		short int			height;
		unsigned char		type;
		unsigned			offset;
	};
	unsigned short			count;
	unsigned				size;
	record					records[1];
};
struct til {
	short unsigned			count;
	short unsigned			width;
	short unsigned			height;
};
#pragma pack(pop)
typedef void(*eventproc)();
//
const int					cell_wd = 45;
const int					cell_hd = 52;
const int					cell_wr = 22;
const int					cell_hr = 13;
extern rect					clipping;
extern unsigned				counter;
extern eventproc			domodal;
const int					width = 640;
const int					height = 480;
extern res_s				font;
extern unsigned char*		font_color;
extern unsigned char		font_yellow[256];
extern unsigned char		route_brown[256];
//
struct state {
	constexpr state() : font(draw::font), font_color(draw::font_color), clipping(draw::clipping) {}
	~state() { draw::font = font; draw::font_color = font_color; draw::clipping = clipping; }
private:
	res_s					font;
	unsigned char*			font_color;
	rect					clipping;
};
struct controli {
	int						x, y;
	res_s					res;
	unsigned char			normal, hilited, pressed;
	explicit constexpr operator bool() const { return res != 0; }
};
struct list {
	typedef void (list::*proc)(int param);
	int						origin;
	int						current;
	int						maximum;
	int						row_per_screen;
	list();
	void					correct();
	void					box(int x, int y, int count);
	void					boxs(int x, int y, int count);
	void					draw(int x, int y, int sy, int sx);
	void					ensurevisible();
	void					execute(list::proc proc, int param = 0) const;
	virtual void			row(int x, int y, int index) const = 0;
	void					select(int value);
	void					scroll(int value);
	void					view(int x, int y) const;
private:
	void					box(int x, int y, res_s icn, int dy, int dx, int sbu, int sbd, int sbs, int sdbd, int sb, int bf, res_s iss) const;
	void					flatbutton(int x, int y, res_s icn, int index, int command) const;
};
struct drawobji			{
	res_s					res;
	object_s				object;
	shapei&					shape;
	unsigned char			first;
	unsigned char			last;
	static const drawobji*	find(res_s res, unsigned char frame);
	static const drawobji*	find(object_s v);
	short unsigned			getid() const;
	static void				initialize();
};
void						breakmodal(int result);
void						button(int x, int y, res_s res, eventproc proc, const buttoni& decor, int key = 0, const char* tips = 0);
bool						button(int x, int y, res_s res, const buttoni& decor, int key = 0, const char* tips = 0);
void						buttoncancel();
void						buttonok();
bool						create(const char* title, unsigned milliseconds, bool fullscreen);
int							dialog(int height, int* xpos = 0);
void						extract(const char* fname, res_s id);
void						edit(int x, int y, int width, int& value, int maximum = 100, int minimum = 0, const controli& up = {}, const controli& down = {}, const controli& max = {});
void						execute(eventproc proc, int param = 0);
bool						ismodal();
const void*					get(res_s id);
const icn::record*			get(res_s id, int n);
rect						get(res_s id, int n, int x, int y, unsigned flags);
res_s						getbuildings(kind_s v);
int							getframecount(res_s id);
int							getheight(res_s id, int n);
const char*					getname(res_s id);
res_s						getres(unsigned char object);
int							getresult();
int							getwidth(res_s id, int n);
void						hexagon(int x, int y, unsigned char m);
void						hexagonf(int x, int y, unsigned char intense);
void						imagb(int x, int y, unsigned short value);
void						image(int x, int y, res_s res, int n, unsigned flags = 0, unsigned char* change = 0);
void						image(int x, int y, monster_s res, int n, unsigned flags = 0, int mode= 1);
void						image(int x, int y, hero_s res, bool large = true);
void						image(int x, int y, skill_s res, int level);
int							image(int x, int y, int width, const costi& e);
void						imags(int x, int y, unsigned short value, unsigned short index);
void						imagt(int x, int y, res_s res, int n, int mode);
void						image(const rect& rc, res_s id, int frame, const char* title, const char* tips);
void						image(const rect& rc, resource_s id, int count, const char* tips);
int							imagex(int x, int y, int width, const variantcol* source, unsigned count);
void						initialize();
res_s						isevil(res_s evil, res_s good);
void						line(int x1, int y1, int x2, int y2, unsigned char m);
int							message(const char* format, const variantcol* source, unsigned count, button_s mode);
bool						mousein(const rect& rc);
void						pixel(int x, int y, unsigned char a);
unsigned char*				ptr(int x, int y);
void						quicktips(int x, int y, const char* format);
void						rectb(const rect& rc, unsigned char m);
void						rectf(const rect& rc, unsigned char m);
void						setcursor(res_s res, unsigned char frame, point pos = point());
void						setevil(bool value);
void						shadow(int x1, int y1, int x2, int y2, int intense);
void						splitter(int x, int y, res_s res, int& value, int from, int to);
void						status(const rect& rc);
void						status(const char* format, ...);
void						stroke(int x, int y, res_s res, int frame, int flags, unsigned char thin, unsigned char color);
void						tooltips(const char* header, const char* format, ...);
void						text(int x, int y, const char* string, int count = -1);
void						text(int x, int y, int width, align_s align, const char* string, int count = -1);
int							textbc(const char* string, int width);
int							textm(int x, int y, int width, align_s align, const char* string);
int							textf(int x, int y, int width, const char* p);
int							textf(int width, const char* string);
int							texth();
int							texth(const char* string, int width);
int							textw(const char* string, int count = -1);
void						updatescreen();
}