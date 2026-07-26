struct Faction {
    void AddAlly(Faction* other);
};
struct FactionMgr {
    Faction* GetFaction(int id);
};
struct CMid {
    unsigned char _pad[0x54];
    FactionMgr* mgr; // +0x54
};
struct CGameScriptInterface {
    void* vt;      // +0x0
    CMid* mid;     // +0x4
};

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