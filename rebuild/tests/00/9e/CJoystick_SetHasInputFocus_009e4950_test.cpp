#include <cstdio>
void __stdcall CJoystick_SetHasInputFocus(int arg0) {}
int main()
{
    CJoystick_SetHasInputFocus(1);
    std::printf("AUTO_TINY_009e4950_TEST PASS\n");
    return 0;
}