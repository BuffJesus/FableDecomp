#include <cstdio>
struct CSubField { int v; };
struct CSpecialAbilitiesDrainLifeDataDef {
    unsigned char _pad_0x0[0x28];
    CSubField sub;
};
struct CBaseDrainLifeDef {
    void Copy(const CBaseDrainLifeDef* other) { }
};

void __fastcall CSpecialAbilitiesDrainLifeDataDef_Copy(CSpecialAbilitiesDrainLifeDataDef* self, int _edx, const CSpecialAbilitiesDrainLifeDataDef* other)
{
    ((CBaseDrainLifeDef*)self)->Copy((const CBaseDrainLifeDef*)other);
    self->sub = other->sub;
}

int main() {
    CSpecialAbilitiesDrainLifeDataDef a, b;
    b.sub.v = 0x1234;
    a.sub.v = 0;
    CSpecialAbilitiesDrainLifeDataDef_Copy(&a, 0, &b);
    if (a.sub.v == 0x1234) { std::printf("CSpecialAbilitiesDrainLifeDataDef_00453aae_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}