#include <cstdio>

struct CBalverineBattleBase {
    void Base_op_assign(const CBalverineBattleBase* src);
};

struct CBalverineBattleDef {
    char pad[0x28];
    int f28;
    int f2c;
    int f30;
};

void CBalverineBattleBase::Base_op_assign(const CBalverineBattleBase* src)
{
    for (int i = 0; i < 0x28; i++) ((char*)this)[i] = ((const char*)src)[i];
}

CBalverineBattleDef* __fastcall CBalverineBattleDef_operatorassign(CBalverineBattleDef* self, void* edx, const CBalverineBattleDef* src)
{
    ((CBalverineBattleBase*)self)->Base_op_assign((const CBalverineBattleBase*)src);
    self->f28 = src->f28;
    self->f2c = src->f2c;
    self->f30 = src->f30;
    return self;
}

int main()
{
    CBalverineBattleDef a, b;
    for (int i = 0; i < (int)sizeof(a); i++) ((char*)&a)[i] = 0;
    for (int i = 0; i < 0x28; i++) ((char*)&b)[i] = (char)(i+1);
    b.f28 = 0x11111111;
    b.f2c = 0x22222222;
    b.f30 = 0x33333333;
    CBalverineBattleDef* r = CBalverineBattleDef_operatorassign(&a, 0, &b);
    bool ok = (r == &a) && a.f28 == 0x11111111 && a.f2c == 0x22222222 && a.f30 == 0x33333333;
    for (int i = 0; i < 0x28; i++) if (((char*)&a)[i] != (char)(i+1)) ok = false;
    if (ok) { std::printf("CBalverineBattleDef_004537e9_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}