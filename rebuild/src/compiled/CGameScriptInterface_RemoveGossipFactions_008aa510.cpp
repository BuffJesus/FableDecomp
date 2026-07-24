// CGameScriptInterface::RemoveGossipFactions @ 008aa510
struct GStr {
    void* p;
    GStr(const GStr& o);
    ~GStr();
};
struct GGlobal {
    void Consume(const GStr& s);
};
extern GGlobal g_gossipMgr;
void __fastcall CGameScriptInterface_RemoveGossipFactions(void* self, GStr arg)
{
    (void)self;
    GStr tmp(arg);
    g_gossipMgr.Consume(tmp);
}