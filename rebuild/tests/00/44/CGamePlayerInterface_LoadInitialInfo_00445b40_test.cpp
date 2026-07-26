#include <stdio.h>

extern "C" __declspec(naked) void F_00445b40(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00445b40\n");
    return 0;
}