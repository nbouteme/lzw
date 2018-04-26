CC = c++
CXXFLAGS = -Ofast -march=native -mtune=native
all: lzwu
lzw: lzw.o sym.o
lzwu: lzw
	cp lzw lzwu
