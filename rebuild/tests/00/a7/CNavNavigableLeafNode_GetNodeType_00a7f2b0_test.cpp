#include <cstdio>
int __fastcall CNavNavigableLeafNode_GetNodeType()
{
    return 1;
}
int main()
{
    if (CNavNavigableLeafNode_GetNodeType() == 1) { std::printf("AUTO_TINY_00a7f2b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a7f2b0_TEST FAIL\n");
    return 1;
}