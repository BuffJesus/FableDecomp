#pragma optimize("s",on)
#include <stdlib.h>
void** __fastcall Allocator_Alloc_24bytes(void** output, void*, const void*)
{
    *output = 0;
    *output = malloc(0x18);
    return output;
}