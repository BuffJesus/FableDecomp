#pragma optimize("s",on)
#include "engine/CDecapitationDef.h"

struct CMember { int x; };
struct CPersistContext { void Persist(CMember* m); };

void __fastcall CDecapitationDef_Transfer(CDecapitationDef* self, void* /*edx*/, CPersistContext* ctx)
{
    ctx->Persist((CMember*)&self->BodyParticleEffect);
    ctx->Persist((CMember*)&self->Head);
}
