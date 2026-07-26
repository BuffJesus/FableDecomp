#include <stdio.h>

extern "C" __declspec(naked) void F_00449880(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00449880\n");
    return 0;
}