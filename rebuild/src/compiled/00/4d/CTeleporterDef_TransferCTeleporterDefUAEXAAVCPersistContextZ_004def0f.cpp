#pragma optimize("s",on)
struct CPersistContext {
    void TransferA(int* v);
    void TransferB(int* v);
};

struct CTCNoiseDef {
    char pad[0x28];
    int a;
    int b;
    int c;
};

void __fastcall CTCNoiseDef_Transfer(CTCNoiseDef* self, void* edx, CPersistContext* ctx)
{
    ctx->TransferA(&self->a);
    ctx->TransferB(&self->b);
    ctx->TransferB(&self->c);
}