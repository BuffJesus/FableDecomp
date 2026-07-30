__declspec(naked) bool AddChildPrimitive()
{
    // Retail is an unoptimized frame: declares a 1-byte local bool at [ebp-1],
    // zero-inits it via stosb, reads it back into al, returns false.
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