#include <stdio.h>

extern "C" __declspec(naked) void F_004431f0(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_004431f0\n");
    return 0;
}