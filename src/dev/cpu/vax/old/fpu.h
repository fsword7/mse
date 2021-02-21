/*
 * float.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Tim Stark
 *
 *  Float instructions
 *
 */

#pragma once

// Result codes
//#define VFP_OK		0x00 // Successful
//#define VFP_CCMSK   0x0F // Condition codes
//#define VFP_OVFL    CC_V // Overflow Error
//#define VFP_UNFL	0x10 // Underflow Error, FP = 0
//#define VFP_DZRO	0x20 // Divide by Zero
//#define VFP_FAULT	0x80 // Fault

#define VFP_OK		0 // Successful
#define VFP_OVFL	1 // Overflow Error
#define VFP_UNFL	2 // Underflow Error, FP = 0
#define VFP_DZRO	3 // Divide by Zero
#define VFP_FAULT	4 // Reserved Operand Fault
#define VFP_ERROR	5 // Coding error

// Integer size
#define VFP_BYTE	0 // Byte
#define VFP_WORD	1 // Word
#define VFP_LONG	2 // Longword
#define VFP_RLONG	3 // Longword with round

// Word-swapped floating
#define FP_SWAP(fp)		((ZXTL(fp) >> 16) | (ZXTL(fp) << 16))

#define FP_SIGN			0x00008000 // sign bit in word-swapped floating value

#define FP_MASK32		0x0000FFFF // 32-bit mask field

#define FP_P_NORM		63
#define FP_NORM			0x8000000000000000ULL
#define FP_MASK64       0xFFFFFFFFFFFFFFFFULL
#define SFP_ROUND		0x0000008000000000ULL
#define DFP_ROUND		0x0000000000000080ULL
#define GFP_ROUND		0x0000000000000400ULL
#define HFP_ROUND		0x0000000000004000ULL

#define SFP_TYPE		0
#define DFP_TYPE		1
#define GFP_TYPE		2
#define HFP_TYPE		3

// Floating Format Definitions

// F_Floating Format (Single Precision)
//
// |<--------- Low-Order --------->|<-------- High-Order --------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Fraction            |S|  Exponential  |  Fraction   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
// D_Floating Format (Double Precision)
//
// |<--------- Low-Order --------->|<-------- High-Order --------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Fraction            |S|  Exponential  |  Fraction   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define UFP_P_EXP		7
#define UFP_M_EXP		0xFF
#define UFP_BIAS		0x80
#define UFP_SIGN		FP_SIGN
#define UFP_HBIT		(1u << UFP_P_EXP)
#define UFP_GUARD		(15 - UFP_P_EXP)
#define UFP_EXP			(UFP_M_EXP << UFP_P_EXP)
#define SFP_FPREC		23
#define DFP_FPREC		55

#define UFP_GETEXP(fp)	(((fp) >> UFP_P_EXP) & UFP_M_EXP)
#define UFP_SETEXP(fp)	(((fp) & UFP_M_EXP) << UFP_P_EXP)
#define UFP_GETFRAC(fp)	FP_SWAP(((fp) & ~(UFP_SIGN|UFP_EXP)) | UFP_HBIT)
#define UFP_SETFRAC(fp)	(FP_SWAP(fp) & ~(UFP_SIGN|UFP_EXP|UFP_HBIT))

// G_Floating Format Definitions
//
// |<--------- Low-Order --------->|<-------- High-Order --------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Fraction            |S|      Exponential    |       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define GFP_P_EXP		4
#define GFP_M_EXP		0x7FF
#define GFP_BIAS		0x400
#define GFP_SIGN		FP_SIGN
#define GFP_HBIT		(1u << GFP_P_EXP)
#define GFP_GUARD		(15 - GFP_P_EXP)
#define GFP_EXP			(GFP_M_EXP << GFP_P_EXP)
#define GFP_FPREC		52

#define GFP_GETEXP(fp)	(((fp) >> GFP_P_EXP) & GFP_M_EXP)
#define GFP_SETEXP(fp)	(((fp) & GFP_M_EXP) << GFP_P_EXP)
#define GFP_GETFRAC(fp)	FP_SWAP(((fp) & ~(GFP_SIGN|GFP_EXP)) | GFP_HBIT)
#define GFP_SETFRAC(fp)	(FP_SWAP(fp) & ~(GFP_SIGN|GFP_EXP|GFP_HBIT))

// H_floating Format Definitions
//
// |<--------- Low-Order --------->|<-------- High-Order --------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Fraction            |S|        Exponential          |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define HFP_P_EXP		0
#define HFP_P_NORM		127
#define HFP_M_EXP		0x7FFF
#define HFP_BIAS		0x4000
#define HFP_SIGN		FP_SIGN
#define HFP_HBIT		(1u << HFP_P_EXP)
#define HFP_GUARD		(15 - HFP_P_EXP)
#define HFP_EXP			(HFP_M_EXP << HFP_P_EXP)

