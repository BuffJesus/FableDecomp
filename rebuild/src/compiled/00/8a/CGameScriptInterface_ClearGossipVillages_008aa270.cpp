// CGameScriptInterface::ClearGossipVillages @ 008aa270
// __fastcall, this unused; one by-value class parameter (4 bytes).

struct StrKey {
    void* p; // +0x0 (4 bytes)
    StrKey(const StrKey& o);  // copy ctor -> 0xf49c0
    ~StrKey();                // dtor    -> 0xf4870
};

struct Pair {
    void* first;  // +0x0
    void* second; // +0x4
};

struct Registry {
    Pair* lookup(const StrKey* k); // 0xfff04970 returns Pair*
};

struct Target {
    void doit(void* a, void* b); // 0xffb890fa
};

extern Registry g_registry; // 0x13bae50

struct CGameScriptInterface;

void __fastcall CGameScriptInterface_ClearGossipVillages(CGameScriptInterface* self, StrKey key)
{
    StrKey local(key);
    Pair* p = g_registry.lookup(&local);
    ((Target*)p)->doit(p->first, p->second);
}