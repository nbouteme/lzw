#ifndef BLOB_H
#define BLOB_H

#include <string>
#include <cstring>
#include <cstdlib>

struct blob {
	char *data;
	unsigned sz;

	blob(const char *s) {
		data = strdup(s);
		sz = strlen(data);
	}

	blob(char s) {
		data = strdup("");
		data[0] = s;
		sz = 1;
	}

	blob() {
		data = 0;
		sz = 0;
	}

	blob(const blob &rhs) {
		data = (char*)malloc(rhs.sz);
		memcpy(data, rhs.data, rhs.sz);
		sz = rhs.sz;
	}

	blob(blob &&rhs) {
		data = rhs.data;
		sz = rhs.sz;
		rhs.data = 0;
	}

	blob &operator=(const blob &rhs) {
		free(data);
		data = (char*)malloc(rhs.sz);
		memcpy(data, rhs.data, rhs.sz);
		sz = rhs.sz;
		return *this;
	}

	blob &operator=(blob &&rhs) {
		data = rhs.data;
		sz = rhs.sz;
		rhs.data = 0;
		return *this;
	}

	bool operator!=(const blob &rhs) const {
		return !(*this == rhs);
	}

	bool operator==(const blob &rhs) const {
		if (rhs.sz != sz)
			return false;
		return !memcmp(data, rhs.data, sz);
	}

	char operator[](unsigned idx) const {
		return data[idx];
	}

	char &operator[](unsigned idx) {
		return data[idx];
	}

	blob operator+(const blob &rhs) const {
		blob ret;
		ret.data = (char*)malloc(sz + rhs.sz);
		memcpy(ret.data, data, sz);
		memcpy(ret.data + sz, rhs.data, rhs.sz);
		ret.sz = sz + rhs.sz;
		return ret;
	}

	bool operator<(const blob &rhs) const {
		int e = memcmp(data, rhs.data, sz < rhs.sz ? sz : rhs.sz);
		if (e)
			return e < 0;
		return sz < rhs.sz;
	}

	unsigned size() const {
		return sz;
	}

	const char *c_str() const {
		return data;
	}

	~blob() {
		free(data);
	}
};

std::ostream &operator<<(std::ostream &os, const blob &b) {
	os.write(b.c_str(), b.sz);
	return os;
}

#endif /* BLOB_H */