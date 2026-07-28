#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
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

using GossipFactionLipSyncMap = std::map<
    CCharString,
    CArray<CMeshDataBank::CLipSyncEntry>,
    std::less<CCharString>,
    std::allocator<std::pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry>>>>;

extern GossipFactionLipSyncMap DAT_013bae5c;

class CGameScriptInterface
{
public:
    void __thiscall _AddGossipFactions_impl(struct CCharStringByValue gossipFaction);
};

namespace
{
struct CCharStringByValue
{
    std::uint32_t m_Raw;
};

static_assert(sizeof(CCharStringByValue) == sizeof(CCharString));

struct GossipFactionMapHeaderOverlay
{
    std::uint32_t m_HeadNode; // 0x00
};

static_assert(offsetof(GossipFactionMapHeaderOverlay, m_HeadNode) == 0x00);

using BinaryTree_SearchFn =
    std::uint32_t(__thiscall*)(GossipFactionLipSyncMap* pThis, const CCharString* pKey);

using GossipFactionInsertPostFn =
    void(__thiscall*)(CArray<CMeshDataBank::CLipSyncEntry>* pThis);

using CCharString_DestroyFn =
    void(__thiscall*)(CCharString* pThis);

inline auto* const BinaryTree_Search =
    reinterpret_cast<BinaryTree_SearchFn>(0x008AC200);

inline auto* const CALL_00432EE9 =
    reinterpret_cast<GossipFactionInsertPostFn>(0x00432EE9);

inline auto* const CCharString__dtor =
    reinterpret_cast<CCharString_DestroyFn>(0x0099EAE0);
}

void __thiscall CGameScriptInterface::_AddGossipFactions_impl(CCharStringByValue gossipFaction)
{
    auto* const gossipFactionString =
        reinterpret_cast<CCharString*>(&gossipFaction);

    std::uint32_t foundNode = BinaryTree_Search(&DAT_013bae5c, gossipFactionString);
    if (foundNode ==
        reinterpret_cast<const GossipFactionMapHeaderOverlay*>(&DAT_013bae5c)->m_HeadNode)
    {
        CArray<CMeshDataBank::CLipSyncEntry>& entries =
            DAT_013bae5c[*gossipFactionString];
        CALL_00432EE9(&entries);
    }

    CCharString__dtor(gossipFactionString);
}