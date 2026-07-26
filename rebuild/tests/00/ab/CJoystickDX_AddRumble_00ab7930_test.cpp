#include <cstdio>
void __stdcall CJoystickDX_AddRumble(int arg0, int arg1, int arg2) {}
int main()
{
    CJoystickDX_AddRumble(1, 2, 3);
    std::printf("AUTO_TINY_00ab7930_TEST PASS\n");
    return 0;
}