#include <cstdio>
int __fastcall COracleMinigameDef_GetSizeofClass()
{
    return 92;
}
int main()
{
    if (COracleMinigameDef_GetSizeofClass() == 92) { std::printf("AUTO_TINY_004d6601_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d6601_TEST FAIL\n");
    return 1;
}