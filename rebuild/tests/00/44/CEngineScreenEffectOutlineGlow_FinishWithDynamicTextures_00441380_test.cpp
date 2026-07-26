#include <stdio.h>

extern "C" __declspec(naked) void F_00441380(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00441380\n");
    return 0;
}