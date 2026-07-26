#include <stdio.h>

extern "C" __declspec(naked) void F_00449990(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00449990\n");
    return 0;
}