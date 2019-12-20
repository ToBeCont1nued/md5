#include <cstring>
#include "md5.h"

const uint32_t v_init[] = {
	0x67452301, 0xefcdab89,
	0x98badcfe, 0x10325476
};

const uint32_t T0[16] = {
	0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
	0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
	0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
	0x6b901122,0xfd987193,0xa679438e,0x49b40821
};
const uint32_t T1[16] = {
	0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
	0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
	0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
	0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a
};
const uint32_t T2[16] = {
	0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
	0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
	0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
	0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665
};
const uint32_t T3[16] = {
	0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
	0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
	0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
	0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
};
const uint32_t* T[4] = { T0,T1,T2,T3 };

const uint32_t s0[4] = { 7,12,17,22 };
const uint32_t s1[4] = { 5,9,14,20 };
const uint32_t s2[4] = { 4,11,16,23 };
const uint32_t s3[4] = { 6,10,15,21 };
const uint32_t* s[4] = { s0,s1,s2,s3 };


uint32_t (*L[4])(uint32_t a, uint32_t b, uint32_t c) = {
	[](uint32_t b, uint32_t c, uint32_t d) {return (b & c) | ((~b) & d); },
	[](uint32_t b, uint32_t c, uint32_t d) {return (b & d) | (c & (~d)); },
	[](uint32_t b, uint32_t c, uint32_t d) {return b ^ c ^ d; },
	[](uint32_t b, uint32_t c, uint32_t d) {return c ^ (b | (~d)); }
};

uint32_t (*p[4])(uint32_t i) = {
	[](uint32_t i) {return i; },
	[](uint32_t i) {return (1 + 5 * i) % 16; },
	[](uint32_t i) {return (5 + 3 * i) % 16; },
	[](uint32_t i) {return 7 * i % 16; }
};

uint32_t rol(uint32_t bits, uint32_t src)
{
	uint32_t x = src >> (32 - bits);
	uint32_t y = src << bits;
	return x | y;
}

void step(
	uint32_t &a, uint32_t b, uint32_t c, uint32_t d,
	uint32_t m, uint32_t shift, uint32_t t,
	uint32_t (*q)(uint32_t, uint32_t, uint32_t)
)
{
	a = b + rol(shift, a + q(b, c, d) + m + t);
}

void md5_hash(const char* message, uint32_t* v)
{
	uint32_t message_bin[16];
	for (size_t i = 0; i != 16; ++i)
		message_bin[i] = *(uint32_t*)&message[4 * i];
	
	uint32_t v0[4];
	memcpy(v0, v, 4 * sizeof(uint32_t));
	uint32_t c;
	for (size_t i = 0; i != 4; ++i) {
		c = 0;
		for (size_t j = 0; j != 16; ++j) {
			step(v0[c%4], v0[(c+1)%4], v0[(c+2)%4], v0[(c+3)%4],
				message_bin[p[i](j)], s[i][j%4], T[i][j], L[i]);
			c = (c + 3) % 4;
		}
	}

	for (size_t i = 0; i != 4; ++i)
		v[i] += v0[i];
}

uint8_t* md5(const char* message, uint64_t size, uint8_t digest[32])
{
	uint64_t padding_size = (size + 8 + 64) & (uint64_t)-64;
	
	char *ext_message;
	ext_message = new char[padding_size];
	memset(ext_message, 0, padding_size);
	strncpy(ext_message, message, size);
	ext_message[size] = 128;
	
	uint64_t bits = size * 8;
	*(uint64_t*)&ext_message[padding_size - 8] = bits;

	memcpy(digest, v_init, 4 * sizeof(uint32_t));
	for (size_t i = 0; i != padding_size; i += 64)
		md5_hash(ext_message + i, (uint32_t*)digest);

	delete[] ext_message;
	return digest;
}
