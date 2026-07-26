#include <map>
#include <cstdio>

struct CCharString
{
    int v;
    CCharString() : v(0) {}
    CCharString(int x) : v(x) {}
    CCharString(const CCharString& o) : v(o.v) {}
    ~CCharString() {}
    bool operator<(const CCharString& o) const { return v < o.v; }
};

template <typename T>
struct CArray
{
    int m_pFirst;
    int m_pLast;
};

struct CLipSyncEntry;

std::map<CCharString, CArray<CLipSyncEntry> > DAT_013BAE5C;

int GetGossipFactionsSize(CCharString gossipFactions)
{
    CCharString key(gossipFactions);
    CArray<CLipSyncEntry>& entries = DAT_013BAE5C[key];
    return (entries.m_pLast - entries.m_pFirst) >> 2;
}

int main()
{
    CArray<CLipSyncEntry> a;
    a.m_pFirst = 0x1000;
    a.m_pLast = 0x1000 + 4 * 7;
    DAT_013BAE5C[CCharString(5)] = a;

    int n = GetGossipFactionsSize(CCharString(5));
    if (n != 7) { std::printf("FAIL got=%d\n", n); return 1; }

    int empty = GetGossipFactionsSize(CCharString(99));
    if (empty != 0) { std::printf("FAIL empty got=%d\n", empty); return 1; }

    std::printf("CGameScriptInterface_008aa480_TEST PASS\n");
    return 0;
}