#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    long __thiscall GetDayCount() const;
};

namespace
{
struct DayCountValueOverlay
{
    std::byte gap0[0x14];
    std::int32_t dayCount;
};

static_assert(offsetof(DayCountValueOverlay, dayCount) == 0x14);

struct DayCountNodeOverlay
{
    std::byte gap0[0x1c];
    const DayCountValueOverlay* value;
};

static_assert(offsetof(DayCountNodeOverlay, value) == 0x1c);

struct CGameScriptInterfaceOverlay
{
    std::byte gap0[4];
    const DayCountNodeOverlay* node;
};

static_assert(offsetof(CGameScriptInterfaceOverlay, node) == 4);
} // namespace

long __thiscall CGameScriptInterface::GetDayCount() const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    return self->node->value->dayCount;
}