#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <cstring>

typedef std::vector<unsigned long>	vector_ul;

#ifndef SHA256_M
#define SHA256_M

	#define ROTRIGHT(word, bits) (((word) >> (bits)) | ((word) << (32-(bits))))
	#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
	#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
	#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
	#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

	#define OP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
	#define OP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

#endif

vector_ul	convert_to_binary( const std::string & );		// 1
void		padding( vector_ul & );							// 2
void		resize_block( vector_ul & );					// 3
std::string	compute( const vector_ul & );					// 4

std::string	show_as_hex( const unsigned long & );			// 5 (b)

unsigned long k[64] = {
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

unsigned long static	H0 = 0x6a09e667;
unsigned long static	H1 = 0xbb67ae85;
unsigned long static	H2 = 0x3c6ef372;
unsigned long static	H3 = 0xa54ff53a;
unsigned long static	H4 = 0x510e527f;
unsigned long static	H5 = 0x9b05688c;
unsigned long static	H6 = 0x1f83d9ab;
unsigned long static	H7 = 0x5be0cd19;

/* ------------------- functions ------------------- */
vector_ul	convert_to_binary(const std::string &input) {

	vector_ul	ret;

	for (int i = 0; i < input.size(); ++i) {

		std::bitset<8> b(input.c_str()[i]);
		ret.push_back(b.to_ulong());
	}

	return ret;
}

/* note: modify the function for 512 or more bits */

void	padding(vector_ul &block) {

	int				l = block.size() * 8;
	int				k = 447 - l;

	// add 1
	unsigned long	t1 = 0x80; // bin = 10000000
	block.push_back(t1);

	// add all the 0
	k -= 7;
	for (int i = 0; i < k / 8; i++) // 'cause we add bit 8 by 8
		block.push_back(0x00000000);

	// add l
	std::bitset<64>			bitLength(l);
	std::string		strLength = bitLength.to_string();

	for (int i = 0; i < 63; i += 8) {

		std::bitset<8>	tmp(strLength.substr(i, 8));
		block.push_back(tmp.to_ulong());
	}
}

/* note: error when the message length is more than 55 characters.*/

void	resize_block(vector_ul &input) {

	vector_ul	ret(16);

	// Loop through the 64 sections by 4 steps and merge those 4 sections.
	for (int i = 0; i < 64; i = i + 4) {

		std::bitset<32> tmp(0);

		tmp  = (unsigned long)input[i] << 24;		// 8 * 3
		tmp |= (unsigned long)input[i + 1] << 16;	// 8 * 2
		tmp |= (unsigned long)input[i + 2] << 8;	// 8 * 1
		tmp |= (unsigned long)input[i + 3];			// 8 * 0

		ret[i / 4] = tmp.to_ulong();
	}

	input = ret;
}

std::string	show_as_hex(const unsigned long &input)
{
	std::bitset<32>			bs(input);
	unsigned			n = bs.to_ulong();
	std::string			ret;
	std::stringstream	sstream;

	sstream << std::hex << std::setw(8) << std::setfill('0') << n;
	sstream >> ret;

	return ret;
}

std::string	compute(const vector_ul &block) {

	unsigned long	W[64] = { 0 };

	unsigned long	tmp1 = 0;
	unsigned long	tmp2 = 0;

	unsigned long	a = H0;
	unsigned long	b = H1;
	unsigned long	c = H2;
	unsigned long	d = H3;
	unsigned long	e = H4;
	unsigned long	f = H5;
	unsigned long	g = H6;
	unsigned long	h = H7;

	// creation of 64 (32 bit) word
	for (int t = 0; t < 16; t++) // The 16 words of the block
		W[t] = block[t] & 0xFFFFFFFF;

	for (int t = 16; t < 64; t++) {

		W[t] = SSIG1(W[t-2]) + W[t-7] + SSIG0(W[t-15]) + W[t-16];
		W[t] &= 0xFFFFFFFF; // Have to make sure we are still dealing with 32 bit numbers.
	}

	// The round
	for (int t = 0; t < 64; t++) {

		tmp1 = h + OP1(e) + CH(e, f, g) + k[t] + W[t];
		tmp2 = OP0(a) + MAJ(a, b, c);

		h = g;
		g = f;
		f = e;
		e = (d + tmp1) & 0xFFFFFFFF;
		d = c;
		c = b;
		b = a;
		a = (tmp1 + tmp2) & 0xFFFFFFFF;
	}

	H0 = (H0 + a) & 0xFFFFFFFF;
	H1 = (H1 + b) & 0xFFFFFFFF;
	H2 = (H2 + c) & 0xFFFFFFFF;
	H3 = (H3 + d) & 0xFFFFFFFF;
	H4 = (H4 + e) & 0xFFFFFFFF;
	H5 = (H5 + f) & 0xFFFFFFFF;
	H6 = (H6 + g) & 0xFFFFFFFF;
	H7 = (H7 + h) & 0xFFFFFFFF;

	return	show_as_hex(H0) + show_as_hex(H1) + show_as_hex(H2) +
			show_as_hex(H3) + show_as_hex(H4) + show_as_hex(H5) +
			show_as_hex(H6) + show_as_hex(H7);
}

int main(int argc, char* argv[])
{
	std::string	msg = "";
	std::string	hash = "";

	if (argc != 2)
		return 1;

	msg = argv[1];
	if (msg.length() > 55)
		return 1;
	
	vector_ul	block;

	block = convert_to_binary(msg);
	padding(block);
	resize_block(block);
	hash = compute(block);
	
	std::cout << hash << std::endl;
	return 0;
}