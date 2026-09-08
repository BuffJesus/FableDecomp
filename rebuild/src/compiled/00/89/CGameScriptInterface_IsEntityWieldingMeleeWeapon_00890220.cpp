#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CWeaponEntityView {
    unsigned char _pad_0x00[0x91];
    unsigned char Flags;
};

struct CGameScriptInterfaceEntityVTable {
    void* slots[11];
    CWeaponEntityView* (__fastcall* GetEntity)(CGameScriptInterface* self);
};

extern "C" bool __fastcall CheckMeleeWield(CWeaponEntityView* entity);

bool __stdcall CGameScriptInterface_IsEntityWieldingMeleeWeapon(
    CGameScriptInterface* self)
{
    CGameScriptInterfaceEntityVTable* vtable =
        (CGameScriptInterfaceEntityVTable*)self->__vftable;
    CWeaponEntityView* entity = vtable->GetEntity(self);
    if (entity != 0 && (entity->Flags & 1) == 0)
        return CheckMeleeWield(entity);
    return false;
}
