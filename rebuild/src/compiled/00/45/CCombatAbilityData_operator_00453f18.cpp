struct CCombatAbilityData {
    int f0;
    int f4;
    int f8;
    int fc;
    int f10;
    int f14;
};

CCombatAbilityData* __fastcall CCombatAbilityData_operator_assign(CCombatAbilityData* self, int edx_dummy, const CCombatAbilityData* rhs) {
    self->f4  = rhs->f4;
    self->f8  = rhs->f8;
    self->fc  = rhs->fc;
    self->f10 = rhs->f10;
    self->f14 = rhs->f14;
    return self;
}