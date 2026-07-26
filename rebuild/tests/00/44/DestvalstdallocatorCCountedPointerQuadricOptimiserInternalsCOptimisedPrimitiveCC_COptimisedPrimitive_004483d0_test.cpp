#include <stdio.h>

extern "C" __declspec(naked) void F_004483d0(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_004483d0\n");
    return 0;
}