#include "rebuild_abi.h"

// Linear search over [begin, end) for the first int equal to *value.
// Unrolled 4-at-a-time main loop plus a fall-through remainder chain
// (Duff-style) for the trailing 1..3 elements. @ 0x0040E170.
extern "C" __declspec(naked) int* FABLE_FASTCALL
Array_LinearSearchInt(int* /*begin ecx*/, int* /*end edx*/, int* /*value*/)
{
    __asm
    {
        mov eax, ecx            ; eax = begin
        mov ecx, edx            ; ecx = end
        push ebx
        sub ecx, eax            ; ecx = end - begin
        push esi
        sar ecx, 4              ; chunkCount = (end-begin) >> 4
        test ecx, ecx
        push edi
        mov edi, dword ptr [esp + 10h]  ; edi = value
        jle done_loop

    loop_reload:
        mov esi, dword ptr [edi]        ; needle = *value

    loop_top:
        cmp dword ptr [eax], esi
        jz found
        mov ebx, dword ptr [eax + 4]
        add eax, 4
        cmp ebx, esi
        jz found
        mov ebx, dword ptr [eax + 4]
        add eax, 4
        cmp ebx, esi
        jz found
        mov ebx, dword ptr [eax + 4]
        add eax, 4
        cmp ebx, esi
        jz found
        add eax, 4
        dec ecx
        test ecx, ecx
        jg loop_top

    done_loop:
        mov ecx, edx
        sub ecx, eax
        sar ecx, 2              ; remainingCount = (end-begin) >> 2
        dec ecx
        jz rem1
        dec ecx
        jz rem2
        dec ecx
        jnz ret_end             ; remainingCount not in {1,2,3} -> return end
        mov ecx, dword ptr [eax]
        cmp ecx, dword ptr [edi]
        jz found
        add eax, 4

    rem2:
        mov ecx, dword ptr [eax]
        cmp ecx, dword ptr [edi]
        jz found
        add eax, 4

    rem1:
        mov ecx, dword ptr [eax]
        cmp ecx, dword ptr [edi]
        jz found

    ret_end:
        mov eax, edx

    found:
        pop edi
        pop esi
        pop ebx
        ret 8
    }
}
