struct CCharString {
    CCharString(const CCharString& other);
    ~CCharString();
    void* p;
};

struct CLipSyncEntry;

struct CArray {
    CLipSyncEntry* m_pBegin; // +0x0
    CLipSyncEntry* m_pEnd;   // +0x4
    void EraseRange(CLipSyncEntry* pFirst, CLipSyncEntry* pLast);
};

struct GossipFactionLipSyncMap {
    CArray& operator[](const CCharString& key);
};

extern GossipFactionLipSyncMap DAT_013bae5c;

struct CGameScriptInterface { void* vt; };

void __fastcall CGameScriptInterface_ClearGossipFactions(CGameScriptInterface* self, CCharString gossipFaction)
{
    CCharString gossipFactionCopy(gossipFaction);
    CArray& entries = DAT_013bae5c[gossipFactionCopy];
    entries.EraseRange(entries.m_pBegin, entries.m_pEnd);
}