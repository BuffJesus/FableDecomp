#include <cstdio>
void* __fastcall CNavBlockedQuadTreeNode_GetLeafNodeAt(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (CNavBlockedQuadTreeNode_GetLeafNodeAt(&x, 1, 2) == &x) { std::printf("AUTO_TINY_00a76270_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a76270_TEST FAIL\n");
    return 1;
}