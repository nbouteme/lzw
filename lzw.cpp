#include "lzw.h"
#include "compress_utils.h"
#include "decompress_utils.h"

int main(int ac, char **av)
{
	lzw::lzwcodec<> lzwcd;
	if (std::string(av[0]).find("lzw") != ~0u) {
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
