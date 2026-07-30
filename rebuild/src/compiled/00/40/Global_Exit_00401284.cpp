#include "rebuild_abi.h"

extern "C" void (FABLE_CDECL* g_FableCrtExit_014401A0)(int);

extern "C" __declspec(naked) void FABLE_CDECL
FableExit_00401284(int)
{
    __asm
    {
        jmp dword ptr [g_FableCrtExit_014401A0]
    }
}
