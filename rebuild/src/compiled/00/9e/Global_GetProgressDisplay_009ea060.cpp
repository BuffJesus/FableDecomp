#include "fable_boot.h"

__declspec(naked) CCountedProgressDisplay* FABLE_FASTCALL
GetProgressDisplay(CCountedProgressDisplay* /* result */)
{
    // The retail ABI supplies the return-value storage in ecx. Preserve its
    // exact register schedule while spelling both counted fields symbolically.
    __asm
    {
        mov eax, ecx
        mov ecx, dword ptr [g_FableProgressDisplayObject_013CAA38]
        mov dword ptr [eax], ecx
        mov edx, dword ptr [g_FableProgressDisplayReference_013CAA3C]
        mov ecx, edx
        test ecx, ecx
        mov dword ptr [eax + 4], edx
        jz no_reference
        inc dword ptr [ecx]
    no_reference:
        ret
    }
}
