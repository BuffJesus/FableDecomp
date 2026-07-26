#include <cstdio>
int __fastcall CEngineLocalDetailLayerDef_GetSizeofClass()
{
    return 84;
}
int main()
{
    if (CEngineLocalDetailLayerDef_GetSizeofClass() == 84) { std::printf("AUTO_TINY_00477424_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00477424_TEST FAIL\n");
    return 1;
}