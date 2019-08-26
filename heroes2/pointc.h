#pragma once

struct pointc
{
	char				x, y;
	inline explicit		operator bool() const { return x || y; }
	void				clear() { x = y = 0; }
};