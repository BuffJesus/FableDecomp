#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CCombatToggleEntityView {
    unsigned char _pad_0x00[0x6c];
    unsigned char CombatFlags;
    unsigned char _pad_0x6d[0x24];
    unsigned char StateFlags;
    unsigned char _pad_0x92[0x2a];
    unsigned char HitFlags;
};

struct CGameScriptInterfaceCombatVTable {
    unsigned char _pad_0x00[0x2c];
    CCombatToggleEntityView* (__fastcall *GetEntity)(void* self);
};

void __stdcall CGameScriptInterface_EntitySetNegateAllHits(
    CGameScriptInterface* self, unsigned char value)
{
    CGameScriptInterfaceCombatVTable* vtable =
        (CGameScriptInterfaceCombatVTable*)self->__vftable;
    CCombatToggleEntityView* entity = vtable->GetEntity(self);
    if (!entity) return;
    if (entity->StateFlags & 1) return;
    if (!(entity->CombatFlags & 2)) return;
    unsigned char bit = (unsigned char)(value << 4);
    bit ^= entity->HitFlags;
    bit &= 0x10;
    entity->HitFlags ^= bit;
}
