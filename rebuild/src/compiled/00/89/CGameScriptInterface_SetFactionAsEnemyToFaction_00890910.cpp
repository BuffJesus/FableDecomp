#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Faction {
    void AddEnemy(Faction* other);
};
struct FactionMgr {
    Faction* Lookup(int id);
};
struct CWorld { char pad[0x54]; FactionMgr* mgr; };

void __fastcall CGameScriptInterface_SetFactionAsEnemyToFaction(CGameScriptInterface* self, int dummy, int a, int b)
{
    FactionMgr* m = self->World->mgr;
    Faction* fa = m->Lookup(a);
    FactionMgr* m2 = self->World->mgr;
    Faction* fb = m2->Lookup(b);
    if (fa && fb) {
        fa->AddEnemy(fb);
        fb->AddEnemy(fa);
    }
}