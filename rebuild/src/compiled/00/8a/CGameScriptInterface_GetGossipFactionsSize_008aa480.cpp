#include <map>

class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();
    bool operator<(const CCharString& other) const;
};

class CMeshDataBank
{
public:
    struct CLipSyncEntry;
};

template <typename T>
struct CArray
{
    int m_pFirst;
    int m_pLast;
};

class CGameScriptInterface
{
public:
    virtual int GetGossipFactionsSize(CCharString gossipFactions);
};

extern std::map<CCharString, CArray<CMeshDataBank::CLipSyncEntry> > DAT_013BAE5C;

int CGameScriptInterface::GetGossipFactionsSize(CCharString gossipFactions)
{
    CCharString key(gossipFactions);
    CArray<CMeshDataBank::CLipSyncEntry>& entries = DAT_013BAE5C[key];
    return (entries.m_pLast - entries.m_pFirst) >> 2;
}