#define HFP_GETEXP(fp)	(((fp) >> HFP_P_EXP) & HFP_M_EXP)
#define HFP_SETEXP(fp)	(((fp) & HFP_M_EXP) << HFP_P_EXP)
#define HFP_GETFRAC(fp)	FP_SWAP(((fp) & ~(HFP_SIGN|HFP_EXP)) | HFP_HBIT)
#define HFP_SETFRAC(fp)	(FP_SWAP(fp) & ~(HFP_SIGN|HFP_EXP|HFP_HBIT))


// Set N and Z flag
#define fpSetNZ(cc, res, flg) \
	if ((res) & FP_SIGN)  cc = CC_N | (flg); \
	else if ((res) == 0)  cc = CC_Z | (flg); \
	else                  cc = (flg);

// VAX S/D_Floating class
class vaxfp_t {
public:
	vaxfp_t(int type = SFP_TYPE);
	~vaxfp_t();

	int unpack(uint32_t *fp);
	int unpackf(uint32_t *fp);
	int unpackd(uint32_t *fp);
	int unpackg(uint32_t *fp);
//	int unpackh(uint32_t *fp);

	int packf(uint32_t *dst);
	int packd(uint32_t *dst);
	int packg(uint32_t *dst);
//	int packh(uint32_t *dst);

	void normalize();

	void convert(int32_t val);

	static int converti(int32_t val, uint32_t *res, int type);
	static int convertfi(uint32_t *fp, int type, uint32_t *dst, int size, uint32_t *cc);
	static int convertfd(uint32_t *val, uint32_t *res);
	static int convertdf(uint32_t *val, uint32_t *res);
	static int convertfg(uint32_t *val, uint32_t *res);
	static int convertgf(uint32_t *val, uint32_t *res);

	static int compare(uint32_t *src, uint32_t *dst, int type, uint32_t *cc);

	static int add(vaxfp_t *add, vaxfp_t *adr, vaxfp_t *sum);
	static int multiply(vaxfp_t *mpy, vaxfp_t *mpr, vaxfp_t *pro);
	static int divide(vaxfp_t *dvd, vaxfp_t *dvr, vaxfp_t *quo);
	static int modulus(vaxfp_t *fp, uint32_t *res, uint32_t *ccFlag);

	static int addf(uint32_t *fp1, uint32_t *fp2, uint32_t *res);
	static int addd(uint32_t *fp1, uint32_t *fp2, uint32_t *res);
	static int addg(uint32_t *fp1, uint32_t *fp2, uint32_t *res);

	static int subtractf(uint32_t *fp1, uint32_t *fp2, uint32_t *res);
	static int subtractd(uint32_t *fp1, uint32_t *fp2, uint32_t *res);
	static int subtractg(uint32_t *fp1, uint32_t *fp2, uint32_t *res);

	static int multiplyf(uint32_t *fp1, uint32_t *fp2, uint32_t *res);
	static int multiplyd(uint32_t *fp1, uint32_t *fp2, uint32_t *res);
	static int multiplyg(uint32_t *fp1, uint32_t *fp2, uint32_t *res);

	static int dividef(uint32_t *fp1, uint32_t *fp2, uint32_t *res);
	static int divided(uint32_t *fp1, uint32_t *fp2, uint32_t *res);
	static int divideg(uint32_t *fp1, uint32_t *fp2, uint32_t *res);

	static int modulusf(uint32_t *fp1, uint32_t *fp2, uint32_t *res,
			uint32_t ext, uint32_t *val, uint32_t *ccFlag);
	static int modulusd(uint32_t *fp1, uint32_t *fp2, uint32_t *res,
			uint32_t ext, uint32_t *val, uint32_t *ccFlag);
	static int modulusg(uint32_t *fp1, uint32_t *fp2, uint32_t *res,
			uint32_t ext, uint32_t *val, uint32_t *ccFlag);

protected:
	int      type;
	int32_t	 bias, round;
	int      fprec;

	int32_t  sign;
	int32_t  exp;
	uint64_t frac;

//	const int64_t fpRound[];
//	const int32_t fpBias[];
//	const int32_t fpMask[];
};

#if 0
class sfp_t {
public:
	sfp_t(uint32_t fp);
	~sfp_t();

	int unpack(uint32_t fp);
	int pack(uint32_t *fp);
	void normalize();
	void convert(int32_t val);

protected:
	int32_t  sign;
	int32_t  exp;
	uint64_t frac;
};

class dfp_t {
public:
	dfp_t(uint32_t *fp);
	~dfp_t();

	int unpack(uint32_t *fp);
	int pack(uint32_t *fp);
	void normalize();
	void convert(int32_t val);

protected:
	int32_t  sign;
	int32_t  exp;
	uint64_t frac;
};

class gfp_t {
public:
	gfp_t(uint32_t *fp);
	~gfp_t();

	int unpack(uint32_t *fp);
	int pack(uint32_t *fp);
	void normalize();
	void convert(int32_t val);

protected:
	int32_t  sign;
	int32_t  exp;
	uint64_t frac;
};
#endif
