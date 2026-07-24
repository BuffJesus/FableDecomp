#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
};

static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

class CGameScriptInterface
{
public:
    long __thiscall GetHeroWillEnergy() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroWillEnergy_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(offsetof(CGameScriptInterface_GetHeroWillEnergy_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroWillEnergy_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    TCInterfaceMap m_TCInterfaceMap; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CGameScriptInterface_GetHeroWillEnergy_TargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(CGameScriptInterface_GetHeroWillEnergy_TargetOverlay, m_TCInterfaceMap) == 0x44);
static_assert(offsetof(CGameScriptInterface_GetHeroWillEnergy_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_GetHeroWillEnergy_InterfaceOverlay
{
    std::byte m_Pad00[0x58];
    long m_WillEnergy; // 0x58
};

static_assert(
    offsetof(CGameScriptInterface_GetHeroWillEnergy_InterfaceOverlay, m_WillEnergy) == 0x58);

using Fn_0040D959 = void* (__thiscall*)(void*);
using Fn_00488DC2 =
    CGameScriptInterface_GetHeroWillEnergy_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_0040D959 CALL_0040D959 =
    reinterpret_cast<Fn_0040D959>(0x0040D959);
inline constexpr Fn_00488DC2 CALL_00488DC2 =
    reinterpret_cast<Fn_00488DC2>(0x00488DC2);
} // namespace

long __thiscall CGameScriptInterface::GetHeroWillEnergy() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroWillEnergy_SelfOverlay*>(this);
    auto* const target = CALL_00488DC2(CALL_0040D959(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        std::int32_t interfaceTypeValue = 0x04;

        auto* pEntry = target->m_TCInterfaceMap.LowerBound(
            reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue));

        if ((pEntry == target->m_TCInterfaceMap.m_pEnd) || (4 < pEntry->m_Key))
        {
            pEntry = target->m_TCInterfaceMap.m_pEnd;
        }

        return reinterpret_cast<const CGameScriptInterface_GetHeroWillEnergy_InterfaceOverlay*>(
                   pEntry->m_Value)
            ->m_WillEnergy;
    }

    return 0;
}