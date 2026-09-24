#include "fable_ui_strings.h"
#include <string.h>
void __fastcall FableUiReserveStringBytes(CCharStringData* data,void*,unsigned length)
{
    unsigned capacity=(length+4u)&~3u;
    if(capacity<=(data->unknown08&0x7fffffffu)) return;
    if(data->flags0C&1)
    {
        // Retail rounds by floor(log2(2*capacity-1)), including 32-bit wrap.
        unsigned doubled=capacity*2u-1u, shift=0;
        if(!doubled) shift=31;
        else { while(doubled>>1) { doubled>>=1; ++shift; } }
        capacity=1u<<shift;
    }
    char* buffer=static_cast<char*>(FableUiAllocateStringBuffer(capacity));
    unsigned used=data->unknown04+1u;
    memset(buffer+used,0,((used+3u)&~3u)-used);
    memcpy(buffer,data->text,data->unknown04);
    buffer[data->unknown04]=0;
    if(data->text) { FableUiFreeStringBuffer(data->text); data->text=0; }
    data->text=buffer;
    data->unknown08=(data->unknown08&0x80000000u)|(capacity&0x7fffffffu);
}
