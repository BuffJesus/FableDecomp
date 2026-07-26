#include <cstdio>
int __fastcall CObstacleNavigatorExternalInterface_GetPNavigatorObstacles()
{
    return 0;
}
int main()
{
    if (CObstacleNavigatorExternalInterface_GetPNavigatorObstacles() == 0) { std::printf("AUTO_TINY_004df47f_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004df47f_TEST FAIL\n");
    return 1;
}