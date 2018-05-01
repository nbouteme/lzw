#include "blob.h"


blob::blob(const char *s) {
	sz = strlen(data);
	sso_enabled = sz <= 256;
	if (sso_enabled)
		memcpy(buf, s, sz);
	else
		data = strdup(s);

}

blob::blob(char s) {
	buf[0] = s;
	sz = 1;
	sso_enabled = sz <= 256;
}

blob::blob() {
	sz = 0;
	sso_enabled = sz <= 256;
}

blob::blob(const blob &rhs) {
	sso_enabled = rhs.sz <= 256;
	sz = rhs.sz;
	if (sso_enabled) {
		memcpy(buf, rhs.buf, sz);
	} else {
		data = (char*)malloc(rhs.sz);
		memcpy(data, rhs.data, rhs.sz);
	}
}

blob::blob(blob &&rhs) {
	sso_enabled = rhs.sz <= 256;
	sz = rhs.sz;
	if (sso_enabled) {
		memcpy(buf, rhs.buf, sz);
	} else {
		data = rhs.data;
		rhs.data = 0;
	}
}

blob &blob::operator=(const blob &rhs) {
	sso_enabled = rhs.sz <= 256;
	sz = rhs.sz;
	if (sso_enabled) {
		memcpy(buf, rhs.buf, sz);
	} else {
		data = (char*)malloc(rhs.sz);
		memcpy(data, rhs.data, rhs.sz);
	}
	return *this;
}

blob &blob::operator=(blob &&rhs) {
	sso_enabled = rhs.sz <= 256;
	sz = rhs.sz;
	if (sso_enabled) {
		memcpy(buf, rhs.buf, sz);
	} else {
		data = rhs.data;
		rhs.data = 0;
	}
	return *this;
}

bool blob::operator!=(const blob &rhs) const {
	return !(*this == rhs);
}

bool blob::operator==(const blob &rhs) const {
	if (rhs.sz != sz)
		return false;
	return !memcmp(c_str(), rhs.c_str(), sz);
}

char blob::operator[](unsigned idx) const {
	return sso_enabled ? buf[idx] : data[idx];
}

char &blob::operator[](unsigned idx) {
	return sso_enabled ? buf[idx] : data[idx];
}

blob blob::operator+(const blob &rhs) const {
	blob ret;
	ret.sz = sz + rhs.sz;
	ret.sso_enabled = ret.sz <= 256;
	char *dest = ret.sso_enabled ? ret.buf : (char*)malloc(ret.sz);
	memcpy(dest, c_str(), sz);
	memcpy(dest + sz, rhs.c_str(), rhs.sz);
	if (!ret.sso_enabled)
		ret.data = dest;
	return ret;
}

bool blob::operator<(const blob &rhs) const {
	int e = memcmp(c_str(), rhs.c_str(), sz < rhs.sz ? sz : rhs.sz);
	if (e)
		return e < 0;
	return sz < rhs.sz;
}

unsigned blob::size() const {
	return sz;
}

const char *blob::c_str() const {
	return sso_enabled ? buf : data;
}

blob::~blob() {
	if (!sso_enabled)
		free(data);
}

std::ostream &operator<<(std::ostream &os, const blob &b) {
	os.write(b.c_str(), b.sz);
	return os;
}
