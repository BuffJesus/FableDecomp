#include <stdio.h>

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

int main()
{
    bool r;
    __asm {
        call AddChildPrimitive
        mov r, al
    }
    if (r == false) {
        printf("OK_0x004853d8\n");
        return 0;
    }
    printf("BAD result=%d\n", (int)r);
    return 1;
}