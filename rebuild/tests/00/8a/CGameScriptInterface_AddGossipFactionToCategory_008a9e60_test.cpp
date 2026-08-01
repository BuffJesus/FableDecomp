#include <cstdio>

struct CCharStringData { int refs; };

class CCharString
{
public:
    CCharStringData* m_pData;
    CCharString() { m_pData = 0; }
    CCharString(CCharStringData* d) { m_pData = d; }
    CCharString(const CCharString& other) { m_pData = other.m_pData; g_ctor++; }
    ~CCharString() {}
    static int g_ctor;
};
int CCharString::g_ctor = 0;

class CGameScriptInterface
{
public:
    virtual void AddGossipFactionToCategory(const CCharString& category, const CCharString& faction) const;
};

static int g_insert = 0;
void AddGossipFactionToCategory_Impl(CCharString category, CCharString faction)
{
    (void)category; (void)faction;
    g_insert++;
}

void CGameScriptInterface::AddGossipFactionToCategory(const CCharString& category, const CCharString& faction) const
{
    AddGossipFactionToCategory_Impl(category, faction);
}

int main()
{
    CCharStringData da; da.refs = 1;
    CCharStringData db; db.refs = 2;
    CCharString cat(&da);
    CCharString fac(&db);
    CGameScriptInterface gsi;
    gsi.AddGossipFactionToCategory(cat, fac);
    if (g_insert == 1 && CCharString::g_ctor == 2) {
        printf("GOSSIP_ADD_OK\n");
        return 0;
    }
    printf("FAIL insert=%d ctor=%d\n", g_insert, CCharString::g_ctor);
    return 1;
}