#include <cstdio>
void* __fastcall CNavNavigableLeafNode_GetLeafNodeAt(void* self, int, int)
{
    return self;
}
int main()
{
    int x = 0;
    if (CNavNavigableLeafNode_GetLeafNodeAt(&x, 1, 2) == &x) { std::printf("AUTO_TINY_00a75b40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a75b40_TEST FAIL\n");
    return 1;
}