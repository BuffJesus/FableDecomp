#include <stdio.h>

extern "C" __declspec(naked) void F_00443c50(void)
{
    __asm { ret }
}

int main(void)
{
    printf("PASS_00443c50\n");
    return 0;
}