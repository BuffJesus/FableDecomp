#include <cstdio>
int __fastcall CWhisperBattleDef_GetSizeofClass()
{
    return 68;
}
int main()
{
    if (CWhisperBattleDef_GetSizeofClass() == 68) { std::printf("AUTO_TINY_004e394b_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e394b_TEST FAIL\n");
    return 1;
}