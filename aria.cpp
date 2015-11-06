// Implementation of ARIA Cryptosystem with 128 bit key
// Written by Siddharth Ramachandran 
// November 1, 2015

#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <algorithm>
#include <stdlib.h>
using namespace boost::multiprecision;

// Intialise S-Box values
	static const uint8_t sb1[256] =
   {
     0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
     0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
     0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
     0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
     0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
     0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
     0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
     0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
     0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
     0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
     0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
     0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
     0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
     0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
     0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
     0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
   };

   static const uint8_t sb2[256] =
  {
     0xE2, 0x4E, 0x54, 0xFC, 0x94, 0xC2, 0x4A, 0xCC, 0x62, 0x0D, 0x6A, 0x46, 0x3C, 0x4D, 0x8B, 0xD1,
     0x5E, 0xFA, 0x64, 0xCB, 0xB4, 0x97, 0xBE, 0x2B, 0xBC, 0x77, 0x2E, 0x03, 0xD3, 0x19, 0x59, 0xC1,
     0x1D, 0x06, 0x41, 0x6B, 0x55, 0xF0, 0x99, 0x69, 0xEA, 0x9C, 0x18, 0xAE, 0x63, 0xDF, 0xE7, 0xBB,
     0x00, 0x73, 0x66, 0xFB, 0x96, 0x4C, 0x85, 0xE4, 0x3A, 0x09, 0x45, 0xAA, 0x0F, 0xEE, 0x10, 0xEB,
     0x2D, 0x7F, 0xF4, 0x29, 0xAC, 0xCF, 0xAD, 0x91, 0x8D, 0x78, 0xC8, 0x95, 0xF9, 0x2F, 0xCE, 0xCD,
     0x08, 0x7A, 0x88, 0x38, 0x5C, 0x83, 0x2A, 0x28, 0x47, 0xDB, 0xB8, 0xC7, 0x93, 0xA4, 0x12, 0x53,
     0xFF, 0x87, 0x0E, 0x31, 0x36, 0x21, 0x58, 0x48, 0x01, 0x8E, 0x37, 0x74, 0x32, 0xCA, 0xE9, 0xB1,
     0xB7, 0xAB, 0x0C, 0xD7, 0xC4, 0x56, 0x42, 0x26, 0x07, 0x98, 0x60, 0xD9, 0xB6, 0xB9, 0x11, 0x40,
     0xEC, 0x20, 0x8C, 0xBD, 0xA0, 0xC9, 0x84, 0x04, 0x49, 0x23, 0xF1, 0x4F, 0x50, 0x1F, 0x13, 0xDC,
     0xD8, 0xC0, 0x9E, 0x57, 0xE3, 0xC3, 0x7B, 0x65, 0x3B, 0x02, 0x8F, 0x3E, 0xE8, 0x25, 0x92, 0xE5,
     0x15, 0xDD, 0xFD, 0x17, 0xA9, 0xBF, 0xD4, 0x9A, 0x7E, 0xC5, 0x39, 0x67, 0xFE, 0x76, 0x9D, 0x43,
     0xA7, 0xE1, 0xD0, 0xF5, 0x68, 0xF2, 0x1B, 0x34, 0x70, 0x05, 0xA3, 0x8A, 0xD5, 0x79, 0x86, 0xA8,
     0x30, 0xC6, 0x51, 0x4B, 0x1E, 0xA6, 0x27, 0xF6, 0x35, 0xD2, 0x6E, 0x24, 0x16, 0x82, 0x5F, 0xDA,
     0xE6, 0x75, 0xA2, 0xEF, 0x2C, 0xB2, 0x1C, 0x9F, 0x5D, 0x6F, 0x80, 0x0A, 0x72, 0x44, 0x9B, 0x6C,
     0x90, 0x0B, 0x5B, 0x33, 0x7D, 0x5A, 0x52, 0xF3, 0x61, 0xA1, 0xF7, 0xB0, 0xD6, 0x3F, 0x7C, 0x6D,
     0xED, 0x14, 0xE0, 0xA5, 0x3D, 0x22, 0xB3, 0xF8, 0x89, 0xDE, 0x71, 0x1A, 0xAF, 0xBA, 0xB5, 0x81
  };

  static const uint8_t sb3[256] =
  {
     0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
     0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
     0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
     0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
     0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
     0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
     0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
     0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
     0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
     0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
     0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
     0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
     0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
     0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
     0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
     0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
  };

