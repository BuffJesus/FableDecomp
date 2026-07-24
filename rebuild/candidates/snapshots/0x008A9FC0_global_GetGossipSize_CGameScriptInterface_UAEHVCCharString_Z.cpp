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
template <typename T>
class allocator;

template <typename T1, typename T2>
struct pair;

template <typename T>
struct less;

template <typename K, typename V, typename Compare, typename Allocator>
class map
{
public:
    V& operator[](const K& key);
};
}

class CGameScriptInterface
{
public:
    int __thiscall GetGossipSize(CCharString gossipName);
};

namespace
{
using TGossipMap = std::map<
    CCharString,
    CArray<CMeshDataBank::CLipSyncEntry>,
    std::less<CCharString>,
    std::allocator<std::pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry>>>>;

inline auto& g_GossipMap = *reinterpret_cast<TGossipMap*>(0x013BAE44);
}

int __thiscall CGameScriptInterface::GetGossipSize(CCharString gossipName)
{
    CCharString key(gossipName);
    CArray<CMeshDataBank::CLipSyncEntry>& entries = g_GossipMap[key];
    const int first = entries.m_pFirst;
    const int last = entries.m_pLast;
    return (last - first) >> 2;
}