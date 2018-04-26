#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "sym.h"
#include "compress_utils.h"
#include "decompress_utils.h"

namespace lzw {
	struct adder {
		template <typename T>
		T operator()(const T &a, const T &b) {
			return a + b;
		}
	};

	template <typename concatf = adder,
			  typename symbol = std::string,
			  typename code = sym>
	struct lzwcodec {
		concatf concat;
		/*
		  T : InputReader:
		  - bool() -> bool | Is there something left to read?
		  - readsym -> symbol | next symbol
		  U : OutputWriter:
		  - write -> code -> void | write a code
		  D : DictionnaryType<Symbol -> Code>:
		  - exists -> Symbol -> bool | Does the symbol exists?
		  - add -> Symbol -> void | Add the symbol to the dictionnary
		  - get -> Symbol -> Code | Returns a code for a symbol
		*/
		template <typename T,
				  typename U,
				  typename D>
		void compress(T &input, U &output, D &dict) {
			symbol w = "";
			while (input) {
				symbol c = input.readsym();
				symbol p = concat(w, c);
				if (dict.exists(p))
					w = p;
				else {
					dict.add(p);
					output.write(dict.get(w));
					w = c;
				}
			}
			if (w.size() > 1)
				output.write(dict.get(w));
		}

		/*
		  T : CodeReader:
		  - bool() -> bool | Is there something left to read?
		  - readcode -> code | next code
		  U : OutputWriter:
		  - write -> Symbol -> void | write a symbol
		  D : DictionnaryType<Code -> Symbole>:
		  - exists -> Code -> bool | Does the symbol exists?
		  - add -> Symbol -> void | Add the symbol to the dictionnary,
		  | allocating a new code
		  - get -> Code -> Symbol | Returns a symbol from a code
		*/
		template <typename T, typename U, typename D>
		void decompress(T &codestream, U &outputstream, D &rdict) {
			if (!codestream)
				return;
			code v = codestream.readcode();
			symbol w = rdict.get(v);
			outputstream.write(w);

			while (codestream) {
				v = codestream.readcode();
				symbol e;
				if (rdict.exists(v)) {
					e = rdict.get(v);
				} else {
					e = w + w[0];
				}
				outputstream.write(e);
				rdict.add(w + e[0]);
				w = e;
			}
		}
	};
}

int main(int ac, char **av)
{
	lzw::lzwcodec<> lzwcd;
	if (ac == 1) {
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
