#include <new>
#include "fable_packed_uint_array.h"

// Typed ABI adapter for the reset/destruction body at 00A629D0.
void __fastcall ClearPackedBankArray(CPackedUIntArray* array)
{
    operator delete[](array->PackedInts);
    array->PackedInts = 0;
    array->Size = 0;
    array->Bits = 0;
    array->Bias = 0;
}
