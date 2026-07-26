#include <cstdio>
int __fastcall CEnginePrimitiveRenderer2D_PeekSceneFilterFlags()
{
    return 2;
}
int main()
{
    if (CEnginePrimitiveRenderer2D_PeekSceneFilterFlags() == 2) { std::printf("AUTO_TINY_00b4b710_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b4b710_TEST FAIL\n");
    return 1;
}