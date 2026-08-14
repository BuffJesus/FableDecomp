#pragma optimize("s",on)

struct CDefClassBase {
    char pad[0x28];
    void Copy(const CDefClassBase* src);  // __fastcall -> ecx=this, [esp]=src
};

struct CSummonableCreatureDef : CDefClassBase {
    unsigned int f28;  // at offset 0x28
    unsigned int f2c;  // at offset 0x2c
};

void __fastcall CSummonableCreatureDef_Copy(CSummonableCreatureDef* self, void* /*edx*/, const CDefClassBase* src)
{
    self->CDefClassBase::Copy(src);
    const CSummonableCreatureDef* s = (const CSummonableCreatureDef*)src;
    self->f28 = s->f28;
    self->f2c = s->f2c;
}