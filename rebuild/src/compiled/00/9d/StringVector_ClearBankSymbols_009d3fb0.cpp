#include "fable_ui_bank_storage.h"
void __fastcall FableUiClearBankSymbols(FableUiGraphicArray* array,void*)
{
    if((static_cast<long>(reinterpret_cast<unsigned>(array->Capacity)-reinterpret_cast<unsigned>(array->Begin))>>2)!=0)
    {
        FableUiEraseStringRange(array,0,static_cast<FableUiStringValue*>(array->Begin),static_cast<FableUiStringValue*>(array->End));
        FableUiStringValue* begin=static_cast<FableUiStringValue*>(array->Begin);
        array->Begin=0;
        FableUiStringValue* end=static_cast<FableUiStringValue*>(array->End);
        array->End=array->Capacity=0;
        FableUiDestroyStringRange(begin,end);
        if(begin) FableUiFreeArchiveArray(begin);
    }
}
