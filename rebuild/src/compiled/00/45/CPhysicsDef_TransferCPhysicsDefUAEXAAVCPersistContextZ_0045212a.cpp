#pragma optimize("s",on)
struct CPersistContext {
    void PersistFloat(void* p);
    void PersistOther(void* p);
    void PersistSub(void* p);
};

struct CPhysicsDef {
    unsigned char _pad[0x28];
    float f28;
    float f2c;
    float f30;
    int   pad34;
    int   pad38;
    int   pad3c;
    unsigned char sub[4];
};

void __fastcall CPhysicsDef_Transfer(CPhysicsDef* self, int edx, CPersistContext* ctx)
{
    ctx->PersistFloat(&self->f28);
    ctx->PersistFloat(&self->f2c);
    ctx->PersistFloat(&self->f30);
    ctx->PersistOther(&self->pad3c);
    ctx->PersistFloat((float*)&self->pad34);
    ctx->PersistFloat((float*)&self->pad38);
    ctx->PersistSub(&self->sub[0]);
}