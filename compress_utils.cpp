#include "compress_utils.h"

namespace lzw {
	default_symstream_reader::default_symstream_reader(std::istream &_os) :
		os(_os) {
		os.read(&n, 1);
		dead = !os;
	}

	default_symstream_reader::operator bool() const {
		return !dead;
	}

	sym_type default_symstream_reader::readsym() {
		if (!*this)
			return "";
		char d = n;
		os.read(&n, 1);
		dead = !os;
		return sym_type() + d;
	}

	default_code_writer::default_code_writer(std::ostream &_os) :
		os(_os) {
	}

	default_code_writer::operator bool() const {
		return os.good();
	}

	void default_code_writer::write(unsigned code) {
		while (code >= max) {
			syms.push_back({s, max});
			++s;
			max = (1 << s) - 1;
		}
		syms.push_back({s, code});
	}

	default_code_writer::~default_code_writer() {
		unsigned size = 0;
		for (auto s : syms) {
			size += s.l;
		}
		size = (size >> 3) + !!(size & 7);
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

	bool default_sym_to_code::exists(const sym &s) {
		if (s.v <= 255)
			return true;
		return rdict.size() > (s.v - 255) &&
			rdict[(s.v - 255)] != "";
	}

	bool default_sym_to_code::exists(const sym_type &s) {
		if (s.size() <= 1)
			return true;
		return !!dict.count(s);
	}

	unsigned default_sym_to_code::get(const sym_type &s) {
		if (s.size() == 1)
			return (unsigned char)s.c_str()[0];
		return dict[s];
	}

	void default_sym_to_code::add(const sym_type &s) {
		auto &ent = dict[s];
		ent = dict.size() + 255;
		rdict.resize(dict.size() + 1);
		rdict[dict.size()] = s;
	}
}
