#include <cstdio>

struct CCombatActionBase {
    char pad[0xc0];
    int strikeCount;
};

bool __fastcall CCombatActionBase_IsStrikeAction(const CCombatActionBase* self)
{
    return self->strikeCount > 0;
}

int main()
{
    CCombatActionBase a;
    a.strikeCount = 5;
    CCombatActionBase b;
    b.strikeCount = 0;
    CCombatActionBase c;
    c.strikeCount = -3;
    if (CCombatActionBase_IsStrikeAction(&a) != true) { std::printf("FAIL pos\n"); return 1; }
    if (CCombatActionBase_IsStrikeAction(&b) != false) { std::printf("FAIL zero\n"); return 1; }
    if (CCombatActionBase_IsStrikeAction(&c) != false) { std::printf("FAIL neg\n"); return 1; }
    std::printf("CCombatActionBase_0062ea30_TEST PASS\n");
    return 0;
}