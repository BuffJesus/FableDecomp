#include <cstdio>

static int g_ctorCount = 0;
static int g_dtorCount = 0;
static int g_removeCount = 0;
static void* g_seenPtr = 0;

struct GString {
    void* p;
    GString() { p = 0; }
    GString(const GString& o) { p = o.p; g_ctorCount++; }
    ~GString() { g_dtorCount++; }
};

struct Gossip {
    void RemoveGossip(GString& g) { g_removeCount++; g_seenPtr = g.p; }
};

Gossip g_gossipObj;

void __fastcall CGameScriptInterface_RemoveGossip(GString gossip)
{
    GString tmp(gossip);
    g_gossipObj.RemoveGossip(tmp);
}

int main()
{
    GString seed;
    seed.p = (void*)0x1234;
    g_ctorCount = 0; g_dtorCount = 0; g_removeCount = 0;
    CGameScriptInterface_RemoveGossip(seed);
    // one copy for pass-by-value param, one copy into tmp => >=2 ctor, >=2 dtor
    if (g_removeCount == 1 && g_seenPtr == (void*)0x1234 &&
        g_ctorCount >= 2 && g_dtorCount >= 2) {
        std::printf("CGameScriptInterface_008aa050_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL remove=%d ptr=%p ctor=%d dtor=%d\n",
        g_removeCount, g_seenPtr, g_ctorCount, g_dtorCount);
    return 1;
}