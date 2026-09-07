#pragma optimize("s",on)
#include <stdlib.h>

void __fastcall _Uninit_copy(int first, int last, void* destination, const void* construction_cookie);

#pragma optimize("y", off)
void* __stdcall STL_AllocateAndInitializeArray(int count, int first, int last)
{
    void* allocation;
    if (count != 0) {
        allocation = malloc(count << 2);
    } else {
        allocation = 0;
    }
    _Uninit_copy(first, last, allocation, (const char*)&count + 3);
    return allocation;
}
#pragma optimize("y", on)