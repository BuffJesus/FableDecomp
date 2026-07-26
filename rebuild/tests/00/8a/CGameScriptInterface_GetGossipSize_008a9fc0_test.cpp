#include <cstdio>
#include <map>

// Standalone reproduction of CGameScriptInterface::GetGossipSize.
// A CCharString is modeled as a 4-byte handle (pointer to interned string)
// so it is comparable and copyable, matching retail's by-value semantics.

class CCharString
{
public:
    int h;
    CCharString(int v) : h(v) {}
    CCharString(const CCharString& o) : h(o.h) {}
    bool operator<(const CCharString& o) const { return h < o.h; }
};

template <typename T>
struct CArray
{
    int m_pFirst;
    int m_pLast;
};

struct CLipSyncEntry;

std::map<CCharString, CArray<CLipSyncEntry> > g_gossip;

int GetGossipSize(CCharString gossip)
{
    CCharString key(gossip);
    CArray<CLipSyncEntry>& entries = g_gossip[key];
    return (entries.m_pLast - entries.m_pFirst) >> 2;
}

int main()
{
    // Insert a vector-like range: 5 elements of 4 bytes each => size 5.
    CArray<CLipSyncEntry> a;
    a.m_pFirst = 0x1000;
    a.m_pLast  = 0x1000 + 5 * 4;
    g_gossip[CCharString(7)] = a;

    int existing = GetGossipSize(CCharString(7));
    // Missing key => default-constructs {0,0} => size 0.
    int missing = GetGossipSize(CCharString(99));

    if (existing == 5 && missing == 0)
    {
        std::printf("CGameScriptInterface_008a9fc0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL existing=%d missing=%d\n", existing, missing);
    return 1;
}