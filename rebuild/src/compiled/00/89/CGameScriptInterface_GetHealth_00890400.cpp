#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

extern const float g_defaultHealth;

struct CHealthEntityView {
    unsigned char _pad_0x00[0x6c];
    unsigned char CombatFlags;
    unsigned char _pad_0x6d[0x24];
    unsigned char StateFlags;
    unsigned char _pad_0x92[0x22];
    float Health;
};

typedef CHealthEntityView* (__fastcall *GetHealthEntityFn)(CGameScriptInterface*);
struct CGameScriptInterfaceHealthVTable {
    void* Slots[11];
    GetHealthEntityFn GetEntity;
};

float __stdcall CGameScriptInterface_GetHealth(CGameScriptInterface* self)
{
    CGameScriptInterfaceHealthVTable* vtable =
        (CGameScriptInterfaceHealthVTable*)self->__vftable;
    CHealthEntityView* entity = vtable->GetEntity(self);
    if (entity != 0 &&
        (entity->StateFlags & 1) == 0 &&
        (entity->CombatFlags & 2) != 0) {
        return entity->Health;
    }
    return g_defaultHealth;
}
