#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <utility>

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
class CArray
{
public:
    T* m_pBegin; // 0x00
    T* m_pEnd;   // 0x04
};

static_assert(offsetof(CArray<CMeshDataBank::CLipSyncEntry>, m_pBegin) == 0x00);
static_assert(offsetof(CArray<CMeshDataBank::CLipSyncEntry>, m_pEnd) == 0x04);
static_assert(sizeof(CArray<CMeshDataBank::CLipSyncEntry>) == 0x08);

using GossipVillageMap = std::map<
    CCharString,
    CArray<CMeshDataBank::CLipSyncEntry>,
    std::less<CCharString>,
    std::allocator<std::pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry>>>>;

extern GossipVillageMap DAT_013BAE50;

class CGameScriptInterface
{
public:
    void __thiscall _AddGossipVillages_impl(struct CCharStringByValue gossipVillage);
};

namespace
{
struct CCharStringByValue
{
    std::uint32_t m_Raw; // ABI carrier for by-value CCharString without automatic destruction
};

static_assert(sizeof(CCharStringByValue) == sizeof(CCharString));

struct GossipVillageMapHeaderOverlay
{
    std::uint32_t m_HeadNode; // 0x00
};

static_assert(offsetof(GossipVillageMapHeaderOverlay, m_HeadNode) == 0x00);

using BinaryTree_SearchFn =
    std::uint32_t(__thiscall*)(GossipVillageMap* pThis, const CCharString* pKey);

using CCharString_DestroyFn =
    void(__thiscall*)(CCharString* pThis);

using GossipVillageInsertPostFn =
    void(__thiscall*)(CArray<CMeshDataBank::CLipSyncEntry>* pThis);

inline auto* const BinaryTree_Search =
    reinterpret_cast<BinaryTree_SearchFn>(0x008AC200);

inline auto* const CCharString__dtor =
    reinterpret_cast<CCharString_DestroyFn>(0x0099EAE0);

inline auto* const CALL_00432EE9 =
    reinterpret_cast<GossipVillageInsertPostFn>(0x00432EE9);
}

void __thiscall CGameScriptInterface::_AddGossipVillages_impl(CCharStringByValue gossipVillage)
{
    auto* const pGossipVillage =
        reinterpret_cast<CCharString*>(&gossipVillage);

    const std::uint32_t searchResult =
        BinaryTree_Search(&DAT_013BAE50, pGossipVillage);

    if (searchResult ==
        reinterpret_cast<const GossipVillageMapHeaderOverlay*>(&DAT_013BAE50)->m_HeadNode)
    {
        CArray<CMeshDataBank::CLipSyncEntry>& entries =
            DAT_013BAE50[*pGossipVillage];
        CALL_00432EE9(&entries);
    }

    CCharString__dtor(pGossipVillage);
}