#include "emu/emucore.h"
#include "formats/dec/word10.h"


// CORE-DUMP Format
//
// 00 01 02 03 04 05 06|07
// 08 09 10 11 12 13|14 15
// 16 17 18 19 20|21 22 23
// 24 25 26 27|28 29 30 31
// __ __ __ __|32 33 34 35

uint36_t dec_img_decodeCoreDump(uint8_t *data8)
{
	uint36_t data36;

	data36  = (uint36_t)(data8[0] & 0xFF) << 29;
	data36 |= (uint36_t)(data8[1] & 0xFF) << 22;
	data36 |= (uint36_t)(data8[2] & 0xFF) << 15;
	data36 |= (uint36_t)(data8[3] & 0xFF) << 8;
	data36 |= (uint36_t)(data8[4] & 0x0F) << 1;

	return data36;
}


// ANSI Format
//
// __|00 01 02 03 04 05 06
// __|07 08 09 10 11 12 13
// __|14 15 16 17 18 19 20
// __|21 22 23 24 25 26 27
// 35|28 29 30 31 32 33 34

uint36_t dec_img_decodeANSI(uint8_t *data8)
{
	uint36_t data36;

	data36  = (uint36_t)(data8[0] & 0x7F) << 29;
	data36 |= (uint36_t)(data8[1] & 0x7F) << 22;
	data36 |= (uint36_t)(data8[2] & 0x7F) << 15;
	data36 |= (uint36_t)(data8[3] & 0x7F) << 8;
	data36 |= (uint36_t)(data8[4] & 0x7F) << 1;
	if (data8[4] & 0x80)
		data36 |= 1;

	return data36;
}

// FTP High Density Format
//
// 00 01 02 03 04 05 06 07
// 08 09 10 11 12 13 14 15
// 16 17 18 19 20 21 22 23
// 24 25 26 27 28 29 30 31
// 32 33 34 35|00 01 02 03
// 04 05 06 07 08 09 10 11
// 12 13 14 15 16 17 18 19
// 20 21 22 23 24 25 26 27
// 28 29 30 31 32 33 34 35
