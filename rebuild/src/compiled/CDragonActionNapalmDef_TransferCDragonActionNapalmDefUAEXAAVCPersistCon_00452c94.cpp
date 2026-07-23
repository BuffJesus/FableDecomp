#pragma optimize("s",on)
struct Field { unsigned char b[4]; };

struct CPersistContext {
    void Xfer(Field* p);
};

struct CDragonActionNapalmDef {
    unsigned char _pad[0x28];
    Field f0; // +0x28
    Field f1; // +0x2c
    Field f2; // +0x30
    Field f3; // +0x34
};

void __fastcall CDragonActionNapalmDef_Transfer(CDragonActionNapalmDef* self, int edx, CPersistContext* ctx)
{
    ctx->Xfer(&self->f0);
    ctx->Xfer(&self->f1);
    ctx->Xfer(&self->f2);
    ctx->Xfer(&self->f3);
}