#include <stdio.h>

extern "C" __declspec(naked) void F_00440770(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00440770\n");
    return 0;
}