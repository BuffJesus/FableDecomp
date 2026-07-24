#include <cstddef>
#include <cstdint>

class CScriptThing;

class CGameScriptInterface
{
public:
    float GetHealth(const CScriptThing& thing) const;
};

struct CGameScriptInterface_VTable
{
    std::byte pad_0x00[0x2C];
    void* slot_0x2C;
};

static_assert(offsetof(CGameScriptInterface_VTable, slot_0x2C) == 0x2C);

struct HealthSource_Overlay
{
    std::byte pad_0x00[0x6C];
    std::uint8_t flags_0x6C;
    std::byte pad_0x6D[0x24];
    std::uint8_t flags_0x91;
    std::byte pad_0x92[0x22];
    float health_0xB4;
};

static_assert(offsetof(HealthSource_Overlay, flags_0x6C) == 0x6C);
static_assert(offsetof(HealthSource_Overlay, flags_0x91) == 0x91);
static_assert(offsetof(HealthSource_Overlay, health_0xB4) == 0xB4);

extern const float _DAT_0122dedc;

float CGameScriptInterface::GetHealth(const CScriptThing& thing) const
{
    (void)thing;

    using Slot_0x2C_Type = HealthSource_Overlay* (__thiscall*)(const CGameScriptInterface*);

    const auto* const vftable =
        *reinterpret_cast<CGameScriptInterface_VTable* const*>(this);
    const auto get_source =
        reinterpret_cast<Slot_0x2C_Type>(vftable->slot_0x2C);

    HealthSource_Overlay* const source = get_source(this);
    if ((source != nullptr) &&
        ((source->flags_0x91 & 0x01) == 0) &&
        ((source->flags_0x6C & 0x02) != 0))
    {
        return source->health_0xB4;
    }

    return _DAT_0122dedc;
}