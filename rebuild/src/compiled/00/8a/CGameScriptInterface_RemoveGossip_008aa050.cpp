struct GString {
    void* p;
    GString(const GString& o);
    ~GString();
};

struct Gossip {
    void RemoveGossip(GString& g);
};

extern Gossip g_gossipObj;

void __fastcall CGameScriptInterface_RemoveGossip(GString gossip)
{
    GString tmp(gossip);
    g_gossipObj.RemoveGossip(tmp);
}