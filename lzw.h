#ifndef LZW_H
#define LZW_H

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "sym.h"
#include "blob.h"

namespace lzw {
	using sym_type = blob;
	using code_type = sym;

	struct adder {
		template <typename T>
		T operator()(const T &a, const T &b) {
			return a + b;
		}
	};

	struct hash_sym {
		int32_t operator()(const sym_type &p) const {
			// Basé sur FNV sauf que les xors se font sur plusieurs
			// octets à la fois
			unsigned rem = p.sz & 0x7;
			size_t hash = 0xCBF29CE484222325;
			long *ptr = (long*)p.data;
			unsigned words = p.sz >> 3;
			for (unsigned i = 0; i < words; ++i) {
				hash *= 0x10000001B3;
				hash ^= ptr[i];
			}
			ptr += words;
			char *it = (char*)ptr;
			for(unsigned i = 0; i < rem; ++i) {
				hash *= 0x1000193;
				hash ^= it[i];
			}
			return hash;
		}
	};

	template <typename concatf = adder,
			  typename symbol = sym_type,
			  typename code = code_type>
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

#endif /* LZW_H */