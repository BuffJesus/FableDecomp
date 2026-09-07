#pragma optimize("s",on)
#include <cstdlib>

void** __fastcall CList_AllocateNode(
    void** outNode,
    void* unusedRegisterArgument,
    void* unusedStackArgument)
{
    (void)unusedRegisterArgument;
    (void)unusedStackArgument;
    *(unsigned long*)outNode &= 0;
    *outNode = std::malloc(0x14);
    return outNode;
}