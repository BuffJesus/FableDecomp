// CGameScriptInterface::SetActiveGossipCategories @ 008a9720
// (Key key /*by value*/, char active) on stack, callee-clean ret 0x8

struct Key {
    char* p;
    Key(const Key& o);   // copy ctor -> f5510
    ~Key();              // dtor      -> f53c0
};

struct GossipMap {
    char* lookup(Key* k);  // -> 47f0, this=&g_gossipMap, k pushed (thiscall)
};
extern GossipMap g_gossipMap; // 0x13bae2c

void __stdcall CGameScriptInterface_SetActiveGossipCategories(Key key, char active)
{
    Key local(key);
    char* dst = g_gossipMap.lookup(&local);
    *dst = active;
}