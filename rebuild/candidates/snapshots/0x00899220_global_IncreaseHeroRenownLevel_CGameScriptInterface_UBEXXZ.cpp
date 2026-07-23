#include <cstddef>
#include <cstdint>

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_HeroStats = 4,
};

class CGameScriptInterface;
class CTCBase;
class CTCHeroStats;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

class CTCBase
{
};

class CTCHeroStats
{
public:
    void __thiscall UpgradeRenownLevel();
};

class CGameScriptInterface
{
public:
    void __thiscall IncreaseHeroRenownLevel() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterfaceSelfOverlay
{
    std::byte pad_00[0x14];
    void* field_14; // 0x14
};

static_assert(offsetof(CGameScriptInterfaceSelfOverlay, field_14) == 0x14);

struct TCInterfaceEntryOverlay
{
    std::int32_t key;     // 0x00
    CTCBase* pValue;      // 0x04
};

static_assert(offsetof(TCInterfaceEntryOverlay, key) == 0x00);
static_assert(offsetof(TCInterfaceEntryOverlay, pValue) == 0x04);

struct TargetOverlay
{
    std::byte pad_00[0x20];
    std::uint8_t flags_20;           // 0x20
    std::byte pad_21[0x23];
    std::byte mapStorage_44[0x04];   // 0x44
    TCInterfaceEntryOverlay* mapEnd; // 0x48
    std::byte pad_4C[0x45];
    std::uint8_t flags_91;           // 0x91
};

static_assert(offsetof(TargetOverlay, flags_20) == 0x20);
static_assert(offsetof(TargetOverlay, mapStorage_44) == 0x44);
static_assert(offsetof(TargetOverlay, mapEnd) == 0x48);
static_assert(offsetof(TargetOverlay, flags_91) == 0x91);

using Helper00449970Fn = void*(__thiscall*)(void*);
using Helper00487DC0Fn = TargetOverlay*(__thiscall*)(void*);

constexpr auto Helper00449970 =
    reinterpret_cast<Helper00449970Fn>(0x00449970);
constexpr auto Helper00487DC0 =
    reinterpret_cast<Helper00487DC0Fn>(0x00487DC0);
}

void __thiscall CGameScriptInterface::IncreaseHeroRenownLevel() const
{
    auto* const savedThis = this;
    auto* const self = reinterpret_cast<const CGameScriptInterfaceSelfOverlay*>(this);
    auto* const target = Helper00487DC0(Helper00449970(self->field_14));

    if ((target != nullptr) && ((target->flags_91 & 0x01) == 0))
    {
        if ((target->flags_20 & 0x10) != 0)
        {
            ETCInterfaceType interfaceType = ETCInterfaceType_HeroStats;

            auto* pEntry = reinterpret_cast<TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->mapStorage_44),
                    &interfaceType));

            if ((pEntry == target->mapEnd) || (ETCInterfaceType_HeroStats < pEntry->key))
            {
                pEntry = target->mapEnd;
            }

            reinterpret_cast<CTCHeroStats*>(pEntry->pValue)->UpgradeRenownLevel();
            return;
        }

        reinterpret_cast<CTCHeroStats*>(const_cast<CGameScriptInterface*>(savedThis))
            ->UpgradeRenownLevel();
    }
}