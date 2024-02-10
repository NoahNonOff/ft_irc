#pragma once

#include <bitset>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip> /* setfill() && setw() */
#include <stdint.h> /* uint32_t */

/* ["https://www.rfc-editor.org/rfc/rfc1321"] */
/* ["https://en.wikipedia.org/wiki/MD5#Algorithm"] */
/* ["https://learn.microsoft.com/en-us/cpp/cpp/data-type-ranges?view=msvc-170"] */
/* ["https://en.wikipedia.org/wiki/Endianness"] */

/* ["http://www.zedwood.com/article/cpp-md5-function"] */

#ifndef MD5_F
#define MD5_F

	/* '^' = XOR ; '&' = AND ; '|' = OR ; '~' = NOT */

	#define TO32BIT(x) (x & 0xFFFFFFFF)

	// #define F(B, C, D) (((B) & (C)) | ((~B) & (D)))
	#define F(B, C, D) ((D) ^ ((B) & ((C) ^ (D)))) // For improved efficiency

	// #define G(B, C, D) (((B) & (D)) | ((C) & (~D)))
	#define G(B, C, D) ((C) ^ ((D) & ((B) ^ (C)))) // For improved efficiency

	#define H(B, C, D) ((B) ^ (C) ^ (D))
	#define I(B, C, D) ((C) ^ ((B) | ~(D)))

	#define ROTLEFT(word, bits) (((word) << (bits)) | ((word) >> (32 - (bits))))

	// to remove
	// Rotation is separate from addition to prevent recomputation

	// fisrt round
	#define FF(A, B, C, D, x, s, ac) {							\
		(A) += F((B), (C), (D)) + (x) + (ac);					\
		(A) = ROTLEFT((A), (s));								\
		(A) += (B);												\
	}

	// second round
	#define GG(A, B, C, D, x, s, ac) {							\
		(A) += G((B), (C), (D)) + (x) + (ac);					\
		(A) = ROTLEFT((A), (s));								\
		(A) += (B);												\
	}

	// third round
	#define HH(A, B, C, D, x, s, ac) {							\
		(A) += H((B), (C), (D)) + (x) + (ac);					\
		(A) = ROTLEFT((A), (s));								\
		(A) += (B);												\
	}

	// last round
	#define II(A, B, C, D, x, s, ac) {							\
		(A) += I((B), (C), (D)) + (x) + (ac);					\
		(A) = ROTLEFT((A), (s));								\
		(A) += (B);												\
	}

#endif

namespace hash {

	class MD5
	{
		public:
			typedef std::vector<uint32_t>	vector32;

		private:
			const std::string	_raw;
			std::string			_hash;

			vector32	_blocks;
			uint32_t	_blocklen;

			uint32_t	_M[16];
			uint32_t	_state[4]; // A, B, C, D

		public:
			MD5( void );
			MD5( const std::string & );
			~MD5( void );

			const std::string	getRaw( void ) const;
			const std::string	getHash( void ) const;

			void				setHash( void );

			void				hash( void );
			void				padding( void );
			void				resizeBlock( void );
			void				constructBlock( const int & );
			void				compute( void );
	};

	const std::string	md5( const std::string & );
}