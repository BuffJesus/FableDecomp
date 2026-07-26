#pragma optimize("s",on)
struct CCombatAbilityBase {
    char pad[0x18];
    void assign(const CCombatAbilityBase* other);
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