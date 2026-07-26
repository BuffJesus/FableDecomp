#include <cstdio>
int __fastcall CCoinGameObstacleDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CCoinGameObstacleDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d597b_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d597b_TEST FAIL\n");
    return 1;
}