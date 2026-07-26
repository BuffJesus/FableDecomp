#include <cstdio>
bool __fastcall CAIStateGroup_ServeFoodAtHome_SetPlaceForSelf()
{
    return true;
}
int main()
{
    if (CAIStateGroup_ServeFoodAtHome_SetPlaceForSelf() == true) { std::printf("AUTO_TINY_00901a2a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00901a2a_TEST FAIL\n");
    return 1;
}