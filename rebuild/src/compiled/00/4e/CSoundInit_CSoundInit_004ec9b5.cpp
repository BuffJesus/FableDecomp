__declspec(naked) void __fastcall CSoundInit()
{
    __asm {
        mov     eax, ecx
        fldz
        xor     ecx, ecx
        mov     dword ptr [eax], ecx
        fstp    dword ptr [eax+4]
        mov     dword ptr [eax+0x18], ecx
        mov     dword ptr [eax+0x1c], ecx
        mov     dword ptr [eax+0x20], ecx
        mov     dword ptr [eax+0x24], ecx
        mov     dword ptr [eax+0x28], ecx
        mov     dword ptr [eax+0x2c], ecx
        ret
    }
}