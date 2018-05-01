#ifndef BLOB_H
#define BLOB_H

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

struct blob {
	union {
		char *data;
		char buf[256];
	};
	unsigned sz;
	bool sso_enabled;

	blob(const char *s);
	blob(char s);
	blob();
	blob(const blob &rhs);
	blob(blob &&rhs);
	blob &operator=(const blob &rhs);
	blob &operator=(blob &&rhs);
	bool operator!=(const blob &rhs) const;
	bool operator==(const blob &rhs) const;
	char operator[](unsigned idx) const;
	char &operator[](unsigned idx);
	blob operator+(const blob &rhs) const;
	bool operator<(const blob &rhs) const;
	unsigned size() const;
	const char *c_str() const;
	~blob();
};

std::ostream &operator<<(std::ostream &os, const blob &b);

#endif /* BLOB_H */