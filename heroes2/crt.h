#pragma once

#define SH(v) (1<<v)
#define maptbl(t,i) t[(i>(sizeof(t)/sizeof(t[0])-1)) ? sizeof(t)/sizeof(t[0])-1: i]
#define zendof(e) (e+sizeof(e)/sizeof(e[0]))

template<class T> inline T			imax(T a, T b) { return a > b ? a : b; }
template<class T> inline T			imin(T a, T b) { return a < b ? a : b; }
template<class T> inline T			iabs(T a) { return a > 0 ? a : -a; }
template<class T> inline void		iswap(T& a, T& b) { T i = a; a = b; b = i; }
//
bool								loadb(const char* url, void* p, unsigned size);
bool								saveb(const char* url, const void* p, unsigned size);
const char*							szdup(const char* text);
char*								szprint(char* result, const char* result_maximum, const char* format, ...);
char*								szprintv(char* result, const char* result_maximum, const char* format, const char* format_param);
template<unsigned N> inline char*	zprint(char(&result)[N], const char* format, ...) { return szprintv(result, result + N - 1, format, (const char*)&format + sizeof(format)); }
// Inline strings functions
template<class T> inline void		zcpy(T* p1, const T* p2) { while(*p2) *p1++ = *p2++; *p1 = 0; }
template<class T> inline void		zcpy(T* p1, const T* p2, int max_count) { while(*p2 && max_count-- > 0) *p1++ = *p2++; *p1 = 0; }
template<class T> inline T*			zend(T* p) { while(*p) p++; return p; }
template<class T> inline void		zcat(T* p1, const T e) { p1 = zend(p1); p1[0] = e; p1[1] = 0; }
template<class T> inline void		zcat(T* p1, const T* p2) { zcpy(zend(p1), p2); }
template<class T> inline int		zlen(T* p) { return zend(p) - p; }
//
extern "C" void						abort();
extern "C" int						atexit(void(*func)(void));
extern "C" void*					bsearch(const void* key, const void *base, unsigned num, unsigned size, int(*compar)(const void *, const void *));
extern "C" unsigned					clock();
extern "C" void						exit(int exit_code);
extern "C" int						memcmp(const void* p1, const void* p2, unsigned size);
extern "C" void*					memmove(void* destination, const void* source, unsigned size);
extern "C" void*					memcpy(void* destination, const void* source, unsigned size);
extern "C" void*					memset(void* destination, int value, unsigned size);
extern "C" void						qsort(void* base, unsigned num, unsigned size, int(*compar)(const void*, const void*));
extern "C" int						rand(void); // Get next random value
extern void							sleep(unsigned seconds); // Suspend execution for an interval of time
extern "C" void						srand(unsigned seed); // Set random seed
extern "C" int						strcmp(const char* s1, const char* s2); // Compare two strings
extern "C" int						system(const char* command); // Execute system command
//
inline int							xrand(int v1, int v2) { return v1 + (rand() % (v2 - v1 + 1)); }
inline int							d100() { return rand() % 100; }
template<class T> inline void		zshuffle(T* p, int count) { for(int i = 0; i < count; i++) iswap(p[i], p[rand() % count]); }

