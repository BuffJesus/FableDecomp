// Byte-exact reconstruction of CEditTransactionBase::ActionSetEngineTheme @ 0x009d52b0
struct C2DCoordI;

struct Target {
    // implicit thiscall member: this in ecx, four stack args
    void Apply(void* a, C2DCoordI* b, unsigned long c, unsigned long d);
};

struct CEditTransactionBase {
    void*   m0;   // +0x00
    Target* m4;   // +0x04
    void*   m8;   // +0x08
};

void __fastcall ActionSetEngineTheme(CEditTransactionBase* self, void* /*edx*/, C2DCoordI* coord, unsigned long a, unsigned long b)
{
    self->m4->Apply(self->m8, coord, a, b);
}