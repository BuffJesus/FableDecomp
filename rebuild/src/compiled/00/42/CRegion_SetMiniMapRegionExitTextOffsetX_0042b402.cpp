extern "C" void sub_0042a82e(void);

extern "C" __declspec(naked) void candidate_0042b402(void)
{
    __asm {
        push    dword ptr [esp+8]
        call    sub_0042a82e
        mov     ecx, dword ptr [esp+4]
        mov     dword ptr [ecx], eax
        mov     eax, ecx
        ret     8
    }
}
