
#include <stdio.h>
#include <stdbool.h>

#include "IQmathLib.h"
#include "cubic_root.h"


// lookup table for cube root of (idx*16)
_iq24 cube_table[] = {
    _IQ24(0.00000), // unreachable
    _IQ24(2.51984), // unreachable
    _IQ24(3.17480),
    _IQ24(3.63424),
    _IQ24(4.00000),
    _IQ24(4.30887),
    _IQ24(4.57886),
    _IQ24(4.82028),
    _IQ24(5.03968),
    _IQ24(5.24148),
    _IQ24(5.42884),
    _IQ24(5.60408),
    _IQ24(5.76900),
    _IQ24(5.92499),
    _IQ24(6.07318),
    _IQ24(6.21447),
    _IQ24(6.34960)};

_iq24 cube_root_fast(_iq24 num)
{
    bool neg_num = false;
    if (num < 0)
    {
        neg_num = true;
        num = -num;
    }
    // Scale the number so that at least one bit from the last three bits is a one.
    unsigned long scaled_num = num;
    char scaling_3bit_shitf = 0;
    if (!(scaled_num & 0xFFFFFF00))
    {
        scaled_num <<= 24;
        scaling_3bit_shitf += 8;
    }
    if (!(scaled_num & 0xFFF00000))
    {
        scaled_num <<= 12;
        scaling_3bit_shitf += 4;
    }
    if (!(scaled_num & 0xFC000000))
    {
        scaled_num <<= 6;
        scaling_3bit_shitf += 2;
    }
    if (!(scaled_num & 0xE0000000))
    {
        scaled_num <<= 3;
        scaling_3bit_shitf += 1;
    }

    // use last 4 bits (MSBs) to find value from table
    char idx = scaled_num >> (24 + 4);

    _iq24 alpha = _IQ24div((scaled_num & 0x0FFFFFFF), 0x10000000); // interpolation alpha
    _iq24 x = (cube_table[idx]+_IQ24mpy(cube_table[idx+1]-cube_table[idx],alpha)) >> scaling_3bit_shitf;

    // _iq24 x = cube_table[idx] >> scaling_3bit_shitf;
    x = (2*x + _IQ24div(_IQ24div(num, x), x)) / 3;

    if (neg_num)
        return -x;
    else
        return x;
}




// int main()
// {
//     printf("Hello World %f", _IQ24toF(cube_root_fast(_IQ24(88))));
//     return 0;
// }