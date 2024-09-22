/*
    This is not the real "IQmathlib.h" provided by texas instruments
    I just wrote this to get ride of code-composer for testings my functions.
    
*/

#include <stdlib.h>
#include <math.h>

typedef    long _iq10;
typedef    long _iq24;
typedef    long _iq;

#define    M 16777216.0L
#define    M2 1024.0L
#define    _IQ24toF(A) (float)((A) / M)

#define    _IQ24(A) (long)((A) * M)
#define    _IQ(A) _IQ24(A)

#define    _IQ24div(A, B) (long)(((float)(A)) / ((float)(B)) * M)
#define    _IQ10div(A, B) (long)(((float)(A)) / ((float)(B)) * M2)
#define    _IQdiv(A, B) _IQ24div(A, B)

#define    _IQ24mpy(A, B) (long)(((A) / M) * (B))
#define    _IQmpy(A, B) _IQ24mpy(A, B)

#define    _IQabs(A) labs(A)
#define    _IQsqrt(A) (long)((double)sqrt((double)A) * 4096)

bool is_close(_iq24 a, _iq24 b);