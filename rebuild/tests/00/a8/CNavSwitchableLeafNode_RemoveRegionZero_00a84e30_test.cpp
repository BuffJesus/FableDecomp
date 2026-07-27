#include <cstdio>
bool __fastcall CNavSwitchableLeafNode_RemoveRegionZero()
{
    return false;
}
int main()
{
    if (CNavSwitchableLeafNode_RemoveRegionZero() == false) { std::printf("AUTO_TINY_00a84e30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a84e30_TEST FAIL\n");
    return 1;
}