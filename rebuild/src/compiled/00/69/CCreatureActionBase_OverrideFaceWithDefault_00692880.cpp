#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimationEntry; extern unsigned int __fastcall probe(CAnimationEntry*);
bool __fastcall OverrideFaceWithDefault(CCreatureActionBase* self){ return (probe(self->PAnimEntry_Data) >> 4) & 1; }