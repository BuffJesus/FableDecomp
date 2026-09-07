#pragma optimize("s",on)
#include <stdlib.h>
void** __fastcall Allocator_Alloc_28bytes(void** output, void*, const void*)
{
    *output = 0;
    *output = malloc(0x1c);
    return output;
}