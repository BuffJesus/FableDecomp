#pragma optimize("s",on)
#include "engine/CSmashableDef.h"  // retyped onto the PDB layout; byte parity re-verified

struct CPersistContext {
    void PersistA(unsigned char* p);
    void PersistB(unsigned char* p);
    void PersistC(int* p);
};

void __fastcall CSmashableDef_Transfer(CSmashableDef* self, int edx, CPersistContext* ctx)
{
    ctx->PersistA(reinterpret_cast<unsigned char*>(&self->Smashable));
    ctx->PersistB(reinterpret_cast<unsigned char*>(&self->ReplacementObject_Val));
    ctx->PersistC(reinterpret_cast<int*>(&self->SmashParticleEmitter));
}
