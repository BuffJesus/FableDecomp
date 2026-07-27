#include "fable_boot.h"

#pragma comment( \
    linker, \
    "/alternatename:??0CProgressDisplay@@QAE@XZ=_CProgressDisplay_Constructor_00499ce0")

// Defining the body through its retail ABI entry point avoids the compiler's
// otherwise harmless constructor-return tail and preserves the exact retail
// instruction schedule. The linker alias keeps ordinary C++ construction
// sites bound to this implementation.
extern "C" __declspec(naked) void CProgressDisplay_Constructor_00499ce0()
{
    __asm
    {
        push ebx
        xor ebx, ebx
        push esi
        mov esi, ecx
        mov dword ptr [esi], offset g_CProgressDisplayVTable_012388B4
        mov dword ptr [esi + 04h], ebx
        mov dword ptr [esi + 08h], ebx
        mov dword ptr [esi + 0Ch], ebx
        mov dword ptr [esi + 10h], ebx
        mov dword ptr [esi + 14h], ebx
        mov dword ptr [esi + 18h], ebx
        mov dword ptr [esi + 1Ch], ebx
        mov dword ptr [esi + 20h], ebx
        mov dword ptr [esi + 24h], ebx
        mov dword ptr [esi + 28h], ebx
        mov dword ptr [esi + 2Ch], ebx
        mov dword ptr [esi + 30h], ebx
        mov dword ptr [esi + 34h], ebx
        mov dword ptr [esi + 38h], ebx
        mov dword ptr [esi + 3Ch], ebx
        mov dword ptr [esi + 40h], ebx

        lea ecx, [esi + 48h]
        mov dword ptr [esi + 44h], ebx
        call FableConstructWideString
        lea ecx, [esi + 4Ch]
        call FableConstructCharString
        lea ecx, [esi + 50h]
        call FableConstructCharString

        fld qword ptr [g_FableZeroDouble_0122ED70]
        fstp qword ptr [esi + 60h]
        mov al, 1
        fld qword ptr [g_FableZeroDouble_0122ED70]
        mov byte ptr [esi + 7Ah], al
        fstp qword ptr [esi + 68h]
        mov byte ptr [esi + 7Ch], al
        fld qword ptr [g_FableZeroDouble_0122ED70]
        mov dword ptr [esi + 54h], ebx
        mov dword ptr [esi + 58h], ebx
        fstp qword ptr [esi + 70h]
        mov byte ptr [esi + 78h], bl
        mov byte ptr [esi + 79h], bl
        mov byte ptr [esi + 7Bh], bl
        mov byte ptr [esi + 7Dh], bl
        mov byte ptr [esi + 7Eh], bl
        mov byte ptr [esi + 7Fh], bl
        mov byte ptr [esi + 80h], bl
        mov byte ptr [esi + 81h], bl
        mov eax, esi
        pop esi
        pop ebx
        ret
    }
}
