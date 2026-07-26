struct Faction {
    void AddEnemy(Faction* other);
};
struct FactionMgr {
    Faction* Lookup(int id);
};
struct CMid { char pad[0x54]; FactionMgr* mgr; };
struct CGameScriptInterface { void* vt; CMid* mid; };

void __fastcall CGameScriptInterface_SetFactionAsEnemyToFaction(CGameScriptInterface* self, int dummy, int a, int b)
{
    FactionMgr* m = self->mid->mgr;
    Faction* fa = m->Lookup(a);
    FactionMgr* m2 = self->mid->mgr;
    Faction* fb = m2->Lookup(b);
    if (fa && fb) {
        fa->AddEnemy(fb);
        fb->AddEnemy(fa);
    }
}