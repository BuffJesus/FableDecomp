#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CEngagementEntityView {
    unsigned char _pad_0x00[0x6c];
    unsigned char CombatFlags;
    unsigned char _pad_0x6d[0x91 - 0x6d];
    unsigned char StateFlags;
    unsigned char _pad_0x92[0xbc - 0x92];
    unsigned char EngagementFlags;
};

struct CGameScriptInterfaceEngagementVTable {
    void* Slots[11];
    CEngagementEntityView* (__fastcall *GetEntity)(void* self);
};

void __stdcall CGameScriptInterface_EntitySetAbleToBeEngagedInCombat(
    CGameScriptInterface* self, char enable)
{
    CGameScriptInterfaceEngagementVTable* vtable =
        (CGameScriptInterfaceEngagementVTable*)self->__vftable;
    CEngagementEntityView* entity = vtable->GetEntity(self);
    if (!entity) return;
    if (entity->StateFlags & 1) return;
    if (!(entity->CombatFlags & 2)) return;
    unsigned char bit = (unsigned char)(enable << 5);
    entity->EngagementFlags ^=
        (unsigned char)((bit ^ entity->EngagementFlags) & 0x20);
}
