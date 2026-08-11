struct CDefClassBase { char pad[0x2c]; };

struct CGoldDef {
    char pad[0x28];
    unsigned long field28;  // +0x28
    void BaseClone(const CDefClassBase* other);  // __fastcall base helper
};

void __fastcall Copy(CGoldDef* self, int /*edx*/, const CDefClassBase* other)
{
    self->BaseClone(other);
    self->field28 = ((const CGoldDef*)other)->field28;
}