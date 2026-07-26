#include <cstdint>

extern "C" __declspec(naked) void __cdecl __SEH_prolog(std::uint32_t /*scopeTable*/, std::int32_t stackAllocation)
{
    __asm {
        push    0x004013D8
        mov     eax, dword ptr fs:[0]
        push    eax
        mov     eax, dword ptr [esp + 0x10]
        mov     dword ptr [esp + 0x10], ebp
        lea     ebp, [esp + 0x10]
        sub     esp, eax
        push    ebx
        push    esi
        push    edi
        mov     eax, dword ptr [ebp - 8]
        mov     dword ptr [ebp - 0x18], esp
        push    eax
        mov     eax, dword ptr [ebp - 4]
        mov     dword ptr [ebp - 4], 0xFFFFFFFF
        mov     dword ptr [ebp - 8], eax
        lea     eax, [ebp - 0x10]
        mov     dword ptr fs:[0], eax
        ret
    }
}