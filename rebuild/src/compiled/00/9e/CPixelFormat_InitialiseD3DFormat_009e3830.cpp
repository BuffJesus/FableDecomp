#include "fable_texture_lifecycle.h"
#include "fable_pixel_format_table.h"

__declspec(naked)
void CPixelFormatByteLengthView::Initialise(fable_u32)
{
    __asm
    {
        push esi
        mov esi, dword ptr [esp + 8]
        push edi
        xor edx, edx
        mov eax, OFFSET g_FablePixelFormatTable + 8
        align 16
    search:
        cmp dword ptr [eax - 8], esi
        je found
        mov edi, dword ptr [eax + 36]
        add eax, 36
        inc edx
        cmp edi, -1
        jne search
        or eax, -1
        pop edi
        mov dword ptr [ecx], eax
        pop esi
        ret 4
    found:
        pop edi
        mov dword ptr [ecx], edx
        pop esi
        ret 4
    }
}
