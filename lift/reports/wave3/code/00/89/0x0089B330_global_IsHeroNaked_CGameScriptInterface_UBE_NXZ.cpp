#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCShop;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(
        CVectorMap* pThis,
        const TKey* pKey);
};

class CTCShop
{
public:
    bool __thiscall GetName();
};

class CGameScriptInterface
{
public:
    bool __thiscall IsHeroNaked() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_IsHeroNaked_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_IsHeroNaked_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_IsHeroNaked_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroNaked_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_IsHeroNaked_TargetOverlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Field28; // 0x28
    std::byte m_Pad2C[0x18];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_IsHeroNaked_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_IsHeroNaked_TargetOverlay, m_Field28) == 0x28);
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroNaked_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroNaked_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_IsHeroNaked_TargetOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_IsHeroNaked_Helper0040D959Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_IsHeroNaked_Helper00487DC0Fn =
    CGameScriptInterface_IsHeroNaked_TargetOverlay* (__thiscall*)(void*);

auto* const CGameScriptInterface_IsHeroNaked_Helper0040D959 =
    reinterpret_cast<CGameScriptInterface_IsHeroNaked_Helper0040D959Fn>(0x0040D959);
auto* const CGameScriptInterface_IsHeroNaked_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_IsHeroNaked_Helper00487DC0Fn>(0x00487DC0);
}

bool __thiscall CGameScriptInterface::IsHeroNaked() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsHeroNaked_SelfOverlay*>(this);
    auto* const target =
        CGameScriptInterface_IsHeroNaked_Helper00487DC0(
            CGameScriptInterface_IsHeroNaked_Helper0040D959(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Field28 & 0x40000000) != 0))
    {
        ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(0x5E);

        auto* pEntry =
            reinterpret_cast<CGameScriptInterface_IsHeroNaked_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    &searchKey));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x5E))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        return reinterpret_cast<CTCShop*>(pEntry->m_pInterface)->GetName();
    }

    return false;
}