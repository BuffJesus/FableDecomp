#include <stdio.h>

extern "C" __declspec(naked) void F_004496b0(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_004496b0\n");
    return 0;
}