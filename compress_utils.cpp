#include "compress_utils.h"
#include <fstream>

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
			return sym_type();
		char d = n;
		os.read(&n, 1);
		dead = !os;
		return sym_type() + d;
	}

	default_code_writer::default_code_writer(std::ostream &_os,
		default_sym_to_code &_dstc) :
		os(_os),
		dstc(_dstc),
		ofs("compresslog") {
		dstc.dcw = this;
	}

	default_code_writer::operator bool() const {
		return os.good();
	}

	void default_code_writer::write(unsigned code) {
		while (dstc.size() >= max) {
			//syms.push_back({s, max});
			++s;
			max = (1 << s) - 1;
		}
		//ofs << code << " " << s << std::endl;
		//ofs << code << " " << s <<  " " << dstc.size() << std::endl;
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
			rdict[(s.v - 255)] != sym_type();
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
		if (size() >= (1 << 16) - 2) {
			clear();
			if (dcw) {
				dcw->s = 9;
				dcw->max = (1 << dcw->s) - 1;
			}
		}
		auto &ent = dict[s];
		ent = dict.size() + 255;
		rdict.resize(dict.size() + 1);
		rdict[dict.size()] = s;
	}

	unsigned default_sym_to_code::size() {
		return dict.size() + 255;
	}

	void default_sym_to_code::clear() {
		dict.clear();
		rdict.clear();
	}
}
