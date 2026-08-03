#include <stdio.h>

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

int main()
{
    // this=ecx; function takes no stack args and returns bool in al.
    char ret;
    void *fake_this = (void*)0xDEADBEEF;
    __asm {
        mov ecx, fake_this
        call AddChildPrimitive
        mov  ret, al
    }
    // Retail always initialises the local to 0 and returns it => false.
    if (ret != 0) { printf("FAIL got %d\n", (int)ret); return 1; }
    printf("OK_0x005f7d41 AddChildPrimitive returned %d\n", (int)ret);
    return 0;
}