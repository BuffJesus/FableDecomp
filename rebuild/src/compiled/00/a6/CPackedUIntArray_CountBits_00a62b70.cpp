#include <new>
#include <string.h>
#include "fable_packed_uint_array.h"
static __forceinline unsigned long PackedLog2(unsigned long value)
{
    unsigned long result;
    if(value>=0x80000000UL)result=31;
    else {
        ++value;
        result=(static_cast<long>(0x10000UL-value)>>31)&16;
        result+=(static_cast<long>((1UL<<(result+8))-value)>>31)&8;
        result+=(static_cast<long>((1UL<<(result+4))-value)>>31)&4;
        result+=(static_cast<long>((1UL<<(result+2))-value)>>31)&2;
        return result+((static_cast<long>((1UL<<(result+1))-value)>>31)&1);
    }
    return result;
}
unsigned long CPackedUIntArray::CountBits(void* source,unsigned long sourceBits,long stride,unsigned long count)
{
    unsigned char* data=static_cast<unsigned char*>(source);
    unsigned long value=0;
    unsigned long bytes=(sourceBits+7)>>3;
    unsigned long combined=0;
    unsigned long mask=g_PackedUIntMasks_013979E8[sourceBits];
    for(unsigned long i=0;i<count;++i){
        if(bytes==4)value=*reinterpret_cast<unsigned long*>(data);
        else if(bytes>0)memcpy(&value,data,bytes);
        value=(mask&value)-Bias;
        combined|=value;
        data+=stride;
    }
    if(combined)return PackedLog2(combined)+1;
    return 0;
}