static const uint8_t sb4[256] =
  {
     0x30, 0x68, 0x99, 0x1B, 0x87, 0xB9, 0x21, 0x78, 0x50, 0x39, 0xDB, 0xE1, 0x72, 0x09, 0x62, 0x3C,
     0x3E, 0x7E, 0x5E, 0x8E, 0xF1, 0xA0, 0xCC, 0xA3, 0x2A, 0x1D, 0xFB, 0xB6, 0xD6, 0x20, 0xC4, 0x8D,
     0x81, 0x65, 0xF5, 0x89, 0xCB, 0x9D, 0x77, 0xC6, 0x57, 0x43, 0x56, 0x17, 0xD4, 0x40, 0x1A, 0x4D,
     0xC0, 0x63, 0x6C, 0xE3, 0xB7, 0xC8, 0x64, 0x6A, 0x53, 0xAA, 0x38, 0x98, 0x0C, 0xF4, 0x9B, 0xED,
     0x7F, 0x22, 0x76, 0xAF, 0xDD, 0x3A, 0x0B, 0x58, 0x67, 0x88, 0x06, 0xC3, 0x35, 0x0D, 0x01, 0x8B,
     0x8C, 0xC2, 0xE6, 0x5F, 0x02, 0x24, 0x75, 0x93, 0x66, 0x1E, 0xE5, 0xE2, 0x54, 0xD8, 0x10, 0xCE,
     0x7A, 0xE8, 0x08, 0x2C, 0x12, 0x97, 0x32, 0xAB, 0xB4, 0x27, 0x0A, 0x23, 0xDF, 0xEF, 0xCA, 0xD9,
     0xB8, 0xFA, 0xDC, 0x31, 0x6B, 0xD1, 0xAD, 0x19, 0x49, 0xBD, 0x51, 0x96, 0xEE, 0xE4, 0xA8, 0x41,
     0xDA, 0xFF, 0xCD, 0x55, 0x86, 0x36, 0xBE, 0x61, 0x52, 0xF8, 0xBB, 0x0E, 0x82, 0x48, 0x69, 0x9A,
     0xE0, 0x47, 0x9E, 0x5C, 0x04, 0x4B, 0x34, 0x15, 0x79, 0x26, 0xA7, 0xDE, 0x29, 0xAE, 0x92, 0xD7,
     0x84, 0xE9, 0xD2, 0xBA, 0x5D, 0xF3, 0xC5, 0xB0, 0xBF, 0xA4, 0x3B, 0x71, 0x44, 0x46, 0x2B, 0xFC,
     0xEB, 0x6F, 0xD5, 0xF6, 0x14, 0xFE, 0x7C, 0x70, 0x5A, 0x7D, 0xFD, 0x2F, 0x18, 0x83, 0x16, 0xA5,
     0x91, 0x1F, 0x05, 0x95, 0x74, 0xA9, 0xC1, 0x5B, 0x4A, 0x85, 0x6D, 0x13, 0x07, 0x4F, 0x4E, 0x45,
     0xB2, 0x0F, 0xC9, 0x1C, 0xA6, 0xBC, 0xEC, 0x73, 0x90, 0x7B, 0xCF, 0x59, 0x8F, 0xA1, 0xF9, 0x2D,
     0xF2, 0xB1, 0x00, 0x94, 0x37, 0x9F, 0xD0, 0x2E, 0x9C, 0x6E, 0x28, 0x3F, 0x80, 0xF0, 0x3D, 0xD3,
     0x25, 0x8A, 0xB5, 0xE7, 0x42, 0xB3, 0xC7, 0xEA, 0xF7, 0x4C, 0x11, 0x33, 0x03, 0xA2, 0xAC, 0x60
  };

  
uint128_t CK1, CK2, CK3, KL, KR, W0, W1, W2, W3;
uint128_t ek[18], dk[18];

