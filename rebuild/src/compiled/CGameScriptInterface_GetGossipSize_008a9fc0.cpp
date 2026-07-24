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
    int GetGossipSize(CCharString gossip);
};

extern std::map<CCharString, CArray<CMeshDataBank::CLipSyncEntry> > DAT_013BAE44;

int CGameScriptInterface::GetGossipSize(CCharString gossip)
{
    CCharString key(gossip);
    CArray<CMeshDataBank::CLipSyncEntry>& entries = DAT_013BAE44[key];
    return (entries.m_pLast - entries.m_pFirst) >> 2;
}