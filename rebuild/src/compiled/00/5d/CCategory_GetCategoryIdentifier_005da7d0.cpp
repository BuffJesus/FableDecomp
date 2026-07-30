// CCategory::GetCategoryIdentifier @ 0x005da7d0
// Reads a value at +0x28 of a refcounted sub-object held at this+0xc,
// bracketing the read with an inlined AddRef/Release (refcount at +0x4,
// vtable slot 1 called when the count drops to zero).
__declspec(naked) int GetCategoryIdentifier()
{
    __asm {
        mov     ecx, dword ptr [ecx+0x0c]
        test    ecx, ecx
        je      skip_addref
        inc     dword ptr [ecx+0x04]
skip_addref:
        mov     eax, dword ptr [ecx+0x04]
        push    esi
        mov     esi, dword ptr [ecx+0x28]
        dec     eax
        mov     dword ptr [ecx+0x04], eax
        jne     skip_release
        mov     eax, dword ptr [ecx]
        call    dword ptr [eax+0x04]
skip_release:
        mov     eax, esi
        pop     esi
        ret
    }
}