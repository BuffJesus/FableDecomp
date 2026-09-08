#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CScriptEntityStateGroupView {
    unsigned char _pad_0x00[0x10];
    char Enabled;
    void SetStategroup(int stateGroup, int enabled);
};

struct CGameScriptInterfaceEntityVTable {
    void* slots[0x2c / 4];
    CScriptEntityStateGroupView* (__fastcall* GetEntity)(CGameScriptInterface*);
};

void __stdcall CGameScriptInterface_EntitySetStategroupEnabled(
    CGameScriptInterface* self, int stateGroup, int enabled)
{
    CGameScriptInterfaceEntityVTable* vtable =
        (CGameScriptInterfaceEntityVTable*)self->__vftable;
    CScriptEntityStateGroupView* entity = vtable->GetEntity(self);
    if (entity && entity->Enabled == 1) {
        entity->SetStategroup(stateGroup, enabled);
    }
}
