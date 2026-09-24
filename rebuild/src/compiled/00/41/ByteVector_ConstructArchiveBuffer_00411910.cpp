#include "fable_ui_bank_decode.h"
#include <string.h>
FableUiGraphicArray* __fastcall FableUiConstructArchiveBytes(FableUiGraphicArray* value,void*,unsigned count)
{
    value->Begin=value->End=value->Capacity=0;
    unsigned char* data=count ? static_cast<unsigned char*>(FableUiAllocateArchiveArray(count)) : 0;
    value->Capacity=reinterpret_cast<void*>(reinterpret_cast<unsigned>(data)+count);
    value->Begin=data;
    if(count) memset(data,0,count);
    value->End=reinterpret_cast<void*>(reinterpret_cast<unsigned>(data)+count);
    return value;
}
