#include <cstdio>
void __stdcall CKeyboard_SetHasInputFocus(int arg0) {}
int main()
{
    CKeyboard_SetHasInputFocus(1);
    std::printf("AUTO_TINY_00a66ac0_TEST PASS\n");
    return 0;
}