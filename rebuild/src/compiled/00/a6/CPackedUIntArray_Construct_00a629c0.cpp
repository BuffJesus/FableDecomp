#include "fable_ui_bank_file.h"
CPackedUIntArray* __fastcall FableUiConstructPackedUIntArray(CPackedUIntArray* array,void*)
{
    array->PackedInts=0; array->Size=0; array->Bits=0; array->Bias=0;
    return array;
}
