#include <cstdio>

__declspec(naked) unsigned int __fastcall GFROR13(unsigned int value)
{
    (void)value;
    __asm
    {
        push ecx
        mov  dword ptr [esp], ecx
        ror  dword ptr [esp], 0x0d
        mov  eax, dword ptr [esp]
        pop  ecx
        ret
    }
}

static unsigned int ref_ror13(unsigned int v)
{
    return (v >> 13) | (v << (32 - 13));
}

int main()
{
    unsigned int cases[] = { 0u, 1u, 0xFFFFFFFFu, 0xDEADBEEFu, 0x12345678u, 0x00002000u, 0x80000000u };
    for (int i = 0; i < 7; ++i)
    {
        unsigned int got = GFROR13(cases[i]);
        unsigned int want = ref_ror13(cases[i]);
        if (got != want)
        {
            std::printf("FAIL in=%08X got=%08X want=%08X\n", cases[i], got, want);
            return 1;
        }
    }
    std::printf("OK_0x00497890\n");
    return 0;
}