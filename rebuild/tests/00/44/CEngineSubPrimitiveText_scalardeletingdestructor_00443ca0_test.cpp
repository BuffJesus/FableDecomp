#include <stdio.h>

extern "C" __declspec(naked) void F_00443ca0(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00443ca0\n");
    return 0;
}