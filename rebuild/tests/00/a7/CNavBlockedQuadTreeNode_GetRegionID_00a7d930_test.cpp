#include <cstdio>
int __fastcall CNavBlockedQuadTreeNode_GetRegionID()
{
    return 0;
}
int main()
{
    if (CNavBlockedQuadTreeNode_GetRegionID() == 0) { std::printf("AUTO_TINY_00a7d930_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a7d930_TEST FAIL\n");
    return 1;
}