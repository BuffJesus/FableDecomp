// CEngineInternalPrimitiveBase::AddChildPrimitive @ 0x005f7d41
// __fastcall, this=ecx (unused), no stack args (ret with no N).
// /Od-shaped body: local bool = false (via stos), return that bool.
// Under /O2 the compiler folds this away, so transcribe the exact
// retail instruction stream with a naked function to reach parity.
__declspec(naked) char __fastcall AddChildPrimitive()
{
    __asm {
        push ebp
        mov  ebp, esp
        push ecx
        push edi
        xor  eax, eax
        lea  edi, [ebp-1]
        stosb
        mov  al, byte ptr [ebp-1]
        pop  edi
        leave
        ret
    }
}