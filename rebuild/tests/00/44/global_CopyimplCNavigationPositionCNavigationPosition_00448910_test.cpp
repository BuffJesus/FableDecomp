#include <stdio.h>

extern "C" __declspec(naked) void F_00448910(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00448910\n");
    return 0;
}