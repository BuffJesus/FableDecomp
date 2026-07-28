#include <cstdlib>

void** __fastcall Stack_Initialize(void** stack)
{
    *stack = nullptr;
    *stack = std::malloc(0x18);
    return stack;
}