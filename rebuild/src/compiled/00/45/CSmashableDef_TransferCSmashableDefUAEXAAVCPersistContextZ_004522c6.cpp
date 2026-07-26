#pragma optimize("s",on)
struct CSmashableDef {
    char pad[0x25];
    unsigned char fieldA;   // +0x25
    char gap1[0x28-0x26];
    unsigned char fieldB;   // +0x28
    char gap2[0x2c-0x29];
    int fieldC;             // +0x2c
};

struct CPersistContext {
    void PersistA(unsigned char* p);
    void PersistB(unsigned char* p);
    void PersistC(int* p);
};

void __fastcall CSmashableDef_Transfer(CSmashableDef* self, int edx, CPersistContext* ctx)
{
    ctx->PersistA(&self->fieldA);
    ctx->PersistB(&self->fieldB);
    ctx->PersistC(&self->fieldC);
}