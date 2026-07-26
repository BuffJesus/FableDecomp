#include <cstdio>
int __fastcall CFireheartMinigameDef_GetSizeofClass()
{
    return 60;
}
int main()
{
    if (CFireheartMinigameDef_GetSizeofClass() == 60) { std::printf("AUTO_TINY_004d665a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d665a_TEST FAIL\n");
    return 1;
}