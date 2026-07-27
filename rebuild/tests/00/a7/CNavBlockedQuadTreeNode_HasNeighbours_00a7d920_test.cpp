#include <cstdio>
bool __fastcall CNavBlockedQuadTreeNode_HasNeighbours()
{
    return false;
}
int main()
{
    if (CNavBlockedQuadTreeNode_HasNeighbours() == false) { std::printf("AUTO_TINY_00a7d920_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a7d920_TEST FAIL\n");
    return 1;
}