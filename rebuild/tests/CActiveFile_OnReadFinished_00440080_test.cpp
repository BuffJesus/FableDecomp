#include <stdio.h>

extern "C" __declspec(naked) void F_00440080(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00440080\n");
    return 0;
}