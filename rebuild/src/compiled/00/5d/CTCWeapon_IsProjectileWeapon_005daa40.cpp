__declspec(naked) bool IsProjectileWeapon()
{
    __asm {
        mov     ecx, dword ptr [ecx+0x0c]
        test    ecx, ecx
        je      L1
        inc     dword ptr [ecx+0x04]
    L1:
        mov     eax, dword ptr [ecx+0x04]
        push    esi
        mov     esi, dword ptr [ecx+0x2c]
        dec     eax
        mov     dword ptr [ecx+0x04], eax
        jne     L2
        mov     eax, dword ptr [ecx]
        call    dword ptr [eax+0x04]
    L2:
        xor     eax, eax
        cmp     esi, 0x03
        sete    al
        pop     esi
        ret
    }
}