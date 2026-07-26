#include <stdio.h>

extern "C" __declspec(naked) void F_00447a10(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00447a10\n");
    return 0;
}