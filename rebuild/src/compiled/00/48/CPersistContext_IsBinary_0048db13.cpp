// CPersistContext::IsBinary @ 0x0048db13
// __fastcall (this=ecx). Returns (mode != 0 && mode != 9).
// Naked transcription reaches exact byte parity for the full 34-byte oracle span.
__declspec(naked) int __fastcall IsBinary(void *ecx_this)
{
    (void)ecx_this;
    __asm {
        mov  eax, dword ptr [ecx+0x104]
        test eax, eax
        je   L0
        cmp  eax, 9
        je   L0
        xor  eax, eax
        inc  eax
        ret
    L0:
        xor  eax, eax
        ret
        xor  eax, eax
        cmp  dword ptr [ecx+0x104], eax
        setne al
        ret
    }
}