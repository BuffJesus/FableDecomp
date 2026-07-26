#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCInventoryClothing;

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

class CTCInventoryClothing
{
public:
    void __thiscall RemoveAllClothing();
};

class CGameScriptInterface
{
public:
    void __thiscall RemoveHeroClothing() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_RemoveHeroClothing_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_RemoveHeroClothing_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_RemoveHeroClothing_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_RemoveHeroClothing_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_RemoveHeroClothing_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint32_t m_Flags20; // 0x20
    std::byte m_Pad24[0x20];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_RemoveHeroClothing_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_RemoveHeroClothing_TargetOverlay, m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_RemoveHeroClothing_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_RemoveHeroClothing_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_RemoveHeroClothing_TargetOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_RemoveHeroClothing_Helper0040D959Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_RemoveHeroClothing_Helper00487DC0Fn =
    CGameScriptInterface_RemoveHeroClothing_TargetOverlay* (__thiscall*)(void*);

auto* const CGameScriptInterface_RemoveHeroClothing_Helper0040D959 =
    reinterpret_cast<CGameScriptInterface_RemoveHeroClothing_Helper0040D959Fn>(0x0040D959);
auto* const CGameScriptInterface_RemoveHeroClothing_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_RemoveHeroClothing_Helper00487DC0Fn>(0x00487DC0);
}

void __thiscall CGameScriptInterface::RemoveHeroClothing() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_RemoveHeroClothing_SelfOverlay*>(this);
    auto* const target =
        CGameScriptInterface_RemoveHeroClothing_Helper00487DC0(
            CGameScriptInterface_RemoveHeroClothing_Helper0040D959(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x00040000) != 0))
    {
        std::int32_t interfaceTypeValue = 0x12;

        auto* pEntry =
            reinterpret_cast<CGameScriptInterface_RemoveHeroClothing_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x12))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        reinterpret_cast<CTCInventoryClothing*>(pEntry->m_pInterface)->RemoveAllClothing();
    }
}