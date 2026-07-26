#include <cstdio>

struct Key {
    char* p;
    Key(char* q) : p(q) {}
    Key(const Key& o) : p(o.p) {}
    ~Key() {}
};

static char g_store[4];

struct GossipMap {
    char* lookup(Key* k);
};
char* GossipMap::lookup(Key* k) { (void)k; return g_store; }
GossipMap g_gossipMap;

void __stdcall CGameScriptInterface_SetActiveGossipCategories(Key key, char active)
{
    Key local(key);
    char* dst = g_gossipMap.lookup(&local);
    *dst = active;
}

int main() {
    g_store[0] = 0;
    char c = 'x';
    Key k(&c);
    CGameScriptInterface_SetActiveGossipCategories(k, (char)0x5A);
    if (g_store[0] == (char)0x5A) {
        std::printf("CGameScriptInterface_008a9720_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %d\n", (int)g_store[0]);
    return 1;
}