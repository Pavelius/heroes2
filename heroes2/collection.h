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
// Common access data types
template<typename T> struct bsmeta {
	typedef T				data_type;
	static T				elements[];
	static unsigned			count;
};
template<typename T> const char* getstr(const T v) { return bsmeta<T>::elements[v].name; }
#define DECLENUM(e) template<> struct bsmeta<e##_s> : bsmeta<e##i> {}
#define assert_enum(e, last) static_assert(sizeof(bsmeta<e##i>::elements) / sizeof(bsmeta<e##i>::elements[0]) == last + 1, "Invalid count of " #e " elements");
#define maptbl(t,i) t[(i>(sizeof(t)/sizeof(t[0])-1)) ? sizeof(t)/sizeof(t[0])-1: i]
#define zendof(e) (e+sizeof(e)/sizeof(e[0]))