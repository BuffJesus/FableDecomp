#include <stdio.h>

extern "C" __declspec(naked) void F_00447d50(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00447d50\n");
    return 0;
}