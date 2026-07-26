#include <cstddef>
#include <cstdint>
#include <map>

class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();
};

static_assert(sizeof(CCharString) == 0x04);

class CMeshDataBank
{
public:
    struct CLipSyncEntry;
};

template <typename T>
struct CArray
{
    std::int32_t m_pFirst;
    std::int32_t m_pLast;
};

static_assert(offsetof(CArray<CMeshDataBank::CLipSyncEntry>, m_pFirst) == 0x00);
static_assert(offsetof(CArray<CMeshDataBank::CLipSyncEntry>, m_pLast) == 0x04);

class CGameScriptInterface
{
public:
    int GetGossipVillagesSize(CCharString gossipVillages);
};

extern std::map<CCharString, CArray<CMeshDataBank::CLipSyncEntry>> DAT_013BAE50;

int CGameScriptInterface::GetGossipVillagesSize(CCharString gossipVillages)
{
    CCharString key(gossipVillages);
    CArray<CMeshDataBank::CLipSyncEntry>& entries = DAT_013BAE50[key];
    return (entries.m_pLast - entries.m_pFirst) >> 2;
}