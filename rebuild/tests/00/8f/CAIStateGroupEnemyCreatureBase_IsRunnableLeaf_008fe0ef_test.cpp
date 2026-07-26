#include <cstdio>
bool __fastcall CAIStateGroup_EnemyCreatureBase_IsRunnableLeaf()
{
    return true;
}
int main()
{
    if (CAIStateGroup_EnemyCreatureBase_IsRunnableLeaf() == true) { std::printf("AUTO_TINY_008fe0ef_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fe0ef_TEST FAIL\n");
    return 1;
}