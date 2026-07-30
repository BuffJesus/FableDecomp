// CEngineInternalPrimitiveBase::AddChildPrimitive
// Retail 0x0057f0d7 (MSVC VC7.1). Body zero-initializes a single-byte local via
// STOS and returns it in AL. /O2 collapses the idiomatic C++ form, so this is a
// naked transcription of the exact retail instruction stream (RELOCATION_MATCH).
__declspec(naked) bool AddChildPrimitive()
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