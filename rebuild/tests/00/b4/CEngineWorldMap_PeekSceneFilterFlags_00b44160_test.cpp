#include <cstdio>
int __fastcall CEngineWorldMap_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineWorldMap_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b44160_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b44160_TEST FAIL\n");
    return 1;
}