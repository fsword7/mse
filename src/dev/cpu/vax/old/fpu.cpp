/*
 * fpu.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: Tim Stark
 *
 *  VAX floating points
 *
 */

#include "emu/emucore.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/fpu.h"

static const int64_t fpRound[] = { SFP_ROUND, DFP_ROUND, GFP_ROUND, HFP_ROUND };
static const int32_t fpBias[]  = { UFP_BIAS,  UFP_BIAS,  GFP_BIAS,  HFP_BIAS  };
static const int32_t fpMask[]  = { 0x7F, 0x7FFF, 0x7FFFFFFF, 0x7FFFFFFF };

vaxfp_t::vaxfp_t(int _type)
: type(_type)
{
	bias  = fpBias[type];
	round = fpRound[type];
	sign  = 0;
	exp   = 0;
	frac  = 0;
//	st    = VFP_OK;
}

vaxfp_t::~vaxfp_t()
{
}

int vaxfp_t::unpack(uint32_t *val)
{
	switch(type) {
	case SFP_TYPE:
		return unpackf(val);
	case DFP_TYPE:
		return unpackd(val);
	case GFP_TYPE:
		return unpackg(val);
	}

	return VFP_FAULT;
}

int vaxfp_t::unpackf(uint32_t *val)
{
	sign  = *val & UFP_SIGN;
	exp   = UFP_GETEXP(*val);
	fprec = SFP_FPREC;
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = ZXTQ(UFP_GETFRAC(*val)) << (UFP_GUARD + 32);
	}

	return VFP_OK;
}

int vaxfp_t::unpackd(uint32_t *val)
{
	sign  = val[0] & UFP_SIGN;
	exp   = UFP_GETEXP(val[0]);
	fprec = DFP_FPREC;
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = ((ZXTQ(UFP_GETFRAC(val[0])) << 32) | ZXTQ(FP_SWAP(val[1]))) << UFP_GUARD;
	}

	return VFP_OK;
}

int vaxfp_t::unpackg(uint32_t *val)
{
	sign  = val[0] & UFP_SIGN;
	exp   = GFP_GETEXP(val[0]);
	fprec = GFP_FPREC;
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = ((ZXTQ(GFP_GETFRAC(val[0])) << 32) | ZXTQ(FP_SWAP(val[1]))) << GFP_GUARD;
	}

	return VFP_OK;
}


int vaxfp_t::packf(uint32_t *dst)
{
	uint64_t tfp;

	// If fraction is zero, return as zero.
	if (frac == 0) {
		*dst = 0;
		return VFP_OK;
	}

	// Round and normalize value.
	frac += SFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	// Check exponential field for overflowing or underflowing.
	if (exp > UFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		*dst = 0;
		return VFP_UNFL;
	}

	// Now packing sign, exponential and fraction together.
	tfp  = frac >> (UFP_GUARD + 32);
	*dst = sign | UFP_SETEXP(exp) | UFP_SETFRAC(tfp);
	return VFP_OK;
}

