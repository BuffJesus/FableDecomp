#include <cstdio>
bool __fastcall CModeStandBase_IsAllowingPlayerSpecialMovementMoves()
{
    return true;
}
int main()
{
    if (CModeStandBase_IsAllowingPlayerSpecialMovementMoves() == true) { std::printf("AUTO_TINY_0073da00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0073da00_TEST FAIL\n");
    return 1;
}