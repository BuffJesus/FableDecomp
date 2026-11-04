#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_packed_uint_array.h"
int main() {
    unsigned char storage[sizeof(CPackedUIntArray)];memset(storage,0xA5,sizeof(storage));
    CPackedUIntArray* array=new (storage) CPackedUIntArray;
    if(array!=reinterpret_cast<CPackedUIntArray*>(storage)||array->PackedInts||array->Size||array->Bits||array->Bias)return 1;
    if(storage[9]!=0xA5||storage[10]!=0xA5||storage[11]!=0xA5)return 2;
    puts("PACKED_ARRAY_CTOR_00a629c0_PASS");return 0;
}