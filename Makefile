CC = c++
CXXFLAGS = -Ofast -march=native -mtune=native # -flto
OBJS = lzw.o sym.o decompress_utils.o compress_utils.o blob.o
all: lzwu
lzw: $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $@
lzwu: lzw
	cp lzw lzwu
