#include <cstdio>
int __fastcall CANavQuadTreeNode_GetRegion()
{
    return 0;
}
int main()
{
    if (CANavQuadTreeNode_GetRegion() == 0) { std::printf("AUTO_TINY_00a19b80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a19b80_TEST FAIL\n");
    return 1;
}