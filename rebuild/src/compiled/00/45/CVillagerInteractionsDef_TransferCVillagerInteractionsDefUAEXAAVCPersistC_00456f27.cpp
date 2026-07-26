#pragma optimize("s",on)
struct CPersistContext {
    void PersistA(int& r);
    void PersistB(int& r);
};

struct CVillagerInteractionsDef {
    char pad[0x3c];
    int fieldA;   // +0x3c
    int fieldB;   // +0x40
    int fieldC;   // +0x44
};

void __fastcall CVillagerInteractionsDef_Transfer(CVillagerInteractionsDef* self, void* /*edx*/, CPersistContext* ctx)
{
    ctx->PersistA(self->fieldA);
    ctx->PersistA(self->fieldB);
    ctx->PersistB(self->fieldC);
}