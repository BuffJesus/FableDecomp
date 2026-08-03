__declspec(naked) void IsCurrentOrUpcomingState()
{
    __asm {
        mov  eax, dword ptr [esp+4]
        cmp  dword ptr [ecx+0xc0], eax
        je   L_true
        cmp  dword ptr [ecx+0xa8], eax
        je   L_true
        cmp  dword ptr [ecx+0xb0], eax
        je   L_true
        xor  eax, eax
        ret  4
    L_true:
        mov  eax, 1
        ret  4
    }
}