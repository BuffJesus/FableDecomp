#include <cstdio>
int __fastcall CNavSwitchableLeafNode_GetNodeType()
{
    return 3;
}
int main()
{
    if (CNavSwitchableLeafNode_GetNodeType() == 3) { std::printf("AUTO_TINY_00a84e00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a84e00_TEST FAIL\n");
    return 1;
}