namespace std {
template<class T> class initializer_list {
public:
	typedef T				value_type;
	typedef const T&		reference;
	typedef const T&		const_reference;
	typedef unsigned		size_type;
	typedef const T*		iterator;
	typedef const T*		const_iterator;
	constexpr initializer_list() noexcept : first(0), last(0) {}
	constexpr initializer_list(const T *first_arg, const T *last_arg) noexcept : first(first_arg), last(last_arg) {}
	constexpr const T*		begin() const noexcept { return first; }
	constexpr const T*		end() const noexcept { return last; }
	constexpr unsigned		size() const noexcept { return last - first; }
private:
	const T*				first;
	const T*				last;
};
}
// Abstract flag data bazed on enumerator
template<typename T, typename DT = unsigned>
struct cflags {
	DT						data;
	constexpr cflags() : data(0) {}
	constexpr cflags(const std::initializer_list<T>& list) : data() { for(auto e : list) add(e); }
	constexpr void			add(const T id) { data |= 1 << id; }
	constexpr void			clear() { data = 0; }
	constexpr bool			is(const T id) const { return (data & (1 << id)) != 0; }
	constexpr void			remove(T id) { data &= ~(1 << id); }
};
// Abstract storage collection
template<typename T, unsigned N> struct adat {
	static constexpr unsigned maximum = N;
	unsigned				count;
	T						data[N];
	constexpr adat() : count(0), data() {}
	constexpr T& operator[](unsigned index) { return data[index]; }
	constexpr const T& operator[](unsigned index) const { return data[index]; }
	constexpr explicit operator bool() const { return count != 0; }
	T*						add() { return (count < N) ? (data + (count++)) : 0; }
	void					add(const T& e) { if(count < N) data[count++] = e; }
	constexpr T*			begin() { return data; }
	constexpr const T*		begin() const { return data; }
	constexpr bool			cangrow() const { return; }
	void					clear() { count = 0; }
	constexpr const T*		end() const { return data + count; }
	int						getcount() const { return count; }
	int						indexof(const T* t) const { if(t<data || t>data + count) return -1; return t - data; }
	int						indexof(const T t) const { for(unsigned i = 0; i < count; i++) if(data[i] == t) return i; return -1; }
	bool					is(const T t) const { return indexof(t) != -1; }
};
// Abstract reference collection
template<typename T> struct aref {
	unsigned				count;
	T*						data;
	constexpr aref() : count(0), data(0) {}
	constexpr aref(T& data) : data(&data), count(1) {}
	template<unsigned N> constexpr aref(T(&data)[N]) : data(data), count(N) {}
	constexpr T& operator[](unsigned index) { return data[index]; }
	constexpr const T& operator[](unsigned index) const { return data[index]; }
	constexpr explicit operator bool() const { return count != 0; }
	constexpr T*			begin() { return data; }
	constexpr const T*		begin() const { return data; }
	constexpr const T*		end() const { return data + count; }
	int						getcount() const { return count; }
	int						indexof(const T* t) const { if(t<data || t>data + count) return -1; return t - data; }
	int						indexof(const T t) const { for(unsigned i = 0; i < count; i++) if(data[i] == t) return i; return -1; }
	bool					is(const T t) const { return indexof(t) != -1; }
};
// Common access to data types
template<typename T> struct bsmeta {
	static constexpr const unsigned size = sizeof(T);
	static T				elements[];
	static unsigned			count;
	static const unsigned	count_maximum;
	//
	static T*				add() { return (count >= count_maximum) ? 0 : &elements[count++]; }
	static T*				begin() { return elements; }
	static T*				end() { return elements + count; }
};
template<typename T> const char* getstr(const T v) { return bsmeta<T>::elements[v].name; }
#define DECLENUM(e) template<> struct bsmeta<e##_s> : bsmeta<e##i> {}
#define DECLBASE(t, n) t bsmeta<t>::elements[n]; unsigned bsmeta<t>::count;\
const unsigned bsmeta<t>::count_maximum = sizeof(bsmeta<t>::elements) / sizeof(bsmeta<t>::elements[0])
#define assert_enum(e, last) static_assert(sizeof(bsmeta<e##i>::elements) / sizeof(bsmeta<e##i>::elements[0]) == last + 1, "Invalid count of " #e " elements");\
unsigned bsmeta<e##i>::count = sizeof(bsmeta<e##i>::elements) / sizeof(bsmeta<e##i>::elements[0]);\
const unsigned bsmeta<e##i>::count_maximum = sizeof(bsmeta<e##i>::elements) / sizeof(bsmeta<e##i>::elements[0]);