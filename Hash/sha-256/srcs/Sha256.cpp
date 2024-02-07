#include "Sha256.hpp"
#include <stdint.h> /* UINT32_MAX */

static const uint32_t	_K[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,
	0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
	0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,
	0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,
	0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
	0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,
	0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,
	0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
	0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

namespace hash {


	SHA256::SHA256(void) : _raw(""), _blocklen(0) { this->hash(); }

	SHA256::SHA256(const std::string &str) : _raw(str), _blocklen(0) { this->hash(); }

	SHA256::~SHA256(void) {}

	const std::string	SHA256::getRaw(void) const { return _raw; }
	const std::string	SHA256::getHash(void) const { return _hash; }

	void	SHA256::setHash(void) {

		_hash = "";
		for (int i = 0; i < 8; i++) {

			std::string			H;
			std::stringstream	ss;
			std::bitset<32>		bit(_state[i]);
			unsigned			n = bit.to_ulong();

			ss << std::hex << std::setw(8) << std::setfill('0') << n;
			ss >> H;
			_hash += H;
		}
	}

	void	SHA256::hash(void) {

		if (_raw.size() > (UINT32_MAX / 8))
			throw SHA256::sha256Error("the length of the message can't be store in a 64-bit integer");

		// set initial hash value H -> 32 first bit of the fractional parts
		// of the square roots of the first 8 prime numbers:
		_state[0] = 0x6a09e667;
		_state[1] = 0xbb67ae85;
		_state[2] = 0x3c6ef372;
		_state[3] = 0xa54ff53a;
		_state[4] = 0x510e527f;
		_state[5] = 0x9b05688c;
		_state[6] = 0x1f83d9ab;
		_state[7] = 0x5be0cd19;

		for (int i = 0; i < (int)_raw.size(); ++i) {

			std::bitset<8>	bit(_raw.c_str()[i]);
			_blocks.push_back(bit.to_ulong());
		}

		padding();
		resizeBlock();
		compute();
	}

	void	SHA256::padding(void) {

		int		l = _blocks.size() * 8;
		_blocklen = (l / 512) + 1; // number of blocks
		int		k = ((_blocklen * 512) - 64 - 1) - l;

		// add 1
		unsigned long	t1 = 0x80; // bin = 10000000
		_blocks.push_back(t1);

		// add all the 0
		k -= 7;
		for (int i = 0; i < k / 8; i++) // because we add bit 8 by 8
			_blocks.push_back(0x00000000);

		// add l
		std::bitset<64>			bitLength(l);
		std::string		strLength = bitLength.to_string();

		for (int i = 0; i < 63; i += 8) {

			std::bitset<8>	tmp(strLength.substr(i, 8));
			_blocks.push_back(tmp.to_ulong());
		}
	}

	void	SHA256::resizeBlock(void) {

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

		_blocks = ret;
	}

	void	SHA256::constructBlock(const int &nBlock) {

		for (int t = 0; t < 16; t++)
			_W[t] = TO32BIT(_blocks[t + (16 * nBlock)]);

		for (int t = 16; t < 64; t++) {

			_W[t] = SSIG1(_W[t - 2]) + _W[t - 7] + SSIG0(_W[t - 15]) + _W[t - 16];
			TO32BIT(_W[t]);
		}
	}

	void	SHA256::compute(void) {

		uint32_t	T1, T2;

		for (int nBlock = 0; nBlock < (int)_blocklen; nBlock++) {

			constructBlock(nBlock);

			uint32_t	a = _state[0];
			uint32_t	b = _state[1];
			uint32_t	c = _state[2];
			uint32_t	d = _state[3];
			uint32_t	e = _state[4];
			uint32_t	f = _state[5];
			uint32_t	g = _state[6];
			uint32_t	h = _state[7];

			// do 64 rounds
			for (int t = 0; t < 64; t++) {

				T1 = h + OP1(e) + CH(e, f, g) + _K[t] + _W[t];
				T2 = OP0(a) + MAJ(a, b, c);
				h = g;
				g = f;
				f = e;
				e = TO32BIT(d + T1);
				d = c;
				c = b;
				b = a;
				a = TO32BIT(T1 + T2);
			}

			_state[0] = TO32BIT(_state[0] + a);
			_state[1] = TO32BIT(_state[1] + b);
			_state[2] = TO32BIT(_state[2] + c);
			_state[3] = TO32BIT(_state[3] + d);
			_state[4] = TO32BIT(_state[4] + e);
			_state[5] = TO32BIT(_state[5] + f);
			_state[6] = TO32BIT(_state[6] + g);
			_state[7] = TO32BIT(_state[7] + h);
		}
		setHash();
	}

	const std::string	sha256(const std::string &raw) {

		SHA256	ret(raw);
		return ret.getHash();
	}
}
