#include <cstddef>
#include <cstdint>
#include <cmath>

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_HeroStats = 4,
};

class CTCBase
{
};

class CTCHeroStats : public CTCBase
{
public:
    long __thiscall GetMoralityMax();
    void __thiscall SetMorality(int morality);

    std::byte pad_0x00[0x28];
    int Morality;
};

static_assert(offsetof(CTCHeroStats, Morality) == 0x28);

template <typename K, typename V, typename Compare>
class CVectorMap
{
public:
    struct CEntry
    {
        K Key;
        V Value;
    };

    CEntry* Begin;
    CEntry* End;

    CEntry* __thiscall LowerBound(const K* key);
};

struct CKeyPairCompareLess_ETCInterfaceType_CTCBasePtr
{
};

using THCBaseMap = CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess_ETCInterfaceType_CTCBasePtr>;

struct MoralityTargetOverlay
{
    std::byte pad_0x00[0x20];
    std::uint8_t Flags20;
    std::byte pad_0x21[0x44 - 0x21];
    THCBaseMap InterfaceMap;
    std::byte pad_0x4C[0x91 - 0x4C];
    std::uint8_t Flags91;
};

static_assert(offsetof(MoralityTargetOverlay, Flags20) == 0x20);
static_assert(offsetof(MoralityTargetOverlay, InterfaceMap) == 0x44);
static_assert(offsetof(MoralityTargetOverlay, Flags91) == 0x91);

struct ScriptInterfaceContext14;

extern ScriptInterfaceContext14* __thiscall sub_00449970(ScriptInterfaceContext14* context);
extern MoralityTargetOverlay* __thiscall sub_00487db0(ScriptInterfaceContext14* value);

class CGameScriptInterface
{
public:
    void __thiscall GiveHeroMorality(float amount) const;

private:
    std::byte pad_0x00[0x14];
    ScriptInterfaceContext14* Context14;
};

static_assert(offsetof(CGameScriptInterface, Context14) == 0x14);

void __thiscall CGameScriptInterface::GiveHeroMorality(float amount) const
{
    MoralityTargetOverlay* const first_target = sub_00487db0(sub_00449970(this->Context14));
    if (first_target != nullptr && (first_target->Flags91 & 0x01) == 0)
    {
        MoralityTargetOverlay* const second_target = sub_00487db0(sub_00449970(this->Context14));
        CTCHeroStats* hero_stats = reinterpret_cast<CTCHeroStats*>(second_target);

        if ((second_target->Flags20 & 0x10) != 0)
        {
            ETCInterfaceType interface_type = ETCInterfaceType_HeroStats;
            THCBaseMap::CEntry* entry = second_target->InterfaceMap.LowerBound(&interface_type);
            if (entry == second_target->InterfaceMap.End || interface_type < entry->Key)
            {
                entry = second_target->InterfaceMap.End;
            }

            hero_stats = static_cast<CTCHeroStats*>(entry->Value);
        }

        const long morality_max = hero_stats->GetMoralityMax();
        const float scaled_delta = static_cast<float>(morality_max) * amount;
        const int rounded_delta = static_cast<int>(std::lrintf(scaled_delta));
        hero_stats->SetMorality(hero_stats->Morality + rounded_delta);
    }
}