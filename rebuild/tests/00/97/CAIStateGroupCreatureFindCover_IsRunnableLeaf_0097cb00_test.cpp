#include <cstdio>
bool __fastcall CAIStateGroup_CreatureFindCover_IsRunnableLeaf()
{
    return false;
}
int main()
{
    if (CAIStateGroup_CreatureFindCover_IsRunnableLeaf() == false) { std::printf("AUTO_TINY_0097cb00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0097cb00_TEST FAIL\n");
    return 1;
}