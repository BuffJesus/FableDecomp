#include <cstdio>
bool __fastcall CCreatureActionBase_IsStrikeEvent(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CCreatureActionBase_IsStrikeEvent(&object, 1, 2)) { std::printf("AUTO_TINY_006924e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006924e0_TEST FAIL\n");
    return 1;
}