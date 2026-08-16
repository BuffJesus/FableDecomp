#include <cstdio>
int __fastcall CNavQuadTreeNode_GetNodeType()
{
    return 0;
}
int main()
{
    if (CNavQuadTreeNode_GetNodeType() == 0) { std::printf("AUTO_TINY_00a7b950_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a7b950_TEST FAIL\n");
    return 1;
}