#pragma once

#include <cstring>
#include <iostream>
#include <stdint.h> /* uint32_t */

/* ["https://www.rfc-editor.org/rfc/rfc1321"] */
/* ["https://en.wikipedia.org/wiki/MD5#Algorithm"] */
/* ["https://learn.microsoft.com/en-us/cpp/cpp/data-type-ranges?view=msvc-170"] */
/* ["https://en.wikipedia.org/wiki/Endianness"] */

/* ["http://www.zedwood.com/article/cpp-md5-function"] */

#ifndef MD5_F
#define MD5_F

	/* '^' = XOR ; '&' = AND ; '|' = OR ; '~' = NOT */

	#define BLOCKSIZE 64

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

// typedef unsigned int size_type; // must be 32bit
namespace hash {

	class MD5
	{
		public:
			MD5( void );
			MD5( const std::string& );

		private:
			bool		_finalized;

			uint8_t		_buffer[BLOCKSIZE];	// bytes that didn't fit in last 64 byte chunk
			uint32_t	_count[2];			// 64bit counter for number of bits (lo, hi)
			uint32_t	_state[4];			// digest so far
			uint8_t		_digest[16];		// the result
			uint32_t	_M[16];

			void				init( void );
			void				update( const char *, uint32_t );
			void				update( const unsigned char *, uint32_t );
			void				transform( const uint8_t block[BLOCKSIZE] );

			void		decode( uint32_t output[], const uint8_t input[], uint32_t );
			void		encode( uint8_t output[], const uint32_t input[], uint32_t );
	};

	const std::string	md5( const std::string & );
}