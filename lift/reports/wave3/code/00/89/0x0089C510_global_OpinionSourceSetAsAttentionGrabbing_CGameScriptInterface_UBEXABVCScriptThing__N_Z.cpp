#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
struct CKeyValuePair
{
    TKey m_Key;
    TValue m_Value;
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyValuePair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

namespace
{
using TTCInterfaceMap = CVectorMap<
    ETCInterfaceType,
    CTCBase*,
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

using TTCInterfaceEntry = CKeyValuePair<ETCInterfaceType, CTCBase*>;

struct CGameScriptInterface_VTableOverlay
{
    std::byte m_Pad00[0x2C];
    void* (__thiscall* m_RawSlot2C)(const CGameScriptInterface* pThis); // 0x2C
};

static_assert(offsetof(CGameScriptInterface_VTableOverlay, m_RawSlot2C) == 0x2C);

struct COpinionSourceOverlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags; // 0x28
    std::byte m_Pad2C[0x18];
    TTCInterfaceMap m_InterfaceMap;     // 0x44
    TTCInterfaceEntry* m_pInterfaceEnd; // 0x48
};

static_assert(offsetof(COpinionSourceOverlay, m_Flags) == 0x28);
static_assert(offsetof(COpinionSourceOverlay, m_InterfaceMap) == 0x44);
static_assert(offsetof(COpinionSourceOverlay, m_pInterfaceEnd) == 0x48);

struct CTCBase_Overlay
{
    std::byte m_Pad00[0x77];
    std::uint8_t m_bAttentionGrabbing; // 0x77
};

static_assert(offsetof(CTCBase_Overlay, m_bAttentionGrabbing) == 0x77);
}

class CGameScriptInterface
{
public:
    void __thiscall OpinionSourceSetAsAttentionGrabbing(
        const CScriptThing& scriptThing,
        bool bAttentionGrabbing) const
    {
        (void)scriptThing;

        const auto* const pVTable =
            *reinterpret_cast<const CGameScriptInterface_VTableOverlay* const*>(this);
        auto* const pOpinionSource =
            static_cast<COpinionSourceOverlay*>(pVTable->m_RawSlot2C(this));

        if (pOpinionSource != nullptr &&
            (pOpinionSource->m_Flags & 0x10000u) != 0)
        {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x50);
            TTCInterfaceEntry* pEntry =
                pOpinionSource->m_InterfaceMap.LowerBound(&interfaceType);

            if (pEntry == pOpinionSource->m_pInterfaceEnd ||
                static_cast<std::int32_t>(interfaceType) < static_cast<std::int32_t>(pEntry->m_Key))
            {
                pEntry = pOpinionSource->m_pInterfaceEnd;
            }

            reinterpret_cast<CTCBase_Overlay*>(pEntry->m_Value)->m_bAttentionGrabbing =
                static_cast<std::uint8_t>(bAttentionGrabbing);
        }
    }
};