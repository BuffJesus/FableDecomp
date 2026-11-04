#include <new>
#include "fable_packed_uint_array.h"
CPackedUIntArray::~CPackedUIntArray() {
    operator delete[](PackedInts);
    PackedInts = 0; Size = 0; Bits = 0; Bias = 0;
}
