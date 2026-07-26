#include <cstdio>
bool __fastcall CAIStateGroup_BanditPlot_PutWeaponAway()
{
    return false;
}
int main()
{
    if (CAIStateGroup_BanditPlot_PutWeaponAway() == false) { std::printf("AUTO_TINY_008fe802_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fe802_TEST FAIL\n");
    return 1;
}