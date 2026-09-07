#include "engine/CTCCombat.h"  // retyped onto the PDB layout; byte parity re-verified
struct CThingCreatureBase { char pad[0x9c]; char flag; };
extern CThingCreatureBase* __fastcall GetSub(CTCCombat* o);

char __fastcall CTCCombat_IsAbleToBeKnockedDown(char* self)
{
    CThingCreatureBase* s = GetSub(*(CTCCombat**)(self + 0x24));
    return s->flag != 0;
}