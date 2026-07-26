#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;

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

class CGameScriptInterface
{
public:
    float __thiscall GetHeroAttractiveness() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroAttractiveness_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
    std::byte m_Pad18[0x84];
    float m_Field9C; // 0x9C
};
static_assert(offsetof(CGameScriptInterface_GetHeroAttractiveness_SelfOverlay, m_Field14) == 0x14);
static_assert(offsetof(CGameScriptInterface_GetHeroAttractiveness_SelfOverlay, m_Field9C) == 0x9C);

struct CGameScriptInterface_GetHeroAttractiveness_TCInterfaceEntryOverlay
{
    std::int32_t m_Key; // 0x00
    CTCBase* m_pValue;  // 0x04
};
static_assert(offsetof(CGameScriptInterface_GetHeroAttractiveness_TCInterfaceEntryOverlay, m_pValue) == 0x04);

struct CGameScriptInterface_GetHeroAttractiveness_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetHeroAttractiveness_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CGameScriptInterface_GetHeroAttractiveness_TargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(CGameScriptInterface_GetHeroAttractiveness_TargetOverlay, m_TCInterfaceMapStorage) == 0x44);
static_assert(offsetof(CGameScriptInterface_GetHeroAttractiveness_TargetOverlay, m_pTCInterfaceMapEnd) == 0x48);
static_assert(offsetof(CGameScriptInterface_GetHeroAttractiveness_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_GetHeroAttractiveness_InterfaceOverlay
{
    std::byte m_Pad00[0x9C];
    float m_Field9C; // 0x9C
};
static_assert(offsetof(CGameScriptInterface_GetHeroAttractiveness_InterfaceOverlay, m_Field9C) == 0x9C);

using CGameScriptInterface_GetHeroAttractiveness_Helper00449970Fn =
    void*(__thiscall*)(void*);
using CGameScriptInterface_GetHeroAttractiveness_Helper00487DD0Fn =
    CGameScriptInterface_GetHeroAttractiveness_TargetOverlay*(__thiscall*)(void*);

auto* const CGameScriptInterface_GetHeroAttractiveness_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_GetHeroAttractiveness_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_GetHeroAttractiveness_Helper00487DD0 =
    reinterpret_cast<CGameScriptInterface_GetHeroAttractiveness_Helper00487DD0Fn>(0x00487DD0);
auto* const DAT_0122DEDC =
    reinterpret_cast<const float*>(0x0122DEDC);
}

float __thiscall CGameScriptInterface::GetHeroAttractiveness() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroAttractiveness_SelfOverlay*>(this);

    auto* const target =
        CGameScriptInterface_GetHeroAttractiveness_Helper00487DD0(
            CGameScriptInterface_GetHeroAttractiveness_Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags20 & 0x10) != 0)
        {
            std::int32_t interfaceTypeValue = 0x04;

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_GetHeroAttractiveness_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x04))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            return reinterpret_cast<
                const CGameScriptInterface_GetHeroAttractiveness_InterfaceOverlay*>(
                pEntry->m_pValue)->m_Field9C;
        }

        return self->m_Field9C;
    }

    return *DAT_0122DEDC;
}