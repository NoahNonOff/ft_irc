#include "../include/Md5.hpp"

////////////////

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

////////////////

namespace hash {

	MD5::MD5(void) : _raw("") { hash(); }

	MD5::MD5(const std::string &raw) : _raw(raw) { hash(); }

	MD5::~MD5(void) {}

	const std::string	MD5::getRaw(void) const { return _raw; }
	const std::string	MD5::getHash(void) const { return _hash; }

	void	MD5::setHash(void) {

		_hash = "";
		uint8_t	digest[16];
		encode(digest, _state, 16);

		for (int i = 0; i < 16; i++) {

			std::string			H;
			std::stringstream	ss;

			ss << std::setw(2) << std::setfill('0') << std::hex << (int)digest[i];
			ss >> H;
			_hash += H;
		}
	}

	void	MD5::hash(void) {

		_count[0] = 0;
		_count[1] = 0;

		_state[0] = 0x67452301;
		_state[1] = 0xefcdab89;
		_state[2] = 0x98badcfe;
		_state[3] = 0x10325476;

		update(_raw.c_str(), _raw.length());
		finalize();
	}

	void	MD5::decode(uint32_t output[], const uint8_t input[], uint32_t len) {

		for (uint32_t i = 0, j = 0; j < len; i++, j += 4) {
			output[i] = ((uint32_t)input[j]) | (((uint32_t)input[j + 1]) << 8) |
				(((uint32_t)input[j + 2]) << 16) | (((uint32_t)input[j + 3]) << 24);
		}
	}

	void	MD5::encode(uint8_t output[], const uint32_t input[], uint32_t len) {

		for (uint32_t i = 0, j = 0; j < len; i++, j += 4) {

			/* (0xff) = (255) = (uint8_t max) */
			output[j + 0] = input[i] & 0xff;
			output[j + 1] = (input[i] >> 8) & 0xff;
			output[j + 2] = (input[i] >> 16) & 0xff;
			output[j + 3] = (input[i] >> 24) & 0xff;
		}
	}

	// for convenience provide a verson with signed char
	void	MD5::update(const char input[], uint32_t length) { update((const uint8_t*)input, length); }

	void	MD5::update(const uint8_t input[], uint32_t length) {

		uint32_t	index = (uint32_t)((_count[0] >> 3) & 0x3F); // same as (_count[0] / 8) % 64

		if ((_count[0] += (length << 3)) < (length << 3)) // (length << 3) = (length * 8)
			_count[1]++;
		_count[1] += (length >> 29);

		// number of bytes we need to fill in buffer
		uint32_t	partLen = 64 - index;
		uint32_t	i = 0;

		if (length >= partLen)
		{
			// fill buffer first, transform
			memcpy(&_buffer[index], input, partLen);
			transform(_buffer);

			// transform chunks of blocksize (64 bytes)
			for (i = partLen; i + 64 <= length; i += 64)
				transform(&input[i]);
			index = 0;
		}
		memcpy(&_buffer[index], &input[i], length - i);
	}

