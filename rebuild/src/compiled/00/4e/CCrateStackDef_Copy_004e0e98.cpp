#pragma optimize("s",on)
// CCrateStackDef::Copy  0x004e0e98
struct CDefClassBase;

struct CBaseDef {
    // base class with Copy at 0x431f10 (__fastcall: ecx=this, stack=other)
    void Copy(const CDefClassBase* other);
};

struct CCrateStackDef : public CBaseDef {
    char pad[0x28 - sizeof(CBaseDef)];
    unsigned int m28;
    unsigned int m2c;
};

void __fastcall Copy(CCrateStackDef* self, void* edx, const CDefClassBase* other)
{
    self->CBaseDef::Copy(other);
    const CCrateStackDef* o = (const CCrateStackDef*)other;
    self->m28 = o->m28;
    self->m2c = o->m2c;
}