#include "fable_ui_bank_storage.h"
void __fastcall FableUiClearBankRuntime(FableUiGraphicArray* array,void*)
{
    if(static_cast<long>(reinterpret_cast<unsigned>(array->Capacity)-reinterpret_cast<unsigned>(array->Begin))/12!=0)
    {
        // Retail's copy helper is called with identical source endpoints here.
        // Entries are trivial 12-byte records, so no element destruction occurs.
        array->End=array->Begin;
        void* begin=array->Begin; array->Begin=array->End=array->Capacity=0;
        if(begin) FableUiFreeArchiveArray(begin);
    }
}
