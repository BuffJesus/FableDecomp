#pragma optimize("s",on)
#include <stdlib.h>
void** __fastcall ListNode_AllocateNode(void** output, void*, const void*)
{
    *output = 0;
    *output = malloc(0x18);
    return output;
}