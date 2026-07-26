#include <cstdio>
int __fastcall CANavQuadTreeNode_GetNeighbours()
{
    return 0;
}
int main()
{
    if (CANavQuadTreeNode_GetNeighbours() == 0) { std::printf("AUTO_TINY_00a19b50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a19b50_TEST FAIL\n");
    return 1;
}