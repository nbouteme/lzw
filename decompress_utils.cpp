#include "decompress_utils.h"

namespace lzw {
	default_symbol_writer::default_symbol_writer(std::ostream &_os) : os(_os) {
	}

	default_symbol_writer::operator bool() const {
		return os.good();
	}

	void default_symbol_writer::write(const sym_type &sym) {
		os.write(sym.c_str(), sym.size());
	}

	default_codestream_reader::operator bool() const {
		return i < syms.size();
	}

	char *default_codestream_reader::readstream(std::istream &is, unsigned &size) {
		unsigned allocated = 4 * 1024 * 1024;
		char *buf = (char*)malloc(allocated);
		size = 0;
		while (is) {
			size += is.read(buf + size, allocated - size).gcount();
			if (is && size == allocated) {
				allocated *= 2;
				if(!(buf = (char*)realloc(buf, allocated)))
					abort();
			}
		}
		if (size && !(buf = (char*)realloc(buf, size)))
			abort();
		return buf;
	}

	default_codestream_reader::default_codestream_reader(std::istream &is) {
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
		free(in);
	}

	sym default_codestream_reader::readcode() {
		return syms[i++];
	}

	bool default_code_to_sym::exists(const sym &s) {
		if (s.v <= 255)
			return true;
		return rdict.size() > (s.v - 255) &&
			rdict[(s.v - 255)] != "";
	}

	bool default_code_to_sym::exists(const sym_type &s) {
		if (s.size() <= 1)
			return true;
		return !!dict.count(s);
	}

	sym_type default_code_to_sym::get(const sym &s) {
		if (s.v <= 255)
			return sym_type() + (char)s.v;
		return rdict[s.v - 255];
	}

	void default_code_to_sym::add(const sym_type &s) {
		auto &ent = dict[s];
		ent = dict.size() + 255;
		rdict.resize(dict.size() + 1);
		rdict[dict.size()] = s;
	}
}
