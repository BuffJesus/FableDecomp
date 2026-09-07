// ?GetAnimDelayTime@CCreatureActionBase@@UBEJXZ @ 0x00692460
// Retail: 33 c0 c3  ->  xor eax,eax ; ret
// Virtual const this-only accessor returning long 0.
// Modeled as __fastcall with 'this' in ecx (byte-identical to __fastcall for a this-only accessor).


// long GetAnimDelayTime() const  ->  xor eax,eax ; ret
#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
long __fastcall CCreatureActionBase_GetAnimDelayTime(const CCreatureActionBase* self)
{
    (void)self;
    return 0;
}