#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CEvasionEntityView {
    unsigned char _pad_0x00[0x6c];
    unsigned char CombatFlags;
    unsigned char _pad_0x6d[0x24];
    unsigned char StateFlags;
    unsigned char _pad_0x92[0x2a];
    unsigned char HitFlags;
};

struct CGameScriptInterfaceEvasionVTable {
    void* Slots[11];
    CEvasionEntityView* (__fastcall* GetEntity)(CGameScriptInterface*);
};

void __stdcall CGameScriptInterface_EntitySetEvadeAllHits(
    CGameScriptInterface* self, char value)
{
    CGameScriptInterfaceEvasionVTable* vtable =
        (CGameScriptInterfaceEvasionVTable*)self->__vftable;
    CEvasionEntityView* entity = vtable->GetEntity(self);
    if (entity == 0) return;
    if (entity->StateFlags & 1) return;
    if (!(entity->CombatFlags & 2)) return;
    unsigned char bit = (unsigned char)value;
    bit <<= 3;
    bit ^= entity->HitFlags;
    bit &= 8;
    entity->HitFlags ^= bit;
}
