#ifndef DECOMPRESS_UTILS_H
#define DECOMPRESS_UTILS_H
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "sym.h"

namespace lzw {
	struct default_symbol_writer {
		std::ostream &os;
		default_symbol_writer(std::ostream &_os) : os(_os) {
		}

		operator bool() const {
			return os.good();
		}

		void write(const std::string &sym) {
			os.write(sym.c_str(), sym.size());
		}
	};

	struct default_codestream_reader {
		unsigned i = 0;
		// flemme
		std::vector<sym> syms;

		operator bool() const {
			return i < syms.size();
		}

		char *readstream(std::istream &is, unsigned &size) {
			unsigned allocated = 4 * 1024 * 1024;
			char *buf = (char*)malloc(allocated);
			size = 0;
			while (is) {
				size += is.read(buf + size, allocated - size).gcount();
				if (is && size == allocated) {
					allocated *= 2;
					if(!realloc(buf, allocated))
						abort();
				}
			}
			if (size && !realloc(buf, size))
				abort();
			return buf;
		}

		default_codestream_reader(std::istream &is) {
			unsigned size = 0;
			char *in = readstream(is, size);
			unsigned nsyms = *(unsigned *)in;
			char *it = in + 4;
			unsigned char idx = 0;
			unsigned sz = 8;
			while (syms.size() != nsyms) {
				unsigned max = (1 << sz) - 1;
				sym s;
				s.read(idx, it, sz);
				while (s.v == max) {
					++sz;
					nsyms--;
					s.read(idx, it, sz);
					max = (1 << sz) - 1;
				}
				syms.push_back(s);
			}
		}

		sym readcode() {
			std::cerr << "Read symbol " << syms[i].v << std::endl;
			return syms[i++];
		}
	};


	struct default_code_to_sym {
		std::map<std::string, unsigned> dict;
		std::vector<std::string> rdict;

		bool exists(const sym &s) {
			if (s.v <= 255)
				return true;
			return rdict.size() > (s.v - 255) &&
				rdict[(s.v - 255)] != "";
		}

		bool exists(const std::string &s) {
			if (s.size() <= 1)
				return true;
			return !!dict.count(s);
		}

		std::string get(const sym &s) {
			if (s.v <= 255)
				return std::string("") + (char)s.v;
			return rdict[s.v - 255];
		}

		void add(const std::string &s) {
			auto &ent = dict[s];
			ent = dict.size() + 255;
			rdict.resize(dict.size() + 1);
			rdict[dict.size()] = s;
		}
	};
}

#endif /* DECOMPRESS_UTILS_H */