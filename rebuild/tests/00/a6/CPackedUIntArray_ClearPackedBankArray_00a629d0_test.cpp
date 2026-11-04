#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_packed_uint_array.h"
void __fastcall ClearPackedBankArray(CPackedUIntArray*);
static void* expected;
static unsigned calls;
void operator delete[](void* p) { if (p != expected) abort(); ++calls; free(p); }
int main() {
    unsigned char storage[sizeof(CPackedUIntArray)];
    memset(storage, 0xA5, sizeof(storage));
    CPackedUIntArray* array = reinterpret_cast<CPackedUIntArray*>(storage);
    expected = malloc(16);
    array->PackedInts = static_cast<unsigned long*>(expected);
    ClearPackedBankArray(array);
    if (calls != 1 || array->PackedInts || array->Size || array->Bits || array->Bias) return 1;
    if (storage[9] != 0xA5 || storage[10] != 0xA5 || storage[11] != 0xA5) return 2;
    expected = 0;
    ClearPackedBankArray(array);
    if (calls != 2 || array->PackedInts || array->Size || array->Bits || array->Bias) return 3;
    puts("PACKED_CLEAR_00a629d0_PASS");
    return 0;
}
