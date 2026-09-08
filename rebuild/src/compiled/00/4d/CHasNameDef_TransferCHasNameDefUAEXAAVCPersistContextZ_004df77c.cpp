#pragma optimize("s",on)
#include "engine/CHasNameDef.h"

struct CPersistContext {
    void TransferA(int* v);
    void TransferB(int* v);
};

void __fastcall CHasNameDef_Transfer(CHasNameDef* self, void* /*edx*/, CPersistContext* ctx)
{
    ctx->TransferA((int*)&self->DefaultNameTag);
    ctx->TransferB((int*)&self->Home);
    ctx->TransferB((int*)&self->Occupation);
}
