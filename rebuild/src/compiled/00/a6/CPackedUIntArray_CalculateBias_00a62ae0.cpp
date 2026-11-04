#include <new>
#include <string.h>
#include "fable_packed_uint_array.h"
unsigned long CPackedUIntArray::CalculateBias(void* source,unsigned long sourceBits,long stride,unsigned long count)
{
    unsigned char* data=static_cast<unsigned char*>(source);
    unsigned long bytes=(sourceBits+7)>>3;
    unsigned long result=count?0xffffffffUL:0;
    unsigned long value=0;
    unsigned long mask=g_PackedUIntMasks_013979E8[sourceBits];
    for(unsigned long i=0;i<count;++i){
        if(bytes==4)value=*reinterpret_cast<unsigned long*>(data);
        else if(bytes>0)memcpy(&value,data,bytes);
        value&=mask;
        if(result>value)result=value;
        data+=stride;
    }
    return result;
}