	// apply MD5 algo on a block
	void	MD5::transform(const uint8_t block[64]) {

		decode(_M, block, 64);

		uint32_t	a = _state[0];
		uint32_t	b = _state[1];
		uint32_t	c = _state[2];
		uint32_t	d = _state[3];

		/* Round 1 */
		FF (a, b, c, d, _M[ 0], S11, 0xd76aa478); /* 1 */
		FF (d, a, b, c, _M[ 1], S12, 0xe8c7b756); /* 2 */
		FF (c, d, a, b, _M[ 2], S13, 0x242070db); /* 3 */
		FF (b, c, d, a, _M[ 3], S14, 0xc1bdceee); /* 4 */
		FF (a, b, c, d, _M[ 4], S11, 0xf57c0faf); /* 5 */
		FF (d, a, b, c, _M[ 5], S12, 0x4787c62a); /* 6 */
		FF (c, d, a, b, _M[ 6], S13, 0xa8304613); /* 7 */
		FF (b, c, d, a, _M[ 7], S14, 0xfd469501); /* 8 */
		FF (a, b, c, d, _M[ 8], S11, 0x698098d8); /* 9 */
		FF (d, a, b, c, _M[ 9], S12, 0x8b44f7af); /* 10 */
		FF (c, d, a, b, _M[10], S13, 0xffff5bb1); /* 11 */
		FF (b, c, d, a, _M[11], S14, 0x895cd7be); /* 12 */
		FF (a, b, c, d, _M[12], S11, 0x6b901122); /* 13 */
		FF (d, a, b, c, _M[13], S12, 0xfd987193); /* 14 */
		FF (c, d, a, b, _M[14], S13, 0xa679438e); /* 15 */
		FF (b, c, d, a, _M[15], S14, 0x49b40821); /* 16 */

		/* Round 2 */
		GG (a, b, c, d, _M[ 1], S21, 0xf61e2562); /* 17 */
		GG (d, a, b, c, _M[ 6], S22, 0xc040b340); /* 18 */
		GG (c, d, a, b, _M[11], S23, 0x265e5a51); /* 19 */
		GG (b, c, d, a, _M[ 0], S24, 0xe9b6c7aa); /* 20 */
		GG (a, b, c, d, _M[ 5], S21, 0xd62f105d); /* 21 */
		GG (d, a, b, c, _M[10], S22,  0x2441453); /* 22 */
		GG (c, d, a, b, _M[15], S23, 0xd8a1e681); /* 23 */
		GG (b, c, d, a, _M[ 4], S24, 0xe7d3fbc8); /* 24 */
		GG (a, b, c, d, _M[ 9], S21, 0x21e1cde6); /* 25 */
		GG (d, a, b, c, _M[14], S22, 0xc33707d6); /* 26 */
		GG (c, d, a, b, _M[ 3], S23, 0xf4d50d87); /* 27 */
		GG (b, c, d, a, _M[ 8], S24, 0x455a14ed); /* 28 */
		GG (a, b, c, d, _M[13], S21, 0xa9e3e905); /* 29 */
		GG (d, a, b, c, _M[ 2], S22, 0xfcefa3f8); /* 30 */
		GG (c, d, a, b, _M[ 7], S23, 0x676f02d9); /* 31 */
		GG (b, c, d, a, _M[12], S24, 0x8d2a4c8a); /* 32 */

		/* Round 3 */
		HH (a, b, c, d, _M[ 5], S31, 0xfffa3942); /* 33 */
		HH (d, a, b, c, _M[ 8], S32, 0x8771f681); /* 34 */
		HH (c, d, a, b, _M[11], S33, 0x6d9d6122); /* 35 */
		HH (b, c, d, a, _M[14], S34, 0xfde5380c); /* 36 */
		HH (a, b, c, d, _M[ 1], S31, 0xa4beea44); /* 37 */
		HH (d, a, b, c, _M[ 4], S32, 0x4bdecfa9); /* 38 */
		HH (c, d, a, b, _M[ 7], S33, 0xf6bb4b60); /* 39 */
		HH (b, c, d, a, _M[10], S34, 0xbebfbc70); /* 40 */
		HH (a, b, c, d, _M[13], S31, 0x289b7ec6); /* 41 */
		HH (d, a, b, c, _M[ 0], S32, 0xeaa127fa); /* 42 */
		HH (c, d, a, b, _M[ 3], S33, 0xd4ef3085); /* 43 */
		HH (b, c, d, a, _M[ 6], S34,  0x4881d05); /* 44 */
		HH (a, b, c, d, _M[ 9], S31, 0xd9d4d039); /* 45 */
		HH (d, a, b, c, _M[12], S32, 0xe6db99e5); /* 46 */
		HH (c, d, a, b, _M[15], S33, 0x1fa27cf8); /* 47 */
		HH (b, c, d, a, _M[ 2], S34, 0xc4ac5665); /* 48 */

		/* Round 4 */
		II (a, b, c, d, _M[ 0], S41, 0xf4292244); /* 49 */
		II (d, a, b, c, _M[ 7], S42, 0x432aff97); /* 50 */
		II (c, d, a, b, _M[14], S43, 0xab9423a7); /* 51 */
		II (b, c, d, a, _M[ 5], S44, 0xfc93a039); /* 52 */
		II (a, b, c, d, _M[12], S41, 0x655b59c3); /* 53 */
		II (d, a, b, c, _M[ 3], S42, 0x8f0ccc92); /* 54 */
		II (c, d, a, b, _M[10], S43, 0xffeff47d); /* 55 */
		II (b, c, d, a, _M[ 1], S44, 0x85845dd1); /* 56 */
		II (a, b, c, d, _M[ 8], S41, 0x6fa87e4f); /* 57 */
		II (d, a, b, c, _M[15], S42, 0xfe2ce6e0); /* 58 */
		II (c, d, a, b, _M[ 6], S43, 0xa3014314); /* 59 */
		II (b, c, d, a, _M[13], S44, 0x4e0811a1); /* 60 */
		II (a, b, c, d, _M[ 4], S41, 0xf7537e82); /* 61 */
		II (d, a, b, c, _M[11], S42, 0xbd3af235); /* 62 */
		II (c, d, a, b, _M[ 2], S43, 0x2ad7d2bb); /* 63 */
		II (b, c, d, a, _M[ 9], S44, 0xeb86d391); /* 64 */

		_state[0] += a;
		_state[1] += b;
		_state[2] += c;
		_state[3] += d;

		memset(_M, 0, sizeof(_M));
	}

	void	MD5::finalize(void) {

		const static uint8_t	padding[64] = { // 0x80 -> 10000000
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		// Save number of bits
		uint8_t	bits[8];
		encode(bits, _count, 8);

		// pad out to 56 mod 64.
		uint32_t	index = (uint32_t)((_count[0] >> 3) & 0x3F);
		uint32_t	padLen = (index < 56) ? (56 - index) : (120 - index);

		update(padding, padLen);
		
		// Append length (before padding)
		update(bits, 8);

		setHash();
	}

	const std::string	md5(const std::string &raw) {

		MD5	ret(raw);
		return ret.getHash();
	}
}
