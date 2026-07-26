#include <stdio.h>

extern "C" __declspec(naked) void F_00448280(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00448280\n");
    return 0;
}