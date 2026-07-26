#include <cstdio>
int __fastcall CEngineLocalDetailObjectDef_GetSizeofClass()
{
    return 136;
}
int main()
{
    if (CEngineLocalDetailObjectDef_GetSizeofClass() == 136) { std::printf("AUTO_TINY_00477698_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00477698_TEST FAIL\n");
    return 1;
}