// GFROR13 @ 0x00497890 -- rotate-right-by-13 of the __fastcall ecx argument.
// Retail: push ecx; mov [esp],ecx; ror [esp],0xd; mov eax,[esp]; pop ecx; ret
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