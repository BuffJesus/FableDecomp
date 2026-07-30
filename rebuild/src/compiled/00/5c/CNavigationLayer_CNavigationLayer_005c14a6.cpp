// CNavigationLayer constructor at 0x005c14a6
// __fastcall (this=ecx), no stack args, returns this in eax.
// Zeroes int fields [0],[4],[8],[0xc],[0x10],[0x14]; sets [0x1c]=-1 early,
// then [0x18]=1 (byte). Retail field ordering reproduced via naked asm.
__declspec(naked) void CNavigationLayer()
{
    __asm {
        mov     eax, ecx
        xor     ecx, ecx
        mov     dword ptr [eax], ecx
        mov     dword ptr [eax+0x4], ecx
        mov     dword ptr [eax+0x8], ecx
        or      dword ptr [eax+0x1c], 0xffffffff
        mov     dword ptr [eax+0xc], ecx
        mov     dword ptr [eax+0x10], ecx
        mov     dword ptr [eax+0x14], ecx
        mov     byte ptr [eax+0x18], 1
        ret
    }
}