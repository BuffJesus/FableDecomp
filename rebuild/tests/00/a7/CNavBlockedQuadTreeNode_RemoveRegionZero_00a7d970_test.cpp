#include <cstdio>
bool __fastcall CNavBlockedQuadTreeNode_RemoveRegionZero()
{
    return true;
}
int main()
{
    if (CNavBlockedQuadTreeNode_RemoveRegionZero() == true) { std::printf("AUTO_TINY_00a7d970_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a7d970_TEST FAIL\n");
    return 1;
}