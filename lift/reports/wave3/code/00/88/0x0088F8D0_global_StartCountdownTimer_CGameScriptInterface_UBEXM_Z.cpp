#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

namespace NUISystem
{
class CObserver;

class CObservable
{
public:
    void __thiscall AddExclusiveObserver(CObserver* pObserver);
};
}

float __cdecl ROUND(float value);

namespace
{
extern std::int32_t DAT_01375550;

struct CGameScriptInterface_StartCountdownTimer_SelfOverlay
{
    std::byte m_Pad00[0x04];
    NUISystem::CObservable* m_pObservable; // 0x04
};
static_assert(
    offsetof(CGameScriptInterface_StartCountdownTimer_SelfOverlay, m_pObservable) == 0x04);
}

class CGameScriptInterface
{
public:
    void __thiscall StartCountdownTimer(float durationSeconds) const;
};

void __thiscall CGameScriptInterface::StartCountdownTimer(float durationSeconds) const
{
    auto* const pSelf =
        reinterpret_cast<const CGameScriptInterface_StartCountdownTimer_SelfOverlay*>(this);

    const float product = static_cast<float>(DAT_01375550) * durationSeconds;
    const std::int32_t observerValue = static_cast<std::int32_t>(ROUND(product));

    pSelf->m_pObservable->AddExclusiveObserver(
        reinterpret_cast<NUISystem::CObserver*>(observerValue));
}