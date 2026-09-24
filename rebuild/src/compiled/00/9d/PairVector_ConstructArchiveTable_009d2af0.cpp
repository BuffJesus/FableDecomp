#include "fable_ui_bank_decode.h"
FableUiGraphicArray* __fastcall FableUiConstructArchivePairs(FableUiGraphicArray* value,void*,unsigned count)
{
    value->Begin=value->End=value->Capacity=0;
    unsigned pointer=count ? reinterpret_cast<unsigned>(FableUiAllocateArchiveArray(count*8)) : 0;
    value->Capacity=reinterpret_cast<void*>(pointer+count*8);
    value->Begin=value->End=reinterpret_cast<void*>(pointer);
    for(unsigned i=0;i<count;++i,pointer+=8)
    {
        if(pointer) { unsigned* pair=reinterpret_cast<unsigned*>(pointer); pair[0]=pair[1]=0; }
    }
    value->End=reinterpret_cast<void*>(pointer);
    return value;
}
