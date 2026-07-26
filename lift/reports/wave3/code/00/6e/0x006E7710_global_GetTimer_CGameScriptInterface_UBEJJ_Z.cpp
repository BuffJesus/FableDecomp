#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    long __thiscall GetTimer(long timerId) const;
};

namespace
{
struct TimerTreeOverlay
{
    void __thiscall BinaryTree_FindLE_v2(void** ppFoundNode, const long* pTimerId) const;
};

struct CGameScriptInterface_GetTimer_Overlay
{
    std::byte pad_0[0x3C];
    void* pTimerTreeSentinel;
};

static_assert(offsetof(CGameScriptInterface_GetTimer_Overlay, pTimerTreeSentinel) == 0x3C);

struct TimerTreeNodeOverlay
{
    std::byte pad_0[0x14];
    std::int32_t timerValue;
};

static_assert(offsetof(TimerTreeNodeOverlay, timerValue) == 0x14);
} // namespace

long __thiscall CGameScriptInterface::GetTimer(long timerId) const
{
    void* foundNode = const_cast<CGameScriptInterface*>(this);

    reinterpret_cast<const TimerTreeOverlay*>(
        reinterpret_cast<const std::byte*>(this) + 0x3C)
        ->BinaryTree_FindLE_v2(&foundNode, &timerId);

    if (foundNode != reinterpret_cast<const CGameScriptInterface_GetTimer_Overlay*>(this)->pTimerTreeSentinel)
    {
        return reinterpret_cast<const TimerTreeNodeOverlay*>(foundNode)->timerValue;
    }

    return 0;
}