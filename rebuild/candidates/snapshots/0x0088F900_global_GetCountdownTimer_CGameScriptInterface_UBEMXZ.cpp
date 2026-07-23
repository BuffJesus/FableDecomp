#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    float __thiscall GetCountdownTimer() const;
};

struct CountdownTimerSourceOverlay
{
    std::byte pad_0000[0xA0];
    std::int32_t countdownTimer;
};

static_assert(offsetof(CountdownTimerSourceOverlay, countdownTimer) == 0xA0);

float __thiscall CGameScriptInterface::GetCountdownTimer() const
{
    const auto source = *reinterpret_cast<CountdownTimerSourceOverlay* const*>(
        reinterpret_cast<std::uintptr_t>(this) + 0x4);

    return static_cast<float>(source->countdownTimer);
}