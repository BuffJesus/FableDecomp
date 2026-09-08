#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified

struct CCreatureActionUpdateTarget;
struct CCreatureActionBaseVTable {
    void (__fastcall *Slots[64])(void*);
};

extern "C" CCreatureActionUpdateTarget* __fastcall getSub(void* creaturePointer);
extern "C" void __fastcall runSub(CCreatureActionUpdateTarget* target);

void __fastcall Update(CCreatureActionBase* self)
{
    if (self->Finished) {
        CCreatureActionUpdateTarget* target = getSub((void*)self->PCreature);
        runSub(target);
    } else {
        CCreatureActionBaseVTable* vtable =
            (CCreatureActionBaseVTable*)self->__vftable;
        vtable->Slots[0xe0 / 4](self);
    }
}
