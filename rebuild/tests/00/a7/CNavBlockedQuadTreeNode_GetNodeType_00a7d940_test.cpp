#include <cstdio>
int __fastcall CNavBlockedQuadTreeNode_GetNodeType()
{
    return 2;
}
int main()
{
    if (CNavBlockedQuadTreeNode_GetNodeType() == 2) { std::printf("AUTO_TINY_00a7d940_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a7d940_TEST FAIL\n");
    return 1;
}