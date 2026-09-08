#pragma optimize("s",on)
// CSmashableDef::Copy @ 0x00453842
#include "engine/CSmashableDef.h"  // retyped onto the PDB layout; byte parity re-verified

// Base copy (CBaseDef::Copy @ 0x431f10) is invoked on self+0. The generated
// header flattens the base subobject, so model the base call through a cast
// to a local CBaseDef declaring only the member.
struct CBaseDef {
    void Copy(const CBaseDef* src);
};

void __fastcall CSmashableDef_Copy(CSmashableDef* self, void* edx, const CSmashableDef* src)
{
    ((CBaseDef*)self)->Copy((const CBaseDef*)src);
    self->Smashable = src->Smashable;
    self->ReplacementObject_Val = src->ReplacementObject_Val;
    self->SmashParticleEmitter = src->SmashParticleEmitter;
}
