#include <cstdio>

struct CCharString {
    void* p;
    CCharString() { p = 0; }
    CCharString(const CCharString& o) { p = o.p; }
    ~CCharString() {}
};

struct CArrayHdr {
    int* first;
    int* last;
};

static int g_backing[8];

struct GossipMap {
    CArrayHdr hdr;
    CArrayHdr& op_index(const CCharString& key) { (void)key; return hdr; }
};

static GossipMap g_gossipVillagesMap;

int __fastcall CGameScriptInterface_GetGossipVillagesSize(void* self, CCharString name)
{
    CCharString key(name);
    CArrayHdr& e = g_gossipVillagesMap.op_index(key);
    int first = (int)e.first;
    int last = (int)e.last;
    return (last - first) >> 2;
}

int main()
{
    // 5 elements, each 4 bytes apart -> (5*4)>>2 = 5
    g_gossipVillagesMap.hdr.first = &g_backing[0];
    g_gossipVillagesMap.hdr.last  = &g_backing[5];
    CCharString s; s.p = 0;
    int r = CGameScriptInterface_GetGossipVillagesSize((void*)0, s);
    if (r != 5) { std::printf("FAIL got %d\n", r); return 1; }

    g_gossipVillagesMap.hdr.last = &g_backing[0];
    r = CGameScriptInterface_GetGossipVillagesSize((void*)0, s);
    if (r != 0) { std::printf("FAIL got %d\n", r); return 1; }

    std::printf("CGameScriptInterface_008aa220_TEST PASS\n");
    return 0;
}