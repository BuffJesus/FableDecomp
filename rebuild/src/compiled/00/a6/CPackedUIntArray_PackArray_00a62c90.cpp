#include <new>
#include <string.h>
#include "fable_packed_uint_array.h"
void CPackedUIntArray::PackArray(void* source,unsigned long sourceBits,long stride,unsigned long wordCount)
{
    unsigned long word=0;
    unsigned long offset=0;
    unsigned long output=0;
    unsigned char* data=static_cast<unsigned char*>(source);
    unsigned long bytes=(sourceBits+7)>>3;
    unsigned long value=0;
    unsigned long mask=g_PackedUIntMasks_013979E8[sourceBits];
    for(unsigned long i=0;i<Size;++i){
        if(bytes==4)value=*reinterpret_cast<unsigned long*>(data);
        else if(bytes>0)memcpy(&value,data,bytes);
        value=(mask&value)-Bias;
        data+=stride;
        word|=value<<offset;
        offset+=Bits;
        if(offset>=32){
            PackedInts[output++]=word;
            // Preserve the retail x86 shift-count behavior, including width32.
            word=value>>(32+Bits-offset);
            offset-=32;
        }
    }
    if(offset)PackedInts[output]=word;
}
