#pragma once

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
template<typename T, unsigned N> struct adat {
	static constexpr unsigned maximum = N;
	unsigned				count;
	T						data[N];
	constexpr adat() :count(0), data() {}
	constexpr T& operator[](unsigned index) { return data[index]; }
	constexpr const T& operator[](unsigned index) const { return data[index]; }
	explicit operator bool() const { return count != 0; }
	T*						add() { return (count < N) ? (data + (count++)) : 0; }
	void					add(const T& e) { if(count<N) data[count++] = e; }
	constexpr T*			begin() { return data; }
	constexpr const T*		begin() const { return data; }
	constexpr bool			cangrow() const { return ; }
	void					clear() { count = 0; }
	constexpr const T*		end() const { return data + count; }
	int						getcount() const { return count; }
	int						indexof(const T* t) const { if(t<data || t>data + count) return -1; return t - data; }
	int						indexof(const T t) const { for(unsigned i = 0; i < count; i++) if(data[i] == t) return i; return -1; }
	bool					is(const T t) const { return indexof(t) != -1; }
};
template<typename T> struct aref {
	unsigned				count;
	T*						data;
	constexpr aref() :count(0), data(0) {}
	constexpr aref(T& data) : data(&data), count(1) {}
	template<unsigned N> constexpr aref(T(&data)[N]) : data(data), count(N) {}
	constexpr T& operator[](unsigned index) { return data[index]; }
	constexpr const T& operator[](unsigned index) const { return data[index]; }
	explicit operator bool() const { return count != 0; }
	constexpr T*			begin() { return data; }
	constexpr const T*		begin() const { return data; }
	constexpr const T*		end() const { return data + count; }
	int						getcount() const { return count; }
	int						indexof(const T* t) const { if(t<data || t>data + count) return -1; return t - data; }
	int						indexof(const T t) const { for(unsigned i = 0; i < count; i++) if(data[i] == t) return i; return -1; }
	bool					is(const T t) const { return indexof(t) != -1; }
};
// Common access data types
template<typename T> struct bsmeta {
	typedef T				data_type;
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
#define SH(v) (1<<v)
#define assert_enum(e, last) static_assert(sizeof(bsmeta<e##i>::elements) / sizeof(bsmeta<e##i>::elements[0]) == last + 1, "Invalid count of " #e " elements");
#define maptbl(t,i) t[(i>(sizeof(t)/sizeof(t[0])-1)) ? sizeof(t)/sizeof(t[0])-1: i]
#define zendof(e) (e+sizeof(e)/sizeof(e[0]))