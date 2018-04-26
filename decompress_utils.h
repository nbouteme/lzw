#ifndef DECOMPRESS_UTILS_H
#define DECOMPRESS_UTILS_H
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <unordered_map>
#include "sym.h"
#include "lzw.h"

namespace lzw {
	struct default_symbol_writer {
		std::ostream &os;
		default_symbol_writer(std::ostream &_os);
		operator bool() const;
		void write(const sym_type &sym);
	};

	struct default_codestream_reader {
		// flemme
		std::vector<sym> syms;
		unsigned i = 0;

		operator bool() const;
		char *readstream(std::istream &is, unsigned &size);
		default_codestream_reader(std::istream &is);
		sym readcode();
	};

	struct default_code_to_sym {
		std::unordered_map<sym_type, unsigned, hash_sym> dict;
		std::vector<sym_type> rdict;

		bool exists(const sym &s);
		bool exists(const sym_type &s);
		sym_type get(const sym &s);
		void add(const sym_type &s);
	};
}

#endif /* DECOMPRESS_UTILS_H */