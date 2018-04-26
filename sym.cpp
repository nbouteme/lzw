#include "sym.h"

sym::operator bool() const {
	return l > 0;
}

void sym::read(unsigned char &idx, char *&it, unsigned char s) {
		l = 0;
		v = 0;
		unsigned left = s;
		while (l != s) {
			unsigned cons = left > 8 ? 8 : left;
			if (cons + idx > 8)
				cons = 8 - idx;
			unsigned char m = ((1 << cons) - 1) << idx;
			v = ((*it & m) >> idx) << l | v;
			idx += cons;
			it += idx >> 3;
			idx %= 8;
			left -= cons;
			l += cons;
		}
	}

void sym::consume(char *&o, unsigned char &r) {
	while (l) {
		int left = l >= r ? r : l;
		unsigned m = (1l << left) - 1;
		*o |= (v & m) << (8 - r);
		r -= left;
		l -= left;
		v >>= left;
		if (r)
			continue;
		++o;
		r = 8;
	}
}
