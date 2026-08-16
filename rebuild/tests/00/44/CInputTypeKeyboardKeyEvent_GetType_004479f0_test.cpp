#include <cstdio>
int __fastcall CInputTypeKeyboardKeyEvent_GetType()
{
    return 0;
}
int main()
{
    if (CInputTypeKeyboardKeyEvent_GetType() == 0) { std::printf("AUTO_TINY_004479f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004479f0_TEST FAIL\n");
    return 1;
}