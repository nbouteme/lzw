#ifndef COMPRESS_UTILS_H
#define COMPRESS_UTILS_H
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "sym.h"
#include "lzw.h"

namespace lzw {
	struct default_symstream_reader {
		std::istream &os;
		bool dead = false;
		char n = 0;

		default_symstream_reader(std::istream &_os);
		operator bool() const;
		sym_type readsym();
	};

	struct default_code_writer {
		// flemme
		std::vector<sym> syms;
		std::ostream &os;
		int s = 8;
		unsigned max = 255;

		default_code_writer(std::ostream &_os);
		operator bool() const;
		void write(unsigned code);
		~default_code_writer();
	};

	struct default_sym_to_code {
		std::unordered_map<sym_type, unsigned, hash_sym> dict;
		std::vector<sym_type> rdict;

		bool exists(const sym &s);
		bool exists(const sym_type &s);
		unsigned get(const sym_type &s);
		void add(const sym_type &s);
	};
}

#endif /* COMPRESS_UTILS_H */