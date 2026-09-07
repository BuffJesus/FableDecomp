#pragma optimize("s",on)
#include <stdlib.h>
void** __fastcall Allocator_Alloc_56bytes(void** output, void*, const void*)
{
    *output = 0;
    *output = malloc(0x38);
    return output;
}