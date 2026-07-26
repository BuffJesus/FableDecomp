#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    long __thiscall GetDayOfWeek() const;
};

namespace
{
struct DayOfWeekValueOverlay
{
    std::byte gap0[0x14];
    std::int32_t dayCount;
};

static_assert(offsetof(DayOfWeekValueOverlay, dayCount) == 0x14);

struct DayOfWeekNodeOverlay
{
    std::byte gap0[0x1c];
    const DayOfWeekValueOverlay* value;
};

static_assert(offsetof(DayOfWeekNodeOverlay, value) == 0x1c);

struct CGameScriptInterfaceOverlay
{
    std::byte gap0[4];
    const DayOfWeekNodeOverlay* node;
};

static_assert(offsetof(CGameScriptInterfaceOverlay, node) == 0x4);
}

long __thiscall CGameScriptInterface::GetDayOfWeek() const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    return self->node->value->dayCount % 7;
}