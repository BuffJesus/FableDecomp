#include <cstdio>
int __fastcall CEngineLocalDetailGenerator_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineLocalDetailGenerator_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b5a040_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b5a040_TEST FAIL\n");
    return 1;
}