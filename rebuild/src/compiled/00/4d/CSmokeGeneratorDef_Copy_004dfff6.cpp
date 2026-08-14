#pragma optimize("s",on)
struct CDefClassBase;

struct CSmokeGeneratorDef {
    char pad[0x28];
    int m28;
    int m2c;
    void BaseCopy(const CDefClassBase* src);  // __fastcall: ecx=this, src on stack
};

void __fastcall CSmokeGeneratorDef_Copy(CSmokeGeneratorDef* self, void* /*edx*/, const CDefClassBase* src)
{
    self->BaseCopy(src);
    const CSmokeGeneratorDef* s = (const CSmokeGeneratorDef*)src;
    self->m28 = s->m28;
    self->m2c = s->m2c;
}