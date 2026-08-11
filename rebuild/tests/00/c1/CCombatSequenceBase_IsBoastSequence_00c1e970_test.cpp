#include <cstdio>

// Match the vtable layout the target expects: slot 2 (call [eax+8]) = GetPeriod.
struct CCombatSequenceBase
{
    unsigned int period;
    virtual unsigned int v0() { return 0; }
    virtual unsigned int v1() { return 0; }
    virtual unsigned int GetPeriod() { return period; }
};

unsigned int __fastcall CCombatSequenceBase_IsBoastSequence(CCombatSequenceBase* self, unsigned int value);

int main()
{
    CCombatSequenceBase s;
    s.period = 7;

    unsigned int r1 = CCombatSequenceBase_IsBoastSequence(&s, 10u); // 10%7=3
    unsigned int r2 = CCombatSequenceBase_IsBoastSequence(&s, 14u); // 14%7=0
    unsigned int r3 = CCombatSequenceBase_IsBoastSequence(&s, 8u);  // 8%7=1

    s.period = 3;
    unsigned int r4 = CCombatSequenceBase_IsBoastSequence(&s, 9u);  // 0

    if (r1 == 3 && r2 == 0 && r3 == 1 && r4 == 0) {
        printf("BOAST_MODULO_OK\n");
        return 0;
    }
    printf("FAIL r1=%u r2=%u r3=%u r4=%u\n", r1, r2, r3, r4);
    return 1;
}