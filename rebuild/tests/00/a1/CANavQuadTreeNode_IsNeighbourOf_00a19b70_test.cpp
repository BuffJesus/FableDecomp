#include <cstdio>
bool __fastcall CANavQuadTreeNode_IsNeighbourOf(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CANavQuadTreeNode_IsNeighbourOf(&object, 1, 2)) { std::printf("AUTO_TINY_00a19b70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a19b70_TEST FAIL\n");
    return 1;
}