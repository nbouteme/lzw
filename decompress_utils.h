#ifndef DECOMPRESS_UTILS_H
#define DECOMPRESS_UTILS_H
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <unordered_map>
#include "sym.h"
#include "lzw.h"

namespace lzw {
	struct default_code_to_sym {
		std::unordered_map<sym_type, unsigned, hash_sym> dict;
		std::vector<sym_type> rdict;

		bool exists(const sym &s);
		bool exists(const sym_type &s);
		sym_type get(const sym &s);
		void add(const sym_type &s);
		void clear();
		unsigned size();
	};

	struct default_symbol_writer {
		std::ostream &os;
		default_symbol_writer(std::ostream &_os);
		operator bool() const;
		void write(const sym_type &sym);
	};

	struct default_codestream_reader {
		// flemme
		std::istream &is;
		default_code_to_sym &dcts;
		std::vector<sym> syms;


		unsigned i = 0;
		unsigned char idx = 0;
		unsigned sz = 8;
		unsigned nsyms;
		char *cptr;
		char *base;

		operator bool() const;
		default_codestream_reader(std::istream &is, default_code_to_sym&);
		sym readcode();
		~default_codestream_reader();
	};
}

#endif /* DECOMPRESS_UTILS_H */
