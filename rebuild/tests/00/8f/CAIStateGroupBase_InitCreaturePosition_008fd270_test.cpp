#include <cstdio>
bool __fastcall CAIStateGroupBase_InitCreaturePosition()
{
    return false;
}
int main()
{
    if (CAIStateGroupBase_InitCreaturePosition() == false) { std::printf("AUTO_TINY_008fd270_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fd270_TEST FAIL\n");
    return 1;
}