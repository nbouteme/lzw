#ifndef COMPRESS_UTILS_H
#define COMPRESS_UTILS_H
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "sym.h"

namespace lzw {
	struct default_symstream_reader {
		std::istream &os;
		default_symstream_reader(std::istream &_os) :
			os(_os) {
		}

		operator bool() const {
			return os.good();
		}

		std::string readsym() {
			if (!*this)
				return "";
			char d[1];
			os.read(d, 1);
			return d;
		}
	};

	struct default_code_writer {
		// flemme
		std::vector<sym> syms;
		std::ostream &os;
		int s = 8;
		unsigned max = 255;
		default_code_writer(std::ostream &_os) :
			os(_os) {
		}

		operator bool() const {
			return os.good();
		}

		void write(unsigned &&code) {
			std::cerr << "Wrote symbol " << code << std::endl;
			while (code >= max) {
				syms.push_back({s, max});
				++s;
				max = (1 << s) - 1;
			}
			syms.push_back({s, code});
		}

		~default_code_writer() {
			unsigned size = 0;
			for (auto s : syms) {
				size += s.l;
			}
			size = (size >> 3) + !!(size & 3);
			char *ret = (char*)malloc(size);
			memset(ret, 0, size);
			char *it = ret;
			unsigned char left = 8;
			for (auto s : syms) {
				s.consume(it, left);
			}
			unsigned nsyms = syms.size();
			os.write((char*)&nsyms, sizeof(nsyms));
			os.write(ret, size);
			free(ret);
		}
	};

	struct default_sym_to_code {
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

		unsigned get(const std::string &s) {
			if (s.size() == 1)
				return (unsigned)s[0];
			return dict[s];
		}

		void add(const std::string &s) {
			auto &ent = dict[s];
			ent = dict.size() + 255;
			rdict.resize(dict.size() + 1);
			rdict[dict.size()] = s;
		}
	};
}

#endif /* COMPRESS_UTILS_H */