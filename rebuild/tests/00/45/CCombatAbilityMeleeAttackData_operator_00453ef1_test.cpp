#include <cstdio>

static int g_base_called = 0;

struct CCombatAbilityBase {
    char pad[0x18];
    void assign(const CCombatAbilityBase* other) { (void)other; g_base_called = 1; }
};

struct CCombatAbilityMeleeAttackData : public CCombatAbilityBase {
    unsigned char b18;
    unsigned char b19;
    char pad2[2];
    int d1c;
};

CCombatAbilityMeleeAttackData* __fastcall CCombatAbilityMeleeAttackData_operator_assign(
    CCombatAbilityMeleeAttackData* self, void* dummy, const CCombatAbilityMeleeAttackData* other)
{
    self->assign(other);
    self->b18 = other->b18;
    self->b19 = other->b19;
    self->d1c = other->d1c;
    return self;
}

int main()
{
    CCombatAbilityMeleeAttackData a, b;
    b.b18 = 0x11; b.b19 = 0x22; b.d1c = 0x12345678;
    a.b18 = 0; a.b19 = 0; a.d1c = 0;
    CCombatAbilityMeleeAttackData* r = CCombatAbilityMeleeAttackData_operator_assign(&a, 0, &b);
    if (r == &a && g_base_called == 1 && a.b18 == 0x11 && a.b19 == 0x22 && a.d1c == 0x12345678) {
        std::printf("CCombatAbilityMeleeAttackData_00453ef1_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}