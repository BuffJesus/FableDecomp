struct Category { char pad[0x2c]; };

// Retail __fastcall: this=ecx, index on stack, ret 4.
__declspec(naked) Category* __stdcall GetCategoryWithIndex(int /*index*/)
{
    __asm {
        mov  eax, dword ptr [esp+4]
        imul eax, eax, 0x2c
        add  eax, dword ptr [ecx+0x20]
        ret  4
    }
}