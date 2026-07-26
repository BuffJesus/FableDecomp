#include <cstdio>
bool __fastcall CCreatureActionBase_IsActionReadyForInterruptionByMovement(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CCreatureActionBase_IsActionReadyForInterruptionByMovement(&object, 1, 2)) { std::printf("AUTO_TINY_00692710_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692710_TEST FAIL\n");
    return 1;
}