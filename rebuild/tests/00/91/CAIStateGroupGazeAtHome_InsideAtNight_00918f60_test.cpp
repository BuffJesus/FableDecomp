#include <cstdio>
bool __fastcall CAIStateGroup_GazeAtHome_InsideAtNight()
{
    return true;
}
int main()
{
    if (CAIStateGroup_GazeAtHome_InsideAtNight() == true) { std::printf("AUTO_TINY_00918f60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00918f60_TEST FAIL\n");
    return 1;
}