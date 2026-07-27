#include <cstdio>
bool __fastcall CObstacleNavigatorExternalInterface_IsPlayer()
{
    return false;
}
int main()
{
    if (CObstacleNavigatorExternalInterface_IsPlayer() == false) { std::printf("AUTO_TINY_004df4a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004df4a0_TEST FAIL\n");
    return 1;
}