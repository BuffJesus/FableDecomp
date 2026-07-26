#include <cstdio>
bool __fastcall CANavQuadTreeNode_IsSwitchable()
{
    return false;
}
int main()
{
    if (CANavQuadTreeNode_IsSwitchable() == false) { std::printf("AUTO_TINY_00a19ba0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a19ba0_TEST FAIL\n");
    return 1;
}