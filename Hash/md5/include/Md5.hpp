#pragma once

#include <iostream>
#include <sstream>
#include <cstring> /* memset */
#include <iomanip> /* setfill() && setw() */
#include <stdint.h> /* uint32_t && uint8_t */

///////////////// MD5 algorithm /////////////////

/* ["https://www.rfc-editor.org/rfc/rfc1321"] */
/* ["https://en.wikipedia.org/wiki/MD5#Algorithm"] */
/* ["http://www.zedwood.com/article/cpp-md5-function"] */

//////////////// extra resources ////////////////

/* ["https://learn.microsoft.com/en-us/cpp/cpp/data-type-ranges?view=msvc-170"] */
/* ["https://en.wikipedia.org/wiki/Endianness"] */


#ifndef MD5_F
#define MD5_F

	/* '^' = XOR ; '&' = AND ; '|' = OR ; '~' = NOT */

	// #define F(B, C, D) (((B) & (C)) | ((~B) & (D)))
	#define F(B, C, D) ((D) ^ ((B) & ((C) ^ (D)))) // For improved efficiency

	// #define G(B, C, D) (((B) & (D)) | ((C) & (~D)))
	#define G(B, C, D) ((C) ^ ((D) & ((B) ^ (C)))) // For improved efficiency

	#define H(B, C, D) ((B) ^ (C) ^ (D))
	#define I(B, C, D) ((C) ^ ((B) | ~(D)))

	#define ROTLEFT(word, bits) (((word) << (bits)) | ((word) >> (32 - (bits))))

	// Rotation is separate from addition to prevent recomputation

	#define FF(A, B, C, D, x, s, ac) {				\
		(A) += F((B), (C), (D)) + (x) + (ac);		\
		(A) = ROTLEFT((A), (s));					\
		(A) += (B);									\
	}

	#define GG(A, B, C, D, x, s, ac) {				\
		(A) += G((B), (C), (D)) + (x) + (ac);		\
		(A) = ROTLEFT((A), (s));					\
		(A) += (B);									\
	}

	#define HH(A, B, C, D, x, s, ac) {				\
		(A) += H((B), (C), (D)) + (x) + (ac);		\
		(A) = ROTLEFT((A), (s));					\
		(A) += (B);									\
	}

	#define II(A, B, C, D, x, s, ac) {				\
		(A) += I((B), (C), (D)) + (x) + (ac);		\
		(A) = ROTLEFT((A), (s));					\
		(A) += (B);									\
	}

#endif

namespace hash {

	class MD5
	{
		private:
			const std::string	_raw;
			std::string			_hash;

			uint8_t		_buffer[64];		// bytes that didn't fit in last 64 byte chunk
			uint32_t	_count[2];			// 64bit counter for number of bits (lo, hi)
			uint32_t	_state[4];
			uint32_t	_M[16];

		public:
			MD5( void );
			MD5( const std::string& );
			~MD5( void );

			const std::string	getRaw( void ) const;
			const std::string	getHash( void ) const;

		private:
			void	hash( void );
			void	setHash( void );

			void	decode( uint32_t output[], const uint8_t input[], uint32_t );
			void	encode( uint8_t output[], const uint32_t input[], uint32_t );

			void	update( const char *, uint32_t );
			void	update( const uint8_t *, uint32_t );

			void	transform( const uint8_t block[64] );
			void	finalize( void );
	};

	const std::string	md5( const std::string & );
}