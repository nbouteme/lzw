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
		return i < nsyms;
	}

	char *readstream(std::istream &is, unsigned &size) {
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

	default_codestream_reader::default_codestream_reader(std::istream &_is,
														 default_code_to_sym &_dcts) :
		is(_is),
		dcts(_dcts),
		ofs("decomplog") {
		unsigned size = 0;
		base = readstream(is, size);
		nsyms = *(unsigned *)base;
		cptr = base + 4;
	}

	sym default_codestream_reader::readcode() {
		sym s;
		unsigned max = (1 << sz) - 1;
		if (dcts.size() + 1 >= max) {
			++sz;
		}
		if (sz == 17) {
			sz = 9;
			dcts.clear();
		}
		s.read(idx, cptr, sz);
		//ofs << s.v << " " << s.l << std::endl;
		ofs << s.v << " " << s.l << " " << dcts.size() << std::endl;
		++i;
		return s;
	}

	default_codestream_reader::~default_codestream_reader() {
		free(base);
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

	unsigned default_code_to_sym::size() {
		return dict.size() + 255;
	}

	void default_code_to_sym::clear() {
		dict.clear();
		rdict.clear();
	}

	void default_code_to_sym::add(const sym_type &s) {
		auto &ent = dict[s];
		ent = dict.size() + 255;
		rdict.resize(dict.size() + 1);
		rdict[dict.size()] = s;
	}
}
