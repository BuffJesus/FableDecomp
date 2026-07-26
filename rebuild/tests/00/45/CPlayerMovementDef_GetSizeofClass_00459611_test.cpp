#include <cstdio>
int __fastcall CPlayerMovementDef_GetSizeofClass()
{
    return 124;
}
int main()
{
    if (CPlayerMovementDef_GetSizeofClass() == 124) { std::printf("AUTO_TINY_00459611_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00459611_TEST FAIL\n");
    return 1;
}