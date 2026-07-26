#include <cstddef>
#include <cstdint>

class CBaseIntelligentPointer;
class CBaseObject;
class CGameScriptInterface;
class CTCBase;
class CThing;

enum ETCInterfaceType : int;

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

class CBaseObject
{
public:
    void __thiscall OnPropertyChanged(std::uint32_t propertyId);
};

class CBaseIntelligentPointer
{
public:
    static CBaseObject* __thiscall GetPItem(CBaseIntelligentPointer* pThis);
};

class CThing
{
public:
    static void __thiscall Kill(CThing* pThis, bool destroyImmediately);
};

class CGameScriptInterface
{
public:
    void __thiscall CancelHeroTeleportEffects() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_CancelHeroTeleportEffects_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(offsetof(CGameScriptInterface_CancelHeroTeleportEffects_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_CancelHeroTeleportEffects_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pValue;     // 0x04
};
static_assert(offsetof(CGameScriptInterface_CancelHeroTeleportEffects_TCInterfaceEntryOverlay, m_pValue) == 0x04);

struct CGameScriptInterface_CancelHeroTeleportEffects_TCInterfaceValueOverlay
{
    std::byte m_Pad00[0x30];
    CBaseIntelligentPointer m_Item; // 0x30
};
static_assert(offsetof(CGameScriptInterface_CancelHeroTeleportEffects_TCInterfaceValueOverlay, m_Item) == 0x30);

struct CGameScriptInterface_CancelHeroTeleportEffects_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x1C];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_CancelHeroTeleportEffects_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CGameScriptInterface_CancelHeroTeleportEffects_TargetOverlay, m_Flags24) == 0x24);
static_assert(offsetof(CGameScriptInterface_CancelHeroTeleportEffects_TargetOverlay, m_TCInterfaceMapStorage) == 0x44);
static_assert(offsetof(CGameScriptInterface_CancelHeroTeleportEffects_TargetOverlay, m_pTCInterfaceMapEnd) == 0x48);
static_assert(offsetof(CGameScriptInterface_CancelHeroTeleportEffects_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_CancelHeroTeleportEffects_ObjectOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CGameScriptInterface_CancelHeroTeleportEffects_ObjectOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_CancelHeroTeleportEffects_Helper00449970Fn =
    void*(__thiscall*)(void*);
using CGameScriptInterface_CancelHeroTeleportEffects_Helper00487DC0Fn =
    CGameScriptInterface_CancelHeroTeleportEffects_TargetOverlay*(__thiscall*)(void*);

auto* const CGameScriptInterface_CancelHeroTeleportEffects_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_CancelHeroTeleportEffects_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_CancelHeroTeleportEffects_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_CancelHeroTeleportEffects_Helper00487DC0Fn>(0x00487DC0);
}

void __thiscall CGameScriptInterface::CancelHeroTeleportEffects() const
{
    auto* const self =
        reinterpret_cast<const CGameScriptInterface_CancelHeroTeleportEffects_SelfOverlay*>(this);

    auto* const helperResult =
        CGameScriptInterface_CancelHeroTeleportEffects_Helper00449970(self->m_Field14);
    auto* const target =
        CGameScriptInterface_CancelHeroTeleportEffects_Helper00487DC0(helperResult);

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        reinterpret_cast<CBaseObject*>(target)->OnPropertyChanged(0xF9);

        if ((target->m_Flags24 & 0x00000200) != 0)
        {
            std::int32_t interfaceTypeValue = 0x29;

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_CancelHeroTeleportEffects_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x29))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            auto* const pInterface =
                reinterpret_cast<CGameScriptInterface_CancelHeroTeleportEffects_TCInterfaceValueOverlay*>(
                    pEntry->m_pValue);
            auto* const pObject = CBaseIntelligentPointer::GetPItem(&pInterface->m_Item);

            if (pObject != nullptr)
            {
                const auto* const objectOverlay =
                    reinterpret_cast<const CGameScriptInterface_CancelHeroTeleportEffects_ObjectOverlay*>(
                        pObject);

                if ((objectOverlay->m_Flags91 & 0x01) == 0)
                {
                    CThing::Kill(reinterpret_cast<CThing*>(pObject), false);
                }
            }
        }
    }
}