int vaxfp_t::packd(uint32_t *dst)
{
	uint64_t tfp;

	// If fraction is zero, return as zero.
	if (frac == 0) {
		dst[0] = dst[1] = 0;
		return VFP_OK;
	}

	// Round and normalize value.
	frac += DFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	// Check exponential field for overflowing or underflowing.
	if (exp > UFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		dst[0] = dst[1] = 0;
		return VFP_UNFL;
	}

	// Now packing sign, exponential and fraction together.
	tfp    = frac >> UFP_GUARD;
	dst[0] = sign | UFP_SETEXP(exp) | UFP_SETFRAC(tfp >> 32);
	dst[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}

int vaxfp_t::packg(uint32_t *dst)
{
	uint64_t tfp;

	// If fraction is zero, return as zero.
	if (frac == 0) {
		dst[0] = dst[1] = 0;
		return VFP_OK;
	}

	// Round and normalize value.
	frac += GFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	// Check exponential field for overflowing or underflowing.
	if (exp > GFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		dst[0] = dst[1] = 0;
		return VFP_UNFL;
	}

	// Now packing sign, exponential and fraction together.
	tfp    = frac >> GFP_GUARD;
	dst[0] = sign | GFP_SETEXP(exp) | GFP_SETFRAC(tfp >> 32);
	dst[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}

#if 0
int vaxfp_t::packh(uint32_t *dst)
{
	uint64_t tfp;

	// If fraction is zero, return as zero.
	if (frac == 0) {
		dst[0] = dst[1] = dst[2] = dst[3] = 0;
		return VFP_OK;
	}

	// Round and normalize value.
	frac += HFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	// Check exponential field for overflowing or underflowing.
	if (exp > HFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		dst[0] = dst[1] = dst[2] = dst[3] = 0;
		return VFP_UNFL;
	}

	// Now packing sign, exponential and fraction together.
	tfp    = frac >> HFP_GUARD;
	dst[0] = sign | HFP_SETEXP(exp) | HFP_SETFRAC(tfp >> 32);
	dst[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}
#endif

void vaxfp_t::normalize()
{
	if (frac == 0) {
		sign = 0;
		exp = 0;
	} else {
		// Shift fraction to left-justified.
		while ((frac & FP_NORM) == 0) {
			frac <<= 1;
			exp--;
		}
	}
}

void vaxfp_t::convert(int32_t val)
{
	if (val == 0) {
		sign = 0;
		exp  = 0;
		frac = 0;
	} else if (val < 0) {
		sign = FP_SIGN;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(val) << (FP_P_NORM - 31);
	}
	normalize();
}

// Convert integer (B/W/L) to F/D/G float
int vaxfp_t::converti(int32_t val, uint32_t *res, int type)
{
	vaxfp_t fp(type);

	// When determine value as zero, do nothing and return with zero
	if (val == 0) {
		res[0] = 0;
		if (type > SFP_TYPE)
			res[1] = 0;
		return VFP_OK;
	}

	// Determine value as positive or negative
	if (val < 0) {
		// Negative value
		fp.sign = FP_SIGN;
		fp.exp  = fpBias[type] + 32;
		fp.frac = uint64_t(-val) << (FP_P_NORM - 31);
	} else {
		// Positive value
		fp.sign = 0;
		fp.exp  = fpBias[type] + 32;
		fp.frac = uint64_t(val) << (FP_P_NORM - 31);
	}
	fp.normalize();

	// All done, pack into FP words
	switch (type) {
	case SFP_TYPE:
		return fp.packf(res);
	case DFP_TYPE:
		return fp.packd(res);
	case GFP_TYPE:
		return fp.packg(res);
	}
	return VFP_ERROR;
}

int vaxfp_t::convertfi(uint32_t *src, int type, uint32_t *dst, int size, uint32_t *ccFlag)
{
	vaxfp_t fp(type);
	int     sts, ubexp;

	// clear code condition flags
	*ccFlag = 0;

	// Unpack FP value
	switch (type) {
	case SFP_TYPE:
		if ((sts = fp.unpackf(src)) != VFP_OK)
			return sts;
		break;
	case DFP_TYPE:
		if ((sts = fp.unpackd(src)) != VFP_OK)
			return sts;
		break;
	case GFP_TYPE:
		if ((sts = fp.unpackg(src)) != VFP_OK)
			return sts;
		break;
	}

	ubexp = fp.exp - fp.bias;
	if ((fp.exp == 0) || (ubexp < 0)) {
		*dst = 0;
		return VFP_OK;
	}

	if (ubexp <= FP_P_NORM) {
		fp.frac >>= (FP_P_NORM - ubexp);
		if (size == VFP_RLONG)
			fp.frac++;
		fp.frac >>= 1;

		// Check fraction for maximum value for overflow flag
		if (fp.frac > (ZXTQ(fpMask[size]) + (fp.sign != 0)))
			*ccFlag = CC_V;
	} else {
		*ccFlag = CC_V;
		if (ubexp > (FP_P_NORM + 32)) {
			*dst = 0;
			return VFP_OK;
		}
		fp.frac <<= (ubexp - FP_P_NORM - 1);
	}

	*dst = fp.sign ? -uint32_t(fp.frac) : uint32_t(fp.frac);
	return VFP_OK;
}


int vaxfp_t::convertfd(uint32_t *src, uint32_t *dst)
{
	vaxfp_t fp(SFP_TYPE);
	int     sts;

	if ((sts = fp.unpackf(src)) != VFP_OK)
		return sts;
	fp.type = DFP_TYPE;

	return fp.packd(dst);
}

int vaxfp_t::convertdf(uint32_t *src, uint32_t *dst)
{
	vaxfp_t fp(DFP_TYPE);
	int     sts;

	if ((sts = fp.unpackd(src)) != VFP_OK)
		return sts;
	fp.type = SFP_TYPE;

	return fp.packf(dst);
}

int vaxfp_t::convertfg(uint32_t *src, uint32_t *dst)
{
	vaxfp_t fp(SFP_TYPE);
	int     sts;

	if ((sts = fp.unpackf(src)) != VFP_OK)
		return sts;
	fp.exp  = fp.exp - UFP_BIAS + GFP_BIAS;
	fp.type = GFP_TYPE;

	return fp.packg(dst);
}

int vaxfp_t::convertgf(uint32_t *src, uint32_t *dst)
{
	vaxfp_t fp(GFP_TYPE);
	int     sts;

	if ((sts = fp.unpackg(src)) != VFP_OK)
		return sts;
	fp.exp  = fp.exp - GFP_BIAS + UFP_BIAS;
	fp.type = SFP_TYPE;

	return fp.packf(dst);
}


int vaxfp_t::compare(uint32_t *src, uint32_t *dst, int type, uint32_t *cc)
{
	vaxfp_t sfp(type), dfp(type);
	int     sts;
	int32_t diff;

	switch (type) {
	case SFP_TYPE:
		if ((sts = sfp.unpackf(src)) != VFP_OK)
			return sts;
		if ((sts = dfp.unpackf(dst)) != VFP_OK)
			return sts;
		break;
	case DFP_TYPE:
		if ((sts = sfp.unpackd(src)) != VFP_OK)
			return sts;
		if ((sts = dfp.unpackd(dst)) != VFP_OK)
			return sts;
		break;
	case GFP_TYPE:
		if ((sts = sfp.unpackg(src)) != VFP_OK)
			return sts;
		if ((sts = dfp.unpackg(dst)) != VFP_OK)
			return sts;
		break;
	}

	if (sfp.sign != dfp.sign) {
		*cc = sfp.sign ? CC_N : 0;
	} else {
		// Determine difference between two FP numbers (exponent and fraction fields)
		if ((diff = sfp.exp - dfp.exp) == 0)
			diff = ((sfp.frac < dfp.frac) ? -1 :
					(sfp.frac > dfp.frac) ?  1 : 0);

		// Update code condition as result
		if (diff < 0)
			*cc = sfp.sign ? 0 : CC_N;
		else if (diff > 0)
			*cc = sfp.sign ? CC_N : 0;
		else
			*cc = CC_Z;
	}

	return VFP_OK;
}

int vaxfp_t::add(vaxfp_t *src, vaxfp_t *dst, vaxfp_t *res)
{
	vaxfp_t *add, *adr;
	int      dexp;
	uint64_t addf;

	// When one of FP numbers contain zero fraction,
	// return other of FP numbers as result.
	if (src->frac == 0) {
		*res = *dst;
		return VFP_OK;
	}
	if (dst->frac == 0) {
		*res = *src;
		return VFP_OK;
	}

	if ((src->exp < dst->exp) || ((src->exp == dst->exp) && (src->frac < dst->frac))) {
		add = dst;
		adr = src;
	} else {
		add = src;
		adr = dst;
	}

	dexp = add->exp - adr->exp;
	if (res != src) {
		res->sign = add->sign;
		res->exp  = add->exp;
	}

	if (add->sign ^ adr->sign) {
		if (dexp != 0) {
			addf = (dexp < 64)
				? ((-adr->frac >> dexp) | (FP_MASK64 << (64 - dexp)))
				: FP_MASK64;
			res->frac = add->frac + addf;
		} else
			res->frac = add->frac - adr->frac;
		res->normalize();
	} else {
		addf = (dexp < 64) ? (adr->frac >> dexp) : 0;

		// Add and normalize it
		res->frac = add->frac + addf;
//		printf("ADD: %016llX %016llX => %016llX\n", add->frac, addf, res->frac);
		if (res->frac < addf) {
			res->frac = FP_NORM | (res->frac >> 1);
			res->exp++;
		}
	}

	return VFP_OK;
}

int vaxfp_t::addf(uint32_t *fp1, uint32_t *fp2, uint32_t *res)
{
	vaxfp_t sfp(SFP_TYPE), dfp(SFP_TYPE), rfp(SFP_TYPE);
	int     sts;

	if ((sts = sfp.unpackf(fp1)) != VFP_OK)
		return sts;
	if ((sts = dfp.unpackf(fp2)) != VFP_OK)
		return sts;

	add(&sfp, &dfp, &rfp);

	return rfp.packf(res);
}

int vaxfp_t::addd(uint32_t *fp1, uint32_t *fp2, uint32_t *res)
{
	vaxfp_t sfp(DFP_TYPE), dfp(DFP_TYPE), rfp(DFP_TYPE);
	int     sts;

	if ((sts = sfp.unpackd(fp1)) != VFP_OK)
		return sts;
	if ((sts = dfp.unpackd(fp2)) != VFP_OK)
		return sts;

	add(&sfp, &dfp, &rfp);

	return rfp.packd(res);
}

int vaxfp_t::addg(uint32_t *fp1, uint32_t *fp2, uint32_t *res)
{
	vaxfp_t sfp(GFP_TYPE), dfp(GFP_TYPE), rfp(GFP_TYPE);
	int     sts;

	if ((sts = sfp.unpackg(fp1)) != VFP_OK)
		return sts;
	if ((sts = dfp.unpackg(fp2)) != VFP_OK)
		return sts;

	add(&sfp, &dfp, &rfp);

	return rfp.packg(res);
}


int vaxfp_t::subtractf(uint32_t *fp1, uint32_t *fp2, uint32_t *res)
{
	vaxfp_t sfp(SFP_TYPE), dfp(SFP_TYPE), rfp(SFP_TYPE);
	int     sts;

	if ((sts = sfp.unpackf(fp1)) != VFP_OK)
		return sts;
	if ((sts = dfp.unpackf(fp2)) != VFP_OK)
		return sts;

	sfp.sign ^= FP_SIGN;
	add(&sfp, &dfp, &rfp);

	return rfp.packf(res);
}

int vaxfp_t::subtractd(uint32_t *fp1, uint32_t *fp2, uint32_t *res)
{
	vaxfp_t sfp(DFP_TYPE), dfp(DFP_TYPE), rfp(DFP_TYPE);
	int     sts;

	if ((sts = sfp.unpackd(fp1)) != VFP_OK)
		return sts;
	if ((sts = dfp.unpackd(fp2)) != VFP_OK)
		return sts;

	sfp.sign ^= FP_SIGN;
	add(&sfp, &dfp, &rfp);

	return rfp.packd(res);
}

int vaxfp_t::subtractg(uint32_t *fp1, uint32_t *fp2, uint32_t *res)
{
	vaxfp_t sfp(GFP_TYPE), dfp(GFP_TYPE), rfp(GFP_TYPE);
	int     sts;

	if ((sts = sfp.unpackg(fp1)) != VFP_OK)
		return sts;
	if ((sts = dfp.unpackg(fp2)) != VFP_OK)
		return sts;

	sfp.sign ^= FP_SIGN;
	add(&sfp, &dfp, &rfp);

	return rfp.packg(res);
}


int vaxfp_t::multiply(vaxfp_t *mpy, vaxfp_t *mpr, vaxfp_t *pro)
{
	uint64_t rhi, rlo, rm1, rm2, ahi, alo, bhi, blo;

	// Determine one of FP numbers as zero and return as zero
	if (mpy->exp == 0 || mpr->exp == 0) {
		pro->sign = 0;
		pro->exp  = 0;
		pro->frac = 0;
		return VFP_OK;
	}

	// For single precision (F floating), do 32/64-bit multiply
	ahi = mpy->frac >> 32;
	bhi = mpr->frac >> 32;
	rhi = ahi * bhi;

	// For double precision (D/G floating), do 64/128-bit multiply
	if (mpy->fprec > 32) {
		alo  = ZXTL(mpy->frac);
		blo  = ZXTL(mpr->frac);
		rlo  = alo * blo;
		rm1  = ahi * blo;
		rm2  = alo * bhi;
		rhi += (rm1 >> 32) + (rm2 >> 32);
		rm1  = rlo + (rm1 << 32);
		rm2  = rm1 + (rm2 << 32);
		if (rm1 < rlo)
			rhi++;
		if (rm2 < rm1)
			rhi++;
	}

	pro->sign = mpy->sign ^ mpr->sign;
	pro->exp  = mpy->exp + mpr->exp - mpy->bias;
	pro->frac = rhi;
	pro->normalize();

	return VFP_OK;
}

int vaxfp_t::divide(vaxfp_t *dvd, vaxfp_t *dvr, vaxfp_t *quo)
{
	uint64_t fdvd, fdvr, fquo;
	int      idx, prec;

	if (dvr->exp == 0)
		return VFP_DZRO;
	if (dvd->exp == 0) {
		if (dvd != quo)
			*quo = *dvd;
		return VFP_OK;
	}

	prec = dvd->fprec + 3;
	fdvd = dvd->frac >> 1;
	fdvr = dvr->frac >> 1;
	fquo = 0;
	for (idx = 0; (idx < prec) && (fdvd != 0); idx++) {
		fquo <<= 1;
		if (fdvd >= fdvr) {
			fdvd -= fdvr;
			fquo++;
		}
		fdvd <<= 1;
	}

	quo->sign = dvd->sign ^ dvr->sign;
	quo->exp  = dvd->exp - dvr->exp + dvd->bias + 1;
	quo->frac = fquo << (FP_P_NORM - idx + 1);
	quo->normalize();

	return VFP_OK;
}

int vaxfp_t::modulus(vaxfp_t *fp, uint32_t *res, uint32_t *ccFlag)
{
	int sc;

	*ccFlag = 0;
	if (fp->exp <= fp->bias) {
		*res = 0;
	} else if (fp->exp > (fp->bias + 64)) {
		fp->sign = 0;
		fp->exp  = 0;
		fp->frac = 0;

		*res = 0;
		*ccFlag = CC_V;
	} else {
		sc   = fp->exp - fp->bias;
		*res = fp->frac >> (64 - sc);

		if ((fp->exp > (fp->bias + 32)) || ((fp->exp == (fp->bias + 32)) &&
			(uint32_t(*res) > ((fp->sign == FP_SIGN) ? 0x8000000 : 0x7FFFFFF))))
			*ccFlag = CC_V;
		if (fp->sign == FP_SIGN)
			*res = -*res;

		fp->frac = (sc < 64) ? (fp->frac << sc) : 0;
		fp->exp  = fp->bias;
	}

	fp->normalize();

	return VFP_OK;
}

int vaxfp_t::multiplyf(uint32_t *fp1, uint32_t *fp2, uint32_t *res)
{
	vaxfp_t sfp(SFP_TYPE), dfp(SFP_TYPE), rfp(SFP_TYPE);
	int     sts;

	if ((sts = sfp.unpackf(fp1)) != VFP_OK)
		return sts;
	if ((sts = dfp.unpackf(fp2)) != VFP_OK)
		return sts;

	multiply(&sfp, &dfp, &rfp);

	return rfp.packf(res);
}

int vaxfp_t::multiplyd(uint32_t *fp1, uint32_t *fp2, uint32_t *res)
{
	vaxfp_t sfp(DFP_TYPE), dfp(DFP_TYPE), rfp(DFP_TYPE);
	int     sts;

	if ((sts = sfp.unpackd(fp1)) != VFP_OK)
		return sts;
	if ((sts = dfp.unpackd(fp2)) != VFP_OK)
		return sts;

	multiply(&sfp, &dfp, &rfp);

	return rfp.packd(res);
}

int vaxfp_t::multiplyg(uint32_t *fp1, uint32_t *fp2, uint32_t *res)
{
	vaxfp_t sfp(GFP_TYPE), dfp(GFP_TYPE), rfp(GFP_TYPE);
	int     sts;

	if ((sts = sfp.unpackg(fp1)) != VFP_OK)
		return sts;
	if ((sts = dfp.unpackg(fp2)) != VFP_OK)
		return sts;

	multiply(&sfp, &dfp, &rfp);

	return rfp.packg(res);
}


int vaxfp_t::dividef(uint32_t *src, uint32_t *dst, uint32_t *res)
{
	vaxfp_t dvd(SFP_TYPE), dvr(SFP_TYPE), quo(SFP_TYPE);
	int     sts;

	if ((sts = dvr.unpackf(src)) != VFP_OK)
		return sts;
	if ((sts = dvd.unpackf(dst)) != VFP_OK)
		return sts;

	if ((sts = divide(&dvd, &dvr, &quo)) != VFP_OK)
		return sts;

	return quo.packf(res);
}

int vaxfp_t::divided(uint32_t *src, uint32_t *dst, uint32_t *res)
{
	vaxfp_t dvd(DFP_TYPE), dvr(DFP_TYPE), quo(DFP_TYPE);
	int     sts;

	if ((sts = dvr.unpackd(src)) != VFP_OK)
		return sts;
	if ((sts = dvd.unpackd(dst)) != VFP_OK)
		return sts;

	if ((sts = divide(&dvd, &dvr, &quo)) != VFP_OK)
		return sts;

	return quo.packd(res);
}

int vaxfp_t::divideg(uint32_t *src, uint32_t *dst, uint32_t *res)
{
	vaxfp_t dvd(GFP_TYPE), dvr(GFP_TYPE), quo(GFP_TYPE);
	int     sts;

	if ((sts = dvr.unpackg(src)) != VFP_OK)
		return sts;
	if ((sts = dvd.unpackg(dst)) != VFP_OK)
		return sts;

	if ((sts = divide(&dvd, &dvr, &quo)) != VFP_OK)
		return sts;

	return quo.packg(res);
}


int vaxfp_t::modulusf(uint32_t *fp1, uint32_t *fp2, uint32_t *res,
		uint32_t ext, uint32_t *val, uint32_t *ccFlag)
{
	vaxfp_t mpc(SFP_TYPE), mpr(SFP_TYPE), pro(SFP_TYPE);
	int     sts;

	if ((sts = mpc.unpackf(fp1)) != VFP_OK)
		return sts;
	if ((sts = mpr.unpackf(fp2)) != VFP_OK)
		return sts;

	mpc.frac |= uint64_t(ext) << 32;
	if ((sts = multiply(&mpc, &mpr, &pro)) != VFP_OK)
		return sts;
	modulus(&pro, val, ccFlag);

	return pro.packf(res);
}

int vaxfp_t::modulusd(uint32_t *fp1, uint32_t *fp2, uint32_t *res,
		uint32_t ext, uint32_t *val, uint32_t *ccFlag)
{
	vaxfp_t mpc(DFP_TYPE), mpr(DFP_TYPE), pro(DFP_TYPE);
	int     sts;

	if ((sts = mpc.unpackd(fp1)) != VFP_OK)
		return sts;
	if ((sts = mpr.unpackd(fp2)) != VFP_OK)
		return sts;

	mpc.frac |= ext;
	if ((sts = multiply(&mpc, &mpr, &pro)) != VFP_OK)
		return sts;
	modulus(&pro, val, ccFlag);

	return pro.packd(res);
}

int vaxfp_t::modulusg(uint32_t *fp1, uint32_t *fp2, uint32_t *res,
		uint32_t ext, uint32_t *val, uint32_t *ccFlag)
{
	vaxfp_t mpc(GFP_TYPE), mpr(GFP_TYPE), pro(GFP_TYPE);
	int     sts;

	if ((sts = mpc.unpackg(fp1)) != VFP_OK)
		return sts;
	if ((sts = mpr.unpackg(fp2)) != VFP_OK)
		return sts;

	mpc.frac |= (ext >> 5);
	if ((sts = multiply(&mpc, &mpr, &pro)) != VFP_OK)
		return sts;
	modulus(&pro, val, ccFlag);

	return pro.packg(res);
}

#if 0
// ****************************************************************************

sfp_t::sfp_t(uint32_t fp)
{
	unpack(fp);
}

sfp_t::~sfp_t()
{
}

int sfp_t::unpack(uint32_t fp)
{
	sign = fp & UFP_SIGN;
	exp  = UFP_GETEXP(fp);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = ZXTQ(UFP_GETFRAC(fp)) << (UFP_GUARD + 32);
	}
	return VFP_OK;
}

int sfp_t::pack(uint32_t *fp)
{
	uint64_t tfp;

	if (frac == 0) {
		*fp = 0;
		return VFP_OK;
	}

	frac += SFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	if (exp > UFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		*fp = 0;
		return VFP_UNFL;
	}

	tfp = frac >> (UFP_GUARD + 32);
	*fp = sign | UFP_SETEXP(exp) | UFP_SETFRAC(tfp);
	return VFP_OK;
}

void sfp_t::normalize()
{
	if (frac == 0) {
		sign = 0;
		exp = 0;
	} else while ((frac & FP_NORM) == 0) {
		frac <<= 1;
		exp--;
	}
}

void sfp_t::convert(int32_t val)
{
	if (val == 0) {
		sign = 0;
		exp  = 0;
		frac = 0;
	} else if (val < 0) {
		sign = FP_SIGN;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(val) << (FP_P_NORM - 31);
	}
	normalize();
}

// ********************************************************************************

dfp_t::dfp_t(uint32_t *fp)
{
	unpack(fp);
}

dfp_t::~dfp_t()
{
}

int dfp_t::unpack(uint32_t *fp)
{
	sign = fp[0] & UFP_SIGN;
	exp  = UFP_GETEXP(fp[0]);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = (ZXTQ(UFP_GETFRAC(fp[0])) << 32) | (FP_SWAP(fp[1]) << UFP_GUARD);
	}
	return VFP_OK;
}

int dfp_t::pack(uint32_t *fp)
{
	uint64_t tfp;

	if (frac == 0) {
		fp[0] = 0;
		fp[1] = 0;
		return VFP_OK;
	}

	frac += DFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	if (exp > UFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		fp[0] = 0;
		fp[1] = 0;
		return VFP_UNFL;
	}

	tfp   = frac >> UFP_GUARD;
	fp[0] = sign | UFP_SETEXP(exp) | UFP_SETFRAC(tfp >> 32);
	fp[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}

void dfp_t::normalize()
{
	if (frac == 0) {
		sign = 0;
		exp = 0;
	} else while ((frac & FP_NORM) == 0) {
		frac <<= 1;
		exp--;
	}
}

void dfp_t::convert(int32_t val)
{
	if (val == 0) {
		sign = 0;
		exp  = 0;
		frac = 0;
	} else if (val < 0) {
		sign = FP_SIGN;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(val) << (FP_P_NORM - 31);
	}
	normalize();
}

// ********************************************************************************

gfp_t::gfp_t(uint32_t *fp)
{
	unpack(fp);
}

gfp_t::~gfp_t()
{
}

int gfp_t::unpack(uint32_t *fp)
{
	sign = fp[0] & GFP_SIGN;
	exp  = GFP_GETEXP(fp[0]);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = (ZXTQ(GFP_GETFRAC(fp[0])) << 32) | (FP_SWAP(fp[1]) << GFP_GUARD);
	}
	return VFP_OK;
}

int gfp_t::pack(uint32_t *fp)
{
	uint64_t tfp;

	if (frac == 0) {
		fp[0] = 0;
		fp[1] = 0;
		return VFP_OK;
	}

	frac += GFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	if (exp > GFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		fp[0] = 0;
		fp[1] = 0;
		return VFP_UNFL;
	}

	tfp   = frac >> GFP_GUARD;
	fp[0] = sign | GFP_SETEXP(exp) | GFP_SETFRAC(tfp >> 32);
	fp[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}

void gfp_t::normalize()
{
	if (frac == 0) {
		sign = 0;
		exp = 0;
	} else while ((frac & FP_NORM) == 0) {
		frac <<= 1;
		exp--;
	}
}

void gfp_t::convert(int32_t val)
{
	if (val == 0) {
		sign = 0;
		exp  = 0;
		frac = 0;
	} else if (val < 0) {
		sign = FP_SIGN;
		exp  = GFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = GFP_BIAS + 32;
		frac = ZXTQ(val) << (FP_P_NORM - 31);
	}
	normalize();
}
#endif
