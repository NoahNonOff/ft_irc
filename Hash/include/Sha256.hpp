#pragma once

#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <cstring>
#include <stdint.h> /* uint32_t */

/* ["https://helix.stormhub.org/papers/SHA-256.pdf"] */
/* ["https://csrc.nist.gov/files/pubs/fips/180-4/upd1/final/docs/fips180-4-draft-aug2014.pdf"] */
/* ["https://www.youtube.com/watch?v=orIgy2MjqrA&ab_channel=RedBlockBlue"] */

#ifndef HASH_F
#define HASH_F

	#define TO32BIT(x) (x & 0xFFFFFFFF)

	#define ROTRIGHT(word, bits) (((word) >> (bits)) | ((word) << (32-(bits))))
	#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
	#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
	#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
	#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

	#define OP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
	#define OP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

#endif

namespace HASH {

	class SHA256 {

		public:
			typedef std::vector<uint32_t>	vector32;

		private:
			const std::string	_raw; // should be less than 2^64
			std::string			_hash;

			vector32	_blocks;
			uint32_t	_blocklen;

			uint8_t		_W[64];
			uint32_t	_state[8]; // A, B, C, D, E, F, G, H

		public:
			SHA256( void );
			SHA256( const std::string & );
			~SHA256( void );

			const std::string	getRaw( void ) const;
			const std::string	getHash( void ) const;

			void				setHash( void );

			void				hash( void );
			void				padding( void );
			void				resizeBlock( void );
			void				constructBlock( const int & );
			void				compute( void );

			//Exceptions
			class sha256Error : public std::exception {
				private:
					std::string	_msg;
				public:
					sha256Error(std::string message) : _msg(message) {}
					virtual ~sha256Error() throw() {}
					virtual const char *what(void) const throw() { return _msg.c_str(); }
			};

	};

	const std::string	sha256( const std::string & );

}