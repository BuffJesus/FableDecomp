#include <cstdio>
int __fastcall CJoystickDX_GetJoystickDeviceNumber()
{
    return 0;
}
int main()
{
    if (CJoystickDX_GetJoystickDeviceNumber() == 0) { std::printf("AUTO_TINY_00ab7900_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00ab7900_TEST FAIL\n");
    return 1;
}