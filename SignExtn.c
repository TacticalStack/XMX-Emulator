/*
 - Extract and sign extend any bit for any 16-bit bit pattern
 - ECED 3403
 - 1 Jun 22
 *** Caveat Emptor - Buyer Beware ***
 *** Don't use this on faith - are you sure it works? ***
*/
#include "emulator.h"

#define NEGSIGN(x)	(0xFFFF << (x))

unsigned short sign_ext(unsigned short offset, unsigned short signbit)
{
    /* Sign extend at signbit
    - return sign-extended value
    */
    if (offset & (1 << signbit))
        offset |= NEGSIGN(signbit); /* bit set - negative */
    /* bit clear - positive */
    return offset;
}

unsigned short extract_bits(unsigned short value, unsigned short signbit)
{
    /*
    - Ignore bits to left of sign bit (that is, signbit + 1)
    - Should be a macro (probably optimized to one - why?)
    */

    return value & ~(NEGSIGN(signbit + 1));

}