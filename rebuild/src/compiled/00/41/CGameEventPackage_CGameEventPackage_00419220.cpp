// Retail 0x00419220 CGameEventPackage ctor (__fastcall/__fastcall, this=ecx, returns this in eax).
// Zeroes field +4, then walks a 40-element array (stride 0x28) starting at this+0x0c:
// each element's byte[0]=0xff and byte[0x22]=0. Exact transcription of the retail disassembly.
__declspec(naked) void CGameEventPackage()
{
    __asm {
        mov     eax, ecx
        push    esi
        mov     dword ptr [eax+4], 0
        lea     edx, [eax+0x2e]
        mov     esi, 0x28
        or      cl, 0xff
    loop_top:
        mov     byte ptr [edx-0x22], cl
        mov     byte ptr [edx], 0
        add     edx, 0x28
        dec     esi
        jne     loop_top
        pop     esi
        ret
    }
}