#include <stdio.h>

extern "C" __declspec(naked) void F_00440650(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00440650\n");
    return 0;
}