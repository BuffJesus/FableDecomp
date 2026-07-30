__declspec(naked) void GetSelectedCategory()
{
    __asm {
        mov  eax, dword ptr [ecx+0x20]
        cmp  eax, dword ptr [ecx+0x24]
        je   L_empty
        mov  eax, dword ptr [ecx+0x70]
        imul eax, eax, 0x2c
        add  eax, dword ptr [ecx+0x20]
        ret
    L_empty:
        xor  eax, eax
        ret
    }
}