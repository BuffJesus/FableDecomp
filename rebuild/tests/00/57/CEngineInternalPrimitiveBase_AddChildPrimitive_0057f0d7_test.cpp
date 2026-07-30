#include <stdio.h>

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

int main()
{
    bool r = AddChildPrimitive();
    if (r == false) {
        printf("OK_0x0057f0d7\n");
        return 0;
    }
    printf("FAIL_0x0057f0d7 got=%d\n", (int)r);
    return 1;
}