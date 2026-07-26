#pragma optimize("s",on)

struct CBalverineBattleBase {
    void Base_op_assign(const CBalverineBattleBase* src);
};

struct CBalverineBattleDef {
    char pad[0x28];
    int f28;
    int f2c;
    int f30;
};

CBalverineBattleDef* __fastcall CBalverineBattleDef_operatorassign(CBalverineBattleDef* self, void* edx, const CBalverineBattleDef* src)
{
    ((CBalverineBattleBase*)self)->Base_op_assign((const CBalverineBattleBase*)src);
    self->f28 = src->f28;
    self->f2c = src->f2c;
    self->f30 = src->f30;
    return self;
}