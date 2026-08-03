#include <cstdio>

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
    if (r == false)
        std::printf("OK_0x00578ce6 result=%d\n", (int)r);
    else
        std::printf("BAD result=%d\n", (int)r);
    return 0;
}