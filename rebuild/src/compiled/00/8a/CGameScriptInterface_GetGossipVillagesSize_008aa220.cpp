// CGameScriptInterface::GetGossipVillagesSize @ 008aa220
// __fastcall int GetGossipVillagesSize(CCharString name) ; name by value

struct CCharString {
    void* p;
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CArrayHdr {
    int* first; // +0
    int* last;  // +4
};

struct GossipMap {
    CArrayHdr& op_index(const CCharString& key);
};

extern GossipMap g_gossipVillagesMap; // at 0x13bae50 (reloc-masked)

int __fastcall CGameScriptInterface_GetGossipVillagesSize(void* self, CCharString name)
{
    CCharString key(name);
    CArrayHdr& e = g_gossipVillagesMap.op_index(key);
    int first = (int)e.first;
    int last = (int)e.last;
    return (last - first) >> 2;
}