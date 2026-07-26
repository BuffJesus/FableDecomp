struct CFactionThing {
    void AddNeutral(CFactionThing* other);
};
struct CFactionMgr {
    CFactionThing* Lookup(int factionId);
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
    CFactionMgr* mgr1 = self->mid->m54;
    CFactionThing* a = mgr1->Lookup(faction1);
    CFactionMgr* mgr2 = self->mid->m54;
    CFactionThing* b = mgr2->Lookup(faction2);
    if (a && b) {
        a->AddNeutral(b);
        b->AddNeutral(a);
    }
}