#include <cstdio>

struct StrKey {
    void* p;
    StrKey() { p = 0; }
    StrKey(const StrKey& o) { p = o.p; }
    ~StrKey() {}
};

struct Pair { void* first; void* second; };

static int g_calls = 0;
static void* g_a = 0;
static void* g_b = 0;

struct Registry {
    Pair* lookup(const StrKey* k) {
        (void)k;
        static Pair theResult;
        theResult.first = (void*)0x11;
        theResult.second = (void*)0x22;
        return &theResult;
    }
};

struct Target {
    void doit(void* a, void* b) { g_calls++; g_a = a; g_b = b; }
};

Registry g_registry;

struct CGameScriptInterface;

void __fastcall CGameScriptInterface_ClearGossipVillages(CGameScriptInterface* self, StrKey key)
{
    StrKey local(key);
    Pair* p = g_registry.lookup(&local);
    ((Target*)p)->doit(p->first, p->second);
}

int main()
{
    StrKey k;
    k.p = (void*)0xdead;
    CGameScriptInterface_ClearGossipVillages((CGameScriptInterface*)0, k);
    if (g_calls == 1 && g_a == (void*)0x11 && g_b == (void*)0x22) {
        std::printf("CGameScriptInterface_008aa270_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", g_calls);
    return 1;
}