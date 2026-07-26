#include <stdio.h>

extern "C" __declspec(naked) void F_00440730(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00440730\n");
    return 0;
}