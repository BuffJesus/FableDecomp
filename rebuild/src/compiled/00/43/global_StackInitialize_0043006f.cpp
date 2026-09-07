#pragma optimize("s",on)
#include <cstdlib>

void** __fastcall Stack_Initialize(
    void** stack,
    void* unusedRegisterArgument,
    void* unusedStackArgument)
{
    (void)unusedRegisterArgument;
    (void)unusedStackArgument;
    *(unsigned long*)stack &= 0;
    *stack = std::malloc(0x18);
    return stack;
}