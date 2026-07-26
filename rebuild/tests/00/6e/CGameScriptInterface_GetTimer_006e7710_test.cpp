
#include <cstdio>

struct Node { char pad[0x14]; long value; };

struct Iter { Node* p; };

struct TimerMap {
    Node* begin;
    Node* end;
    Iter LowerBound(const long* key);
};

struct CGameScriptInterface {
    char pad[0x3c];
    TimerMap map;
    long GetTimer(long key) const;
};

static Node g_nodes[2];
static Node* g_found = 0;

Iter TimerMap::LowerBound(const long* key)
{
    Iter r; r.p = g_found; return r;
}

long CGameScriptInterface::GetTimer(long key) const
{
    const TimerMap* m = &this->map;
    Iter it = ((TimerMap*)m)->LowerBound(&key);
    if (it.p != m->begin)
        return it.p->value;
    return 0;
}

int main()
{
    CGameScriptInterface gsi;
    gsi.map.begin = &g_nodes[0];
    gsi.map.end = &g_nodes[2];
    g_nodes[1].value = 12345;

    g_found = &g_nodes[0];
    if (gsi.GetTimer(7) != 0) { std::printf("FAIL begin\n"); return 1; }

    g_found = &g_nodes[1];
    if (gsi.GetTimer(7) != 12345) { std::printf("FAIL value\n"); return 1; }

    std::printf("CGameScriptInterface_006e7710_TEST PASS\n");
    return 0;
}