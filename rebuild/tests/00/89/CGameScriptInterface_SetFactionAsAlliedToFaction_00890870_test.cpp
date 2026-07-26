#include <cstdio>

struct Faction;
struct FactionMgr;

static int g_add_calls = 0;
static Faction* g_last_this[8];
static Faction* g_last_arg[8];

struct Faction {
    int id;
    void AddAlly(Faction* other);
};
struct FactionMgr {
    Faction* fa;
    Faction* fb;
    Faction* GetFaction(int id);
};
struct CMid {
    unsigned char _pad[0x54];
    FactionMgr* mgr;
};
struct CGameScriptInterface {
    void* vt;
    CMid* mid;
};

void Faction::AddAlly(Faction* other) {
    if (g_add_calls < 8) {
        g_last_this[g_add_calls] = this;
        g_last_arg[g_add_calls] = other;
    }
    g_add_calls++;
}

Faction* FactionMgr::GetFaction(int id) {
    if (id == 1) return fa;
    if (id == 2) return fb;
    return 0;
}

void __fastcall CGameScriptInterface_SetFactionAsAlliedToFaction(CGameScriptInterface* self, int edx, int faction1, int faction2)
{
    FactionMgr* m = self->mid->mgr;
    Faction* a = m->GetFaction(faction1);
    FactionMgr* m2 = self->mid->mgr;
    Faction* b = m2->GetFaction(faction2);
    if (a && b) {
        a->AddAlly(b);
        b->AddAlly(a);
    }
}

int main() {
    Faction fA; fA.id = 1;
    Faction fB; fB.id = 2;
    FactionMgr mgr; mgr.fa = &fA; mgr.fb = &fB;
    CMid midObj; midObj.mgr = &mgr;
    CGameScriptInterface gsi; gsi.mid = &midObj;

    // both present -> two AddAlly calls, cross referenced
    g_add_calls = 0;
    CGameScriptInterface_SetFactionAsAlliedToFaction(&gsi, 0, 1, 2);
    if (g_add_calls != 2) { std::printf("FAIL calls=%d\n", g_add_calls); return 1; }
    if (g_last_this[0] != &fA) { std::printf("FAIL c0t\n"); return 1; }
    if (g_last_arg[0] != &fB) { std::printf("FAIL c0a\n"); return 1; }
    if (g_last_this[1] != &fB) { std::printf("FAIL c1t\n"); return 1; }
    if (g_last_arg[1] != &fA) { std::printf("FAIL c1a\n"); return 1; }

    // one missing -> no calls
    g_add_calls = 0;
    CGameScriptInterface_SetFactionAsAlliedToFaction(&gsi, 0, 1, 99);
    if (g_add_calls != 0) { std::printf("FAIL missing=%d\n", g_add_calls); return 1; }

    std::printf("CGameScriptInterface_00890870_TEST PASS\n");
    return 0;
}