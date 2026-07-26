#include <stdio.h>

extern "C" __declspec(naked) void F_004477c0(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_004477c0\n");
    return 0;
}