#include <stdio.h>

static void * last_a;
static void * last_b;
static void * last_c;

extern "C" void * Ucopy_target(void *a, void *b, void *c)
{
    last_a = a; last_b = b; last_c = c;
    return a;
}

__declspec(naked) void * __stdcall Ucopy_C(void *, void *)
{
    __asm {
        push ebp
        mov ebp, esp
        lea eax, [ebp+0x0f]
        push eax
        push dword ptr [ebp+0x0c]
        push dword ptr [ebp+0x08]
        call Ucopy_target
        add esp, 0x0c
        pop ebp
        ret 8
    }
}

int main()
{
    void *r = Ucopy_C((void*)0x2222, (void*)0x3333);
    if (last_a == (void*)0x2222 && last_b == (void*)0x3333 && r == (void*)0x2222 && last_c != 0)
        printf("OK_005c190b a=%p b=%p c=%p\n", last_a, last_b, last_c);
    else
        printf("FAIL a=%p b=%p r=%p\n", last_a, last_b, r);
    return 0;
}