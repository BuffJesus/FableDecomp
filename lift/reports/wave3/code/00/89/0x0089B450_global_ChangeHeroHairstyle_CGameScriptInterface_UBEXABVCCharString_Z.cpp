#include <cstddef>
#include <cstdint>

class CCharString;
class CDefinitionManager;
class CGameScriptInterface;
class CTCBase;
class CTCHeroAttachableAppearanceModifiers;

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

class CDefinitionManager
{
public:
    long __thiscall GetDefGlobalIndexFromName(const CCharString* name);
};

class CTCHeroAttachableAppearanceModifiers
{
public:
    void __thiscall AddAppearanceModifierWithReplace(long definitionIndex);
};

class CGameScriptInterface
{
public:
    void __thiscall ChangeHeroHairstyle(const CCharString& hairstyleName) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_ChangeHeroHairstyle_SelfOverlay
{
    std::byte m_Pad00[0x10];
    CDefinitionManager* m_pDefinitionManager; // 0x10
    void* m_Field14;                          // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_ChangeHeroHairstyle_SelfOverlay, m_pDefinitionManager) == 0x10);
static_assert(
    offsetof(CGameScriptInterface_ChangeHeroHairstyle_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_ChangeHeroHairstyle_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_ChangeHeroHairstyle_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_ChangeHeroHairstyle_TargetOverlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x18];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_ChangeHeroHairstyle_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_ChangeHeroHairstyle_TargetOverlay, m_Flags28) == 0x28);
static_assert(
    offsetof(
        CGameScriptInterface_ChangeHeroHairstyle_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_ChangeHeroHairstyle_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_ChangeHeroHairstyle_TargetOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_ChangeHeroHairstyle_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_ChangeHeroHairstyle_Helper00487DC0Fn =
    CGameScriptInterface_ChangeHeroHairstyle_TargetOverlay* (__thiscall*)(void*);

inline constexpr auto CGameScriptInterface_ChangeHeroHairstyle_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_ChangeHeroHairstyle_Helper00449970Fn>(0x00449970);
inline constexpr auto CGameScriptInterface_ChangeHeroHairstyle_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_ChangeHeroHairstyle_Helper00487DC0Fn>(0x00487DC0);
}

void __thiscall CGameScriptInterface::ChangeHeroHairstyle(
    const CCharString& hairstyleName) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_ChangeHeroHairstyle_SelfOverlay*>(this);
    auto* const target =
        CGameScriptInterface_ChangeHeroHairstyle_Helper00487DC0(
            CGameScriptInterface_ChangeHeroHairstyle_Helper00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags28 & 0x40000000) != 0))
    {
        ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(0x5E);

        auto* pEntry =
            reinterpret_cast<CGameScriptInterface_ChangeHeroHairstyle_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    &searchKey));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x5E < pEntry->m_Key))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        auto* const appearanceModifiers =
            reinterpret_cast<CTCHeroAttachableAppearanceModifiers*>(pEntry->m_pInterface);
        const long definitionIndex =
            self->m_pDefinitionManager->GetDefGlobalIndexFromName(&hairstyleName);

        if (definitionIndex > 0)
        {
            appearanceModifiers->AddAppearanceModifierWithReplace(definitionIndex);
        }
    }
}