// 8 bit XOR
int xor_8(int x, int y)
{
	return (~(x & y) & ~(~x & ~y));
}
uint128_t a(uint128_t v1)
{
	int x[16],y[16],i=0;
	uint128_t out=0x0;
	for (i=0;i<16;i++)
	{
		uint128_t val= v1 & 0xff;
		v1=v1>>8;
		x[15-i]= val.template convert_to<int>();
	}

	 y[0] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[3],x[4]),x[6]),x[8]),x[9]),x[13]),x[14]);
	 y[1] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[2],x[5]),x[7]),x[8]),x[9]),x[12]),x[15]);
	 y[2] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[1],x[4]),x[6]),x[10]),x[11]),x[12]),x[15]);
	 y[3] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[0],x[5]),x[7]),x[10]),x[11]),x[13]),x[14]);
	 y[4] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[0],x[2]),x[5]),x[8]),x[11]),x[14]),x[15]);
	 y[5] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[1],x[3]),x[4]),x[9]),x[10]),x[14]),x[15]);
	 y[6] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[0],x[2]),x[7]),x[9]),x[10]),x[12]),x[13]);
	 y[7] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[1],x[3]),x[6]),x[8]),x[11]),x[12]),x[13]);
	 y[8] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[0],x[1]),x[4]),x[7]),x[10]),x[13]),x[15]);
	 y[9] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[0],x[1]),x[5]),x[6]),x[11]),x[12]),x[14]);
	 y[10] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[2],x[3]),x[5]),x[6]),x[8]),x[13]),x[15]);
	 y[11] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[2],x[3]),x[4]),x[7]),x[9]),x[12]),x[14]);
	 y[12] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[1],x[2]),x[6]),x[7]),x[9]),x[11]),x[12]);
	 y[13] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[0],x[3]),x[6]),x[7]),x[8]),x[10]),x[13]);
	 y[14] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[0],x[3]),x[4]),x[5]),x[9]),x[11]),x[14]);
	 y[15] = xor_8(xor_8(xor_8(xor_8(xor_8(xor_8(x[1],x[2]),x[4]),x[5]),x[8]),x[10]),x[15]);

	 for(i=0;i<16;i++)
	{
		out=out<<8;
		out = out | y[i];
	}

return out;
	
}

//Substitution Layer 1

uint128_t sl1(uint128_t v1)
{
	int x[16],y[16],i=0;
	uint128_t out=0x0;
	for (i=0;i<16;i++)
	{
		uint128_t val= v1 & 0xff;
		v1=v1>>8;
		x[15-i]= val.template convert_to<int>();
	}

	for (i=0; i<16;i+=4)
	{
		y[i] = sb1[x[i]];
		y[i+1] = sb2[x[i+1]];
		y[i+2] = sb3[x[i+2]];
		y[i+3] = sb4[x[i+3]];
	}

	for(i=0;i<16;i++)
	{
		out=out<<8;
		out = out | y[i];
	}

return out;

}
//Substitution Layer 2

uint128_t sl2(uint128_t v1)
{
	int x[16],y[16],i=0;
	uint128_t out=0x0;
	for (i=0;i<16;i++)
	{
		uint128_t val= v1 & 0xff;
		v1=v1>>8;
		x[15-i]= val.template convert_to<int>();
	}
	for (i=0; i<16;i+=4)
	{
		y[i] = sb3[x[i]];
		y[i+1] = sb4[x[i+1]];
		y[i+2] = sb1[x[i+2]];
		y[i+3] = sb2[x[i+3]];
	}
	for(i=0;i<16;i++)
	{
		out=out<<8;
		out = out | y[i];
	}
return out;

}

// 128 bit XOR

uint128_t xor_128(uint128_t x, uint128_t y )
{
	return (~(x & y) & ~(~x & ~y));
}

// Round function for odd rounds

uint128_t fo(uint128_t d, uint128_t rk)
{
	return a(sl1(xor_128(d,rk)));
}

// Round function for even rounds 

uint128_t fe(uint128_t d, uint128_t rk)
{
	return a(sl2(xor_128(d,rk)));
}

// 128 bit circular right shift 

uint128_t ror_128(uint128_t x, int num)
{
	 return (x >> num) | (x << (128-num));
}

// 128 bit circular left shift

uint128_t rol_128(uint128_t x, int num)
{
	return (x << num) | (x >> (128-num));
}

// Initialise the constants

void init_const()
{
	uint32_t x1=0x517CC1B7;
	uint32_t x2=0x27220A94;
	uint32_t x3=0xFE13ABE8;
	uint32_t x4=0xFA9A6EE0;
	CK1=x1;
	CK1=CK1<<32;
	CK1=CK1+x2;
	CK1=CK1<<32;
	CK1=CK1+x3;
	CK1=CK1<<32;
	CK1=CK1+x4;
	x1=0x6DB14ACC;
	x2=0x9E21C820;
	x3=0xFF28B1D5;
	x4=0xEF5DE2B0;
	CK2=x1;
	CK2=CK2<<32;
	CK2=CK2+x2;
	CK2=CK2<<32;
	CK2=CK2+x3;
	CK2=CK2<<32;
	CK2=CK2+x4;
	x1=0xDB92371D;
	x2=0x2126E970;
	x3=0x03249775;
	x4=0x04E8C90E;
	CK3=x1;
	CK3=CK3<<32;
	CK3=CK3+x2;
	CK3=CK3<<32;
	CK3=CK3+x3;
	CK3=CK3<<32;
	CK3=CK3+x4;
	
	//Fiestel Round value init
	W0 = KL;
	W1 = xor_128(fo(W0,CK1),KR);
	W2 = xor_128(fe(W1,CK2),W0);
	W3 = xor_128(fo(W2,CK3),W1);
}

