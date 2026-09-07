#pragma optimize("s",on)
// CCarriedReadableDef::Copy  0x004e157c
#include "engine/CCarriedReadableDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase { int dummy; };


// base Copy(other) — stdcall callee taking other on the stack (cleans its own arg)
extern "C" void __stdcall BaseCopy_431f10(CDefClassBase const* other);

void __fastcall CCarriedReadableDef_Copy(CCarriedReadableDef* self, void* /*edx*/, CDefClassBase const* other)
{
    BaseCopy_431f10(other);
    const CCarriedReadableDef* o = (const CCarriedReadableDef*)other;
    self->TitleTextTag_TablePos = o->TitleTextTag_TablePos;
    self->BodyTextTag_TablePos = o->BodyTextTag_TablePos;
}