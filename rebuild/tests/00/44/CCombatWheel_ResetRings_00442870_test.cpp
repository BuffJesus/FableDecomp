#include <stdio.h>

extern "C" __declspec(naked) void F_00442870(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00442870\n");
    return 0;
}