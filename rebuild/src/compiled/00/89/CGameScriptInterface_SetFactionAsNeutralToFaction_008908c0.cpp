#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CFactionThing {
    void AddNeutral(CFactionThing* other);
};
struct CFactionMgr {
    CFactionThing* Lookup(int factionId);
};
struct CWorld {
    unsigned char _pad[0x54];
    CFactionMgr* m54; // +0x54
};

void __fastcall CGameScriptInterface_SetFactionAsNeutralToFaction(CGameScriptInterface* self, void* edxpad, int faction1, int faction2)
{
    CFactionMgr* mgr1 = self->World->m54;
    CFactionThing* a = mgr1->Lookup(faction1);
    CFactionMgr* mgr2 = self->World->m54;
    CFactionThing* b = mgr2->Lookup(faction2);
    if (a && b) {
        a->AddNeutral(b);
        b->AddNeutral(a);
    }
}