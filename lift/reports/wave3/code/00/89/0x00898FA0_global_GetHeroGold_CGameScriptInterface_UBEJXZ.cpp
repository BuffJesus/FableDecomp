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
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

class CGameScriptInterface
{
public:
    long __thiscall GetHeroGold() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterfaceOverlay
{
    std::byte pad_0x00[0x14];
    void* field_0x14;
};

static_assert(offsetof(CGameScriptInterfaceOverlay, field_0x14) == 0x14);

struct HeroOverlay
{
    std::byte pad_0x00[0x20];
    std::uint8_t flags_0x20;
    std::byte pad_0x21[0x44 - 0x21];
    std::byte tcInterfaceMapStorage[8];
    std::byte pad_0x4C[0x91 - 0x4C];
    std::uint8_t flags_0x91;
};

static_assert(offsetof(HeroOverlay, flags_0x20) == 0x20);
static_assert(offsetof(HeroOverlay, tcInterfaceMapStorage) == 0x44);
static_assert(offsetof(HeroOverlay, flags_0x91) == 0x91);

struct TCInterfaceEntryOverlay
{
    std::int32_t key;
    CTCBase* value;
};

static_assert(offsetof(TCInterfaceEntryOverlay, key) == 0x0);
static_assert(offsetof(TCInterfaceEntryOverlay, value) == 0x4);
static_assert(sizeof(TCInterfaceEntryOverlay) == 0x8);

struct GoldSourceOverlay
{
    std::byte pad_0x00[0x3C];
    long gold;
};

static_assert(offsetof(GoldSourceOverlay, gold) == 0x3C);

using Fn00449970 = void*(__thiscall*)(void*);
using Fn00487DC0 = HeroOverlay*(__thiscall*)(void*);

constexpr auto sub_00449970 = reinterpret_cast<Fn00449970>(0x00449970);
constexpr auto sub_00487DC0 = reinterpret_cast<Fn00487DC0>(0x00487DC0);
} // namespace

long __thiscall CGameScriptInterface::GetHeroGold() const
{
    const auto* const pThis = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    void* const intermediate = sub_00449970(pThis->field_0x14);
    HeroOverlay* const hero = sub_00487DC0(intermediate);
    if (hero == nullptr) {
        return 0;
    }

    if (((hero->flags_0x91 & 0x01u) == 0) && ((hero->flags_0x20 & 0x10u) != 0)) {
        const auto interfaceType = static_cast<ETCInterfaceType>(4);
        auto* const tcInterfaceMap =
            reinterpret_cast<TCInterfaceMap*>(hero->tcInterfaceMapStorage);
        auto* entry = static_cast<TCInterfaceEntryOverlay*>(
            TCInterfaceMap::LowerBound(tcInterfaceMap, &interfaceType));
        auto* const end =
            *reinterpret_cast<TCInterfaceEntryOverlay**>(hero->tcInterfaceMapStorage + 0x4);

        if ((entry == end) || (entry->key > 4)) {
            entry = end;
        }

        return reinterpret_cast<GoldSourceOverlay*>(entry->value)->gold;
    }

    return 0;
}