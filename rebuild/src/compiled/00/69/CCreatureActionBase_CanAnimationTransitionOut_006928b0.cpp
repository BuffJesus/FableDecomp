#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimationEntry; extern int __fastcall cat_probe(CAnimationEntry*);
bool __fastcall CanAnimationTransitionOut(CCreatureActionBase* self)
{
    return (~cat_probe(self->PAnimEntry_Data)) & 1;
}