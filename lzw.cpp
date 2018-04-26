#include "lzw.h"
#include "compress_utils.h"
#include "decompress_utils.h"

namespace lzw {
	size_t hash_sym::operator()(const sym_type &p) const {
		// Basé sur FNV sauf que les xors se font sur plusieurs
		// octets à la fois
		unsigned rem = p.sz & 0x7;
		size_t hash = 0xCBF29CE484222325;
		long *ptr = (long*)p.data;
		unsigned words = p.sz >> 3;
		for (unsigned i = 0; i < words; ++i) {
			hash *= 0x10000001B3;
			hash ^= ptr[i];
		}
		ptr += words;
		char *it = (char*)ptr;
		for(unsigned i = 0; i < rem; ++i) {
			hash *= 0x1000193;
			hash ^= it[i];
		}
		return hash;
	}

}

int main(int ac, char **av)
{
	lzw::lzwcodec<> lzwcd;
	if (std::string(av[0]).find("lzwu") == std::string::npos) {
		lzw::default_symstream_reader dsr(std::cin);
		lzw::default_code_writer dcw(std::cout);
		lzw::default_sym_to_code dstc;
		lzwcd.compress(dsr, dcw, dstc);
	} else {
		lzw::default_codestream_reader dsr(std::cin);
		lzw::default_symbol_writer dcw(std::cout);
		lzw::default_code_to_sym dstc;
		lzwcd.decompress(dsr, dcw, dstc);
	}
	return 0;
}
