#include "Md5.hpp"

// T[i] = 4294967296 * abs(sin(i)), where i is in radians.
static const uint32_t	T[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// s specifies the per-round shift amounts
static const uint8_t	s[64] = {
	7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

namespace hash {

	MD5::MD5(void) : _raw(""), _blocklen(0) { this->hash(); }

	MD5::MD5(const std::string &str) : _raw(str), _blocklen(0) { this->hash(); }

	MD5::~MD5(void) {}

	const std::string	MD5::getRaw(void) const { return _raw; }
	const std::string	MD5::getHash(void) const { return _hash; }

	void	MD5::setHash(void) {

		_hash = "";
		for (int i = 0; i < 4; i++) {

			std::string			H;
			std::stringstream	ss;
			std::bitset<32>		bit(_state[i]);
			unsigned			n = bit.to_ulong();

			ss << std::hex << std::setw(8) << std::setfill('0') << n;
			ss >> H;
			_hash += H;
			if (i < 3)
				_hash += " ";
		}
	}

	void	MD5::hash(void) {

		_state[0] = 0x67452301;
		_state[1] = 0xefcdab89;
		_state[2] = 0x98badcfe;
		_state[3] = 0x10325476;

		// BE -> 10110001
		// LE    10110001 <-
		// sha-2 use big endian (BE) AND md5 use little endian (LE)
		for (int i = 0; i < (int)_raw.size(); ++i) {

			std::bitset<8>	bit(_raw.c_str()[i]);
			_blocks.push_back(bit.to_ulong());
		}

		padding();
		compute();
	}

	void	MD5::padding(void) {

		int		l = _blocks.size() * 8;
		_blocklen = (l / 512) + 1;
		int		k = ((_blocklen * 512) - 64 - 1) - l;

		unsigned long	t1 = 0x80; // 0x1
		_blocks.push_back(t1);

		k -= 7;
		for (int i = 0; i < k / 8; i++)
			_blocks.push_back(0x00000000);

		std::bitset<64>			bitLength(l);
		std::string		strLength = bitLength.to_string();

		for (int i = 0; i < 63; i += 8) {

			std::bitset<8>	tmp(strLength.substr(i, 8));
			_blocks.push_back(tmp.to_ulong());
		}
		resizeBlock();
	}

	void	MD5::resizeBlock(void) {

		vector32	ret(16 * _blocklen);

		// create 16 words of 32 bit for each block
		for (int i = 0; i < (int)(64 * _blocklen); i += 4) {

			std::bitset<32> tmp(0);

			tmp  = (uint32_t)_blocks[i + 0] << 24;	// 8 * 3
			tmp |= (uint32_t)_blocks[i + 1] << 16;	// 8 * 2
			tmp |= (uint32_t)_blocks[i + 2] << 8;	// 8 * 1
			tmp |= (uint32_t)_blocks[i + 3] << 0;	// 8 * 0

			ret[i / 4] = tmp.to_ulong();
		}

		for (int i = 0; i < 64 * (int)_blocklen; i++)
			std::cout << "_blocks[" << i << "] : " << std::hex << _blocks[i] << std::endl;
		std::cout << "--------------------" << std::endl;
		_blocks = ret;
		for (int i = 0; i < 16 * (int)_blocklen; i++)
			std::cout << "_blocks[" << i << "] : " << std::hex << _blocks[i] << std::endl;
	}

	void	MD5::constructBlock(const int &nBlock) {

		for (int j = 0; j < 16; j++)
			_M[j] = TO32BIT(_blocks[j + (16 * nBlock)]);
	}

	/*
		_M should be:
		x[ 0] : 1953719668
		x[ 1] : 128
		x[ 2] : 0
		x[ 3] : 0
		x[ 4] : 0
		x[ 5] : 0
		x[ 6] : 0
		x[ 7] : 0
		x[ 8] : 0
		x[ 9] : 0
		x[10] : 0
		x[11] : 0
		x[12] : 0
		x[13] : 0
		x[14] : 32
		x[15] : 0
	*/

	void	MD5::compute(void) {

		for (int nBlock = 0; nBlock < (int)_blocklen; nBlock++) {

			std::cout << "nBlock : " << nBlock << std::endl;

			uint32_t	a = _state[0];
			uint32_t	b = _state[1];
			uint32_t	c = _state[2];
			uint32_t	d = _state[3];
			uint32_t	F = 0, g = 0;


			constructBlock(nBlock);

			for (int i = 0; i < 16; i++)
				std::cout << "_M[" << i << "] : " << std::hex << _M[i] << std::endl;

			std::cout << "--------------------" << std::endl;

			_M[ 0] = 0x74736574;
			_M[ 1] = 0x80;
			_M[ 2] = 0x0;
			_M[ 3] = 0x0;
			_M[ 4] = 0x0;
			_M[ 5] = 0x0;
			_M[ 6] = 0x0;
			_M[ 7] = 0x0;
			_M[ 8] = 0x0;
			_M[ 9] = 0x0;
			_M[10] = 0x0;
			_M[11] = 0x0;
			_M[12] = 0x0;
			_M[13] = 0x0;
			_M[14] = 0x20;
			_M[15] = 0x0;

			for (int i = 0; i < 16; i++)
				std::cout << "_M[" << i << "] : " << std::hex << _M[i] << std::endl;

			// do 64 rounds
			for (int i = 0; i < 64; i++) {

				if (0 <= i && i < 16) {
					F = F(b, c, d);
					g = i;
				} else if (i < 32) {
					F = G(b, c, d);
					g = (5 * i + 1) % 16;
				} else if (i < 48) {
					F = H(b, c, d);
					g = (3 * i + 5) % 16;
				} else {
					F = I(b, c, d);
					g = (7 * i ) % 16;
				}

				F += a + T[i] + _M[g];
				a = d;
				d = c;
				c = b;
				b += ROTLEFT(F, s[i]);
			}

			_state[0] = _state[0] + a;
			_state[1] = _state[1] + b;
			_state[2] = _state[2] + c;
			_state[3] = _state[3] + d;
		}
		setHash();
	}

	const std::string	md5(const std::string &raw) {

		MD5	ret(raw);
		return ret.getHash();
	}
}

/*

	FF(a, b, c, d, _M[], s[i], T[i])

	for (int i = 0; i < 64; i++) {

		if (0 <= i && i < 16) {
			FF(a, b, c, d, _M[i], s[i], T[i]);
		} else if (i < 32) {
			GG(a, b, c, d, _M[(5 * i + 1) % 16], s[i], T[i]);
		}  else if (i < 48) {
			HH(a, b, c, d, _M[(3 * i + 5) % 16], s[i], T[i]);
		}  else {
			II(a, b, c, d, _M[(7 * i ) % 16], s[i], T[i]);
		}
	}

*/

/*
	A faire :
		- verifier que l'algo fonctione avec les bonnes valeurs
		- convertir au little endian
*/