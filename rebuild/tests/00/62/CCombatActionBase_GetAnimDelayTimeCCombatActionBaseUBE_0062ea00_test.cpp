#include <cstdio>
struct CCombatActionBase { int _vfp; };
long __fastcall CCombatActionBase_GetAnimDelayTime(const CCombatActionBase* self)
{
    (void)self;
    return 0;
}
int main()
{
    CCombatActionBase obj; obj._vfp = 0x1234;
    long r = CCombatActionBase_GetAnimDelayTime(&obj);
    if (r == 0) { std::printf("CCombatActionBase_0062ea00_TEST PASS\n"); return 0; }
    std::printf("FAIL got %ld\n", r); return 1;
}