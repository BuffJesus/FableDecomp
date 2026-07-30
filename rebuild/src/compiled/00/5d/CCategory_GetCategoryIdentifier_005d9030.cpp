__declspec(naked) int __fastcall GetCategoryIdentifier(void* self)
{
    __asm {
        mov     ecx, dword ptr [ecx+0Ch]
        test    ecx, ecx
        je      skip
        inc     dword ptr [ecx+4]
    skip:
        mov     eax, dword ptr [ecx+4]
        push    esi
        mov     esi, dword ptr [ecx+50h]
        dec     eax
        mov     dword ptr [ecx+4], eax
        jne     done
        mov     eax, dword ptr [ecx]
        call    dword ptr [eax+4]
    done:
        mov     eax, esi
        pop     esi
        ret
    }
}