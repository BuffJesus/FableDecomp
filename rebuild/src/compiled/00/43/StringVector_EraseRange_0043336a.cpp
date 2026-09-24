#include "fable_ui_bank_storage.h"
FableUiStringValue* __fastcall FableUiEraseStringRange(FableUiGraphicArray* array,void*,FableUiStringValue* first,FableUiStringValue* last)
{
    FableUiStringValue* destination=first;
    long count=(static_cast<long>(reinterpret_cast<unsigned>(array->End)-reinterpret_cast<unsigned>(last)))>>2;
    for(long i=0;i<count;++i,++destination,++last) FableUiAssignString(destination,0,last);
    FableUiDestroyStringRange(destination,static_cast<FableUiStringValue*>(array->End));
    array->End=destination;
    return first;
}
