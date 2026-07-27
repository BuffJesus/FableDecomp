#include <cstdio>
bool __fastcall CAIStateGroupBase_AllowPriorityOverlap()
{
    return false;
}
int main()
{
    if (CAIStateGroupBase_AllowPriorityOverlap() == false) { std::printf("AUTO_TINY_008fd273_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fd273_TEST FAIL\n");
    return 1;
}