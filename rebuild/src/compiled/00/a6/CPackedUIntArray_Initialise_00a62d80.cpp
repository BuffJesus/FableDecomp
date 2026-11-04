#include <new>
#include <string.h>
#include "fable_packed_uint_array.h"
void CPackedUIntArray::Initialise(void* source,unsigned long sourceBits,long stride,unsigned long count)
{
    if(sourceBits>32)sourceBits=32;
    delete[] PackedInts;
    PackedInts=0;Bits=0;Bias=0;Size=count;
    Bias=CalculateBias(source,sourceBits,stride,count);
    Bits=static_cast<unsigned char>(CountBits(source,sourceBits,stride,count));
    if(Bits>0){
        unsigned long words=(Bits*Size+31)>>5;
        PackedInts=new unsigned long[words];
        PackArray(source,sourceBits,stride,words);
    }
}
