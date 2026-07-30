__declspec(naked) bool AddChildPrimitive()
{
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push edi
        xor eax, eax
        lea edi, [ebp-1]
        stos byte ptr es:[edi]
        mov al, byte ptr [ebp-1]
        pop edi
        leave
        ret
    }
}