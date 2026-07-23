#include <stdio.h>

extern "C" __declspec(naked) void F_00448410(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00448410\n");
    return 0;
}