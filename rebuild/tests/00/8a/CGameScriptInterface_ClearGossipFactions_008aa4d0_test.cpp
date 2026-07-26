#include <cstdio>

struct CLipSyncEntry;

struct CCharString {
    void* p;
    int* refcount;
    CCharString() : p(0), refcount(0) {}
    CCharString(const CCharString& other) : p(other.p), refcount(other.refcount) { if (refcount) (*refcount)++; }
    ~CCharString() { if (refcount) (*refcount)--; }
};

struct CArray {
    CLipSyncEntry* m_pBegin;
    CLipSyncEntry* m_pEnd;
    int erased;
    void EraseRange(CLipSyncEntry* pFirst, CLipSyncEntry* pLast) {
        erased = (int)((char*)pLast - (char*)pFirst);
        m_pEnd = m_pBegin;
    }
};

struct GossipFactionLipSyncMap {
    CArray* stored;
    const CCharString* lastKey;
    CArray& operator[](const CCharString& key) { lastKey = &key; return *stored; }
};

GossipFactionLipSyncMap DAT_013bae5c;

struct CGameScriptInterface { void* vt; };

void __fastcall CGameScriptInterface_ClearGossipFactions(CGameScriptInterface* self, CCharString gossipFaction)
{
    CCharString gossipFactionCopy(gossipFaction);
    CArray& entries = DAT_013bae5c[gossipFactionCopy];
    entries.EraseRange(entries.m_pBegin, entries.m_pEnd);
}

int main() {
    CArray arr;
    arr.m_pBegin = (CLipSyncEntry*)16;
    arr.m_pEnd   = (CLipSyncEntry*)64;
    arr.erased = -1;
    DAT_013bae5c.stored = &arr;
    DAT_013bae5c.lastKey = 0;

    int rc = 1;
    CCharString key; key.p = (void*)0xABCD; key.refcount = &rc;

    CGameScriptInterface obj; obj.vt = 0;
    CGameScriptInterface_ClearGossipFactions(&obj, key);

    if (arr.erased != 48) { std::printf("FAIL erased=%d\n", arr.erased); return 1; }
    if (arr.m_pEnd != arr.m_pBegin) { std::printf("FAIL end not reset\n"); return 1; }
    if (DAT_013bae5c.lastKey == 0) { std::printf("FAIL key not passed\n"); return 1; }
    if (rc != 1) { std::printf("FAIL refcount=%d\n", rc); return 1; }

    std::printf("CGameScriptInterface_008aa4d0_TEST PASS\n");
    return 0;
}