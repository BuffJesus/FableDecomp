#include <cstdio>
bool __fastcall CNavSwitchableLeafNode_IsSwitchable()
{
    return true;
}
int main()
{
    if (CNavSwitchableLeafNode_IsSwitchable() == true) { std::printf("AUTO_TINY_00a84e10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a84e10_TEST FAIL\n");
    return 1;
}