#include <cstdio>

struct Faction {
    int id; int enemies[8]; int nEnemies;
    void AddEnemy(Faction* other);
};
struct FactionMgr {
    Faction* list[16]; int n;
    Faction* Lookup(int id);
};
struct CMid { char pad[0x54]; FactionMgr* mgr; };
struct CGameScriptInterface { void* vt; CMid* mid; };

Faction* FactionMgr::Lookup(int id)
{
    for (int i = 0; i < n; ++i)
        if (list[i]->id == id) return list[i];
    return 0;
}

void Faction::AddEnemy(Faction* other)
{
    enemies[nEnemies++] = other->id;
}

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

int main()
{
    Faction f0; f0.id = 10; f0.nEnemies = 0;
    Faction f1; f1.id = 20; f1.nEnemies = 0;
    FactionMgr mgr; mgr.list[0] = &f0; mgr.list[1] = &f1; mgr.n = 2;
    CMid midObj; midObj.mgr = &mgr;
    CGameScriptInterface gsi; gsi.mid = &midObj;

    CGameScriptInterface_SetFactionAsEnemyToFaction(&gsi, 0, 10, 20);
    if (f0.nEnemies != 1 || f0.enemies[0] != 20) { std::printf("FAIL a\n"); return 1; }
    if (f1.nEnemies != 1 || f1.enemies[0] != 10) { std::printf("FAIL b\n"); return 1; }

    f0.nEnemies = 0; f1.nEnemies = 0;
    CGameScriptInterface_SetFactionAsEnemyToFaction(&gsi, 0, 10, 99);
    if (f0.nEnemies != 0 || f1.nEnemies != 0) { std::printf("FAIL c\n"); return 1; }

    std::printf("CGameScriptInterface_00890910_TEST PASS\n");
    return 0;
}