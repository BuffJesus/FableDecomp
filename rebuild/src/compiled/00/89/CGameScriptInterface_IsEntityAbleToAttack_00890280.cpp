#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CAttackEntityView {
    void* __vftable;
    unsigned char _pad_0x04[0x91 - 4];
    unsigned char Flags;
};

typedef CAttackEntityView* (__fastcall* GetAttackEntityFn)(CGameScriptInterface*);
extern char __fastcall Entity_CanAttack(CAttackEntityView* self);

bool __stdcall CGameScriptInterface_IsEntityAbleToAttack(CGameScriptInterface* self)
{
    GetAttackEntityFn* vtable = (GetAttackEntityFn*)self->__vftable;
    CAttackEntityView* entity = vtable[0x2c / 4](self);
    if (entity != 0 && (entity->Flags & 1) == 0)
        return Entity_CanAttack(entity);
    return false;
}
