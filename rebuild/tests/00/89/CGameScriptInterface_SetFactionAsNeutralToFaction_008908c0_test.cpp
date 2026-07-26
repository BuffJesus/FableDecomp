#include <cstdio>

struct CFactionThing {
    int id;
    CFactionThing* neutralTo;
    void AddNeutral(CFactionThing* other) { neutralTo = other; }
};
static CFactionThing g_factions[4];
struct CFactionMgr {
    CFactionThing* Lookup(int factionId) {
        if (factionId < 0 || factionId >= 4) return 0;
        if (factionId == 2) return 0; // simulate a miss
        return &g_factions[factionId];
    }
};
struct CMid {
    unsigned char _pad[0x54];
    CFactionMgr* m54; // +0x54
};
struct CGameScriptInterface {
    void* vt;
    CMid* mid; // +0x4
};

void __fastcall CGameScriptInterface_SetFactionAsNeutralToFaction(CGameScriptInterface* self, void* edxpad, int faction1, int faction2)
{
    CFactionThing* a = self->mid->m54->Lookup(faction1);
    CFactionThing* b = self->mid->m54->Lookup(faction2);
    if (a && b) {
        a->AddNeutral(b);
        b->AddNeutral(a);
    }
}

int main()
{
    for (int i = 0; i < 4; ++i) { g_factions[i].id = i; g_factions[i].neutralTo = 0; }
    CFactionMgr mgr;
    CMid midObj;
    midObj.m54 = &mgr;
    CGameScriptInterface gsi;
    gsi.mid = &midObj;

    // both valid -> mutual neutral
    CGameScriptInterface_SetFactionAsNeutralToFaction(&gsi, 0, 0, 1);
    if (g_factions[0].neutralTo != &g_factions[1]) { std::printf("FAIL a\n"); return 1; }
    if (g_factions[1].neutralTo != &g_factions[0]) { std::printf("FAIL b\n"); return 1; }

    // second lookup miss -> no mutation
    g_factions[3].neutralTo = 0;
    CGameScriptInterface_SetFactionAsNeutralToFaction(&gsi, 0, 3, 2);
    if (g_factions[3].neutralTo != 0) { std::printf("FAIL c\n"); return 1; }

    std::printf("CGameScriptInterface_008908c0_TEST PASS\n");
    return 0;
}