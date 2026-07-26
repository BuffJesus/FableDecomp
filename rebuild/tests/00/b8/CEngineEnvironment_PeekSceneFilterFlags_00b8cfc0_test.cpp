#include <cstdio>
int __fastcall CEngineEnvironment_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineEnvironment_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b8cfc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b8cfc0_TEST FAIL\n");
    return 1;
}