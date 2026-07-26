#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;

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

class CGameScriptInterface
{
public:
    float __thiscall GetDistanceHeroCanBeHeardFrom() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_SelfOverlay
{
    std::byte m_Pad00[0x14];
    union
    {
        void* m_Field14;
        float m_Distance14;
    };
};
static_assert(offsetof(CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pValue;     // 0x04
};
static_assert(offsetof(CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TCInterfaceEntryOverlay, m_pValue) == 0x04);

struct CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x1C];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TargetOverlay, m_Flags24) == 0x24);
static_assert(offsetof(CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TargetOverlay, m_TCInterfaceMapStorage) == 0x44);
static_assert(offsetof(CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TargetOverlay, m_pTCInterfaceMapEnd) == 0x48);
static_assert(offsetof(CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_InterfaceOverlay
{
    std::byte m_Pad00[0x14];
    float m_Field14; // 0x14
};
static_assert(offsetof(CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_InterfaceOverlay, m_Field14) == 0x14);

using CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_Helper0040D959Fn =
    void*(__thiscall*)(void*);
using CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_Helper00487DE0Fn =
    CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TargetOverlay*(__thiscall*)(void*);

auto* const CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_Helper0040D959 =
    reinterpret_cast<CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_Helper0040D959Fn>(0x0040D959);
auto* const CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_Helper00487DE0 =
    reinterpret_cast<CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_Helper00487DE0Fn>(0x00487DE0);
auto* const DAT_0122DEDC =
    reinterpret_cast<const float*>(0x0122DEDC);
}

float __thiscall CGameScriptInterface::GetDistanceHeroCanBeHeardFrom() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_SelfOverlay*>(this);

    auto* const target =
        CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_Helper00487DE0(
            CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_Helper0040D959(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags24 & 0x00400000) != 0)
        {
            std::int32_t interfaceTypeValue = 0x36;

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x36))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            return reinterpret_cast<
                const CGameScriptInterface_GetDistanceHeroCanBeHeardFrom_InterfaceOverlay*>(
                pEntry->m_pValue)->m_Field14;
        }

        return self->m_Distance14;
    }

    return *DAT_0122DEDC;
}