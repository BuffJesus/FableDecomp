#include <cstdio>
bool __fastcall CAIStateGroupBase_IsTimingCritical()
{
    return false;
}
int main()
{
    if (CAIStateGroupBase_IsTimingCritical() == false) { std::printf("AUTO_TINY_008fd279_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fd279_TEST FAIL\n");
    return 1;
}