#pragma optimize("s",on)
struct CPersistContext {
    void TransferA(void* p);
    void TransferB(void* p);
    void TransferC(void* p);
};
struct CMessageEventDef {
    char pad[0x3c];
    int f3c;
    int f40;
    int f44;
    int f48;
    int f4c;
};

void __fastcall CMessageEventDef_Transfer(CMessageEventDef* self, void* edx, CPersistContext* ctx) {
    ctx->TransferA(&self->f3c);
    ctx->TransferA(&self->f40);
    ctx->TransferA(&self->f44);
    ctx->TransferB(&self->f4c);
    ctx->TransferC(&self->f48);
}