#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_packed_uint_array.h"
CPackedUIntArray::CPackedUIntArray() : PackedInts(0), Size(7), Bits(5), Bias(9) {}
static void* expected;
static unsigned calls;
void operator delete[](void* p) { if (p != expected) abort(); ++calls; free(p); }
int main() {
    unsigned char storage[sizeof(CPackedUIntArray)];
    memset(storage, 0xA5, sizeof(storage));
    CPackedUIntArray* array = new (storage) CPackedUIntArray;
    expected = malloc(16);
    array->PackedInts = static_cast<unsigned long*>(expected);
    array->~CPackedUIntArray();
    if (calls != 1 || array->PackedInts || array->Size || array->Bits || array->Bias) return 1;
    if (storage[9] != 0xA5 || storage[10] != 0xA5 || storage[11] != 0xA5) return 2;
    array = new (storage) CPackedUIntArray;
    expected = 0;
    array->~CPackedUIntArray();
    if (calls != 2 || array->PackedInts || array->Size || array->Bits || array->Bias) return 3;
    puts("PACKED_DTOR_00a62ac0_PASS");
    return 0;
}
