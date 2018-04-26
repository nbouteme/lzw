#ifndef SYM_H
#define SYM_H

struct sym {
	int l;
	unsigned v;
	operator bool() const;
	void read(unsigned char &idx, char *&it, unsigned char s);
	void consume(char *&o, unsigned char &r);
};

#endif /* SYM_H */