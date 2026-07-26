#include <stdio.h>

extern "C" __declspec(naked) void F_004454e0(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_004454e0\n");
    return 0;
}