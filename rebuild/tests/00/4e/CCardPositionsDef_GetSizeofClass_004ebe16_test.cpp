#include <cstdio>
int __fastcall CCardPositionsDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CCardPositionsDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_004ebe16_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004ebe16_TEST FAIL\n");
    return 1;
}