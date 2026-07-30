// CTCWeapon::IsAnalogueLoading @ 0x005da8f0
// Reads a byte field (+0x90) off a refcounted sub-object held at this+0xc,
// bracketing the read with AddRef/Release; Release invokes vtable slot +4
// when the refcount reaches zero. Naked transcription for exact byte parity.
__declspec(naked) unsigned char IsAnalogueLoading()
{
    __asm
    {
        mov  ecx, [ecx+0x0c]
        test ecx, ecx
        je   skip_inc
        inc  dword ptr [ecx+0x04]
    skip_inc:
        mov  eax, [ecx+0x04]
        push ebx
        mov  bl, byte ptr [ecx+0x90]
        dec  eax
        mov  dword ptr [ecx+0x04], eax
        jne  no_release
        mov  eax, [ecx]
        call dword ptr [eax+0x04]
    no_release:
        mov  al, bl
        pop  ebx
        ret
    }
}