#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Faction {
    void AddAlly(Faction* other);
};
struct FactionMgr {
    Faction* GetFaction(int id);
};
struct CWorld {
    unsigned char _pad[0x54];
    FactionMgr* mgr; // +0x54
};

void __fastcall CGameScriptInterface_SetFactionAsAlliedToFaction(CGameScriptInterface* self, int edx, int faction1, int faction2)
{
    FactionMgr* m = self->World->mgr;
    Faction* a = m->GetFaction(faction1);
    FactionMgr* m2 = self->World->mgr;
    Faction* b = m2->GetFaction(faction2);
    if (a && b) {
        a->AddAlly(b);
        b->AddAlly(a);
    }
}