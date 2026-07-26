#include <cstddef>
#include <cstdint>

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

namespace std
{
template <typename TKey, typename TValue>
class map
{
public:
    TValue& __thiscall operator[](const TKey& key);
};
}

class CGameScriptInterface
{
public:
    int __thiscall GetGossipFactionsSize(CCharString scriptName);
};

namespace
{
inline constexpr auto g_GossipFactions =
    reinterpret_cast<std::map<CCharString, CArray<CMeshDataBank::CLipSyncEntry>>*>(0x013BAE5C);
}

int __thiscall CGameScriptInterface::GetGossipFactionsSize(CCharString scriptName)
{
    CCharString localScriptName(scriptName);
    CArray<CMeshDataBank::CLipSyncEntry>* const pEntries =
        &(*g_GossipFactions)[localScriptName];

    const std::int32_t first = pEntries->m_pFirst;
    const std::int32_t last = pEntries->m_pLast;
    return (last - first) >> 2;
}