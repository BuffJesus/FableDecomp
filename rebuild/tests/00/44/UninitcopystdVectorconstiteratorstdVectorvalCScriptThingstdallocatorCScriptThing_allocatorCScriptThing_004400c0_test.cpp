#include <stdio.h>

extern "C" __declspec(naked) void F_004400c0(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_004400c0\n");
    return 0;
}