#include <cstdio>
int __fastcall CNavQuadTreeNode_GetRegionID()
{
    return 0;
}
int main()
{
    if (CNavQuadTreeNode_GetRegionID() == 0) { std::printf("AUTO_TINY_00a7b940_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a7b940_TEST FAIL\n");
    return 1;
}