#include <stdio.h>

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

int main()
{
    char r = (char)0x7F;
    void* fake_this = (void*)0xDEADBEEF;
    __asm {
        mov ecx, fake_this
        call AddChildPrimitive
        mov  byte ptr r, al
    }
    if (r != 0) {
        printf("FAIL got %d\n", (int)r);
        return 1;
    }
    printf("OK_0x0049373e result=%d\n", (int)r);
    return 0;
}