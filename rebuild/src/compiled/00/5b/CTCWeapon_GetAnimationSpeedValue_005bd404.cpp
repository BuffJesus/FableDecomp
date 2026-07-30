// CTCWeapon::GetAnimationSpeedValue @ 0x005bd404
// __fastcall (this=ecx) returning float in st(0).
// Reads ref-counted handle at this+0x68; AddRef/Release around reading
// the float at +0x3c; if refcount reaches zero, calls virtual slot 1.
__declspec(naked) float __fastcall GetAnimationSpeedValue(void* ecx)
{
    (void)ecx;
    __asm {
        push ebp
        mov  ebp, esp
        push ecx
        mov  ecx, dword ptr [ecx+0x68]
        test ecx, ecx
        je   skip_add
        inc  dword ptr [ecx+0x4]
    skip_add:
        mov  eax, dword ptr [ecx+0x3c]
        dec  dword ptr [ecx+0x4]
        mov  dword ptr [ebp-4], eax
        jne  no_rel
        mov  eax, dword ptr [ecx]
        call dword ptr [eax+4]
    no_rel:
        fld  dword ptr [ebp-4]
        leave
        ret
    }
}