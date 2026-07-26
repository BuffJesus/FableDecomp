#include <cstdio>
struct GStr {
    void* p;
    GStr() : p(0) {}
    GStr(const GStr& o) : p(o.p) {}
    ~GStr() {}
};
static void* g_consumed = 0;
struct GGlobal {
    void Consume(const GStr& s) { g_consumed = s.p; }
};
GGlobal g_gossipMgr;
void __fastcall CGameScriptInterface_RemoveGossipFactions(void* self, GStr arg)
{
    (void)self;
    GStr tmp(arg);
    g_gossipMgr.Consume(tmp);
}
int main() {
    GStr s; s.p = (void*)0xABCD;
    CGameScriptInterface_RemoveGossipFactions((void*)0, s);
    if (g_consumed == (void*)0xABCD) { std::printf("CGameScriptInterface_008aa510_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}