// Rounds key generation

void round_key_gen()
{
	int i=1, num=19;
	for (i=1; i <= 8; i+=4)
	{
		ek[i] = xor_128(W0,ror_128(W1,num));
		ek[i+1] = xor_128(W1,ror_128(W2,num));
		ek[i+2] = xor_128(W2,ror_128(W3,num));
		ek[i+3] = xor_128(ror_128(W0,num),W3);
		num+=12;
	}
	num=61;
	for(i=9; i<= 16; i+=4)
	{
		ek[i] = xor_128(W0,rol_128(W1,num));
		ek[i+1] = xor_128(W1, rol_128(W2,num));
		ek[i+2] = xor_128(W2, rol_128(W3,num));
		ek[i+3] = xor_128(rol_128(W0,num),W3);
		num-=30;
	}
	ek[17] = xor_128(W0, rol_128(W1,19));
}

// Perform 12 round encryption using appropriate round function

uint128_t encrypt(uint128_t P)
{
	int i=1;
	for (i=1;i<=11;i++)
	{
		if(i % 2 == 1)
		{
			P = fo(P,ek[i]);
		}
		else if (i % 2 == 0)
		{
			P = fe(P,ek[i]);
		}
	}
	return (xor_128(sl2(xor_128(P,ek[12])),ek[13])); 

}

// Decryption round key generation

void decrypt_round_key_gen()
{
	int i=0, count=2;
	dk[1] = ek[13];
	for (i=12; i >= 2; --i)
	{
		dk[count] = a(ek[i]); 
		++count;
	}
	dk[count] = ek[1];
}

// Performs the decryption 

uint128_t decrypt(uint128_t P)
{
	int i=1;
	for (i=1;i<=11;i++)
	{
		if(i % 2 == 1)
		{
			P = fo(P,dk[i]);
		}
		else if (i % 2 == 0)
		{
			P = fe(P,dk[i]);
		}
	}
	return (xor_128(sl2(xor_128(P,dk[12])),dk[13])); 

}

int main() {

int i=0;
char choice;
std::string key;
std::cout<<"Enter 128 bit key: \n";

std::getline(std::cin,key);

for(i=0;i<32;i+=2)
{
	
	int x = strtoul(key.substr(i,2).c_str(), NULL, 16);
	KL = KL << 8;
	KL = KL | x;

}
init_const();
round_key_gen();


while (1)
{
uint128_t P, cipher_text, plain_text, temp;
int count =0,  g=0;
uint8_t x=0;
std::string input, output, tp;
std::cout<<"1. Encrypt \n";
std::cout<<"2. Decrypt \n";
std::cout<<"3. Exit \n";
std::cout<<"\n\nEnter choice: ";
std::cin>>choice;
switch (choice)
{
	case '1':   // Encrypt
		input.clear();
		std::cin.ignore();
		std::cout<<"Enter a string: \n";
		
		std::getline(std::cin, input);
		std::cout<<"Cipher Text: ";
		count =0;
		for (i=0;i<input.length();++i)
		{
			++count;
			P = P << 8;
			uint8_t t= input[i];
			P = P | t;
			if (count == 16)
			{
				cipher_text=encrypt(P);
				std::cout<<std::hex<<cipher_text;
				count=0;
				P = P << 128;
			}
		}	
		if (count < 16 && count > 0)
		{
			P = P << ((16 - count)*8);
			cipher_text=encrypt(P);
			std::cout<<std::hex<<cipher_text;
		}
		std::cout<<"\n\n\n";
	break;

	case '2':  // Decrypt
		decrypt_round_key_gen();
		input.clear();
		std::cin.ignore();
		std::cout<<"Enter cipher text: \n";
	
		std::getline(std::cin, input);
		for (i=0;i<input.length();i+=2)
		{
			count+=2;
			x = strtoul(input.substr(i,2).c_str(), NULL, 16);
			cipher_text = cipher_text<<8;
			cipher_text = cipher_text | x;
			
			if (count == 32)
			{

				plain_text = decrypt(cipher_text);
				for(g=0;g<16;g++)
				{
					temp = plain_text & 0xff;
					plain_text = plain_text >> 8;
					tp.push_back(temp.template convert_to<int>());
				}
				count=0;
				cipher_text = cipher_text << 128;
				std::reverse(tp.begin(), tp.end());
				output.append(tp);
				tp.clear();
			}
		}
		std::cout<<"\nThe plain text: "<<output<<"\n\n\n";
	break;
	case '3':
		return 0;
	break;
}
}
return 0;

}