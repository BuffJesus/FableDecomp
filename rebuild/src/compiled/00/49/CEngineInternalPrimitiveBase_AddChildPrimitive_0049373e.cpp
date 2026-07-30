// CEngineInternalPrimitiveBase::AddChildPrimitive @ 0x0049373e
// Naked transcription of the retail prologue/body.
__declspec(naked) char AddChildPrimitive()
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