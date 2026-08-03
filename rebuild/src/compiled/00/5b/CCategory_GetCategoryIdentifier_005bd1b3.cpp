// CCategory::GetCategoryIdentifier @ 0x005bd1b3
// __fastcall (this=ecx), no stack args; returns the identifier held by a
// ref-counted target reachable at this+0x14. Takes a scoped reference:
// AddRef (inc [target+0x4]) when non-null, read the identifier at [target+0x50],
// then Release (dec [target+0x4]); on zero, call vtable slot +4 (destructor).
__declspec(naked) int __fastcall GetCategoryIdentifier(void *thisptr)
{
    (void)thisptr;
    __asm {
        mov     ecx, dword ptr [ecx + 0x14]
        test    ecx, ecx
        je      L_skip_addref
        inc     dword ptr [ecx + 0x4]
    L_skip_addref:
        push    esi
        mov     esi, dword ptr [ecx + 0x50]
        dec     dword ptr [ecx + 0x4]
        jne     L_done
        mov     eax, dword ptr [ecx]
        call    dword ptr [eax + 0x4]
    L_done:
        mov     eax, esi
        pop     esi
        ret
    }
}