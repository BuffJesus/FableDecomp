#include <cstdio>
int __fastcall CObstacleNavigatorExternalInterface_GetPThingToNotAvoid()
{
    return 0;
}
int main()
{
    if (CObstacleNavigatorExternalInterface_GetPThingToNotAvoid() == 0) { std::printf("AUTO_TINY_004df482_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004df482_TEST FAIL\n");
    return 1;
}