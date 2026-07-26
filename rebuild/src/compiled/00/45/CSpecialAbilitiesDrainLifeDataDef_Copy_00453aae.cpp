struct CSubField { int v; };
struct CSpecialAbilitiesDrainLifeDataDef {
    unsigned char _pad_0x0[0x28]; // to 0x28
    CSubField sub; // +0x28
};
struct CBaseDrainLifeDef {
    void Copy(const CBaseDrainLifeDef* other);
};

void __fastcall CSpecialAbilitiesDrainLifeDataDef_Copy(CSpecialAbilitiesDrainLifeDataDef* self, int _edx, const CSpecialAbilitiesDrainLifeDataDef* other)
{
    ((CBaseDrainLifeDef*)self)->Copy((const CBaseDrainLifeDef*)other);
    self->sub = other->sub;
}