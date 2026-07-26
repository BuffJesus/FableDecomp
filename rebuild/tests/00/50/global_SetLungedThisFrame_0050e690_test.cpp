#include <cstdio>

int __cdecl NBalverineCombat_GetRandomValue()
{
    return 100;
}

int __fastcall NBalverineCombat_SetLungedThisFrame(int n)
{
    return NBalverineCombat_GetRandomValue() % n;
}

int main()
{
    if (NBalverineCombat_SetLungedThisFrame(7) == (100 % 7) &&
        NBalverineCombat_SetLungedThisFrame(30) == (100 % 30))
    {
        std::printf("_global_0050e690_TEST PASS\n");
        return 0;
    }
    std::printf("_global_0050e690_TEST FAIL\n");
    return 1;
}