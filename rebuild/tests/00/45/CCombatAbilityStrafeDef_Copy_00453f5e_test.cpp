#include <cstdio>

struct CCombatAbilityStrafeDef { int x; };

static int g_called = 0;
static CCombatAbilityStrafeDef* g_self = 0;
static const CCombatAbilityStrafeDef* g_other = 0;

void __fastcall CCombatAbilityStrafeDef_Copy_impl(CCombatAbilityStrafeDef* self, void* dummy, const CCombatAbilityStrafeDef* other)
{
    (void)dummy;
    g_called = 1;
    g_self = self;
    g_other = other;
}

void __fastcall CCombatAbilityStrafeDef_Copy(CCombatAbilityStrafeDef* self, void* dummy, const CCombatAbilityStrafeDef* other)
{
    CCombatAbilityStrafeDef_Copy_impl(self, dummy, other);
}

int main()
{
    CCombatAbilityStrafeDef a; a.x = 1;
    CCombatAbilityStrafeDef b; b.x = 2;
    CCombatAbilityStrafeDef_Copy(&a, 0, &b);
    if (g_called == 1 && g_self == &a && g_other == &b) {
        std::printf("CCombatAbilityStrafeDef_00453f5e_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}