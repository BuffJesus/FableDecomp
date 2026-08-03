#include <cstdio>

namespace NUISystem
{
class CObserver;

class CObservable
{
public:
    CObserver* m_pLastObserver;
    void AddExclusiveObserver(CObserver* pObserver) { m_pLastObserver = pObserver; }
};
}

int DAT_01375550 = 0;

__forceinline int ROUND(float value)
{
    int result;
    __asm { fld value }
    __asm { fistp result }
    return result;
}

class CGameScriptInterface
{
public:
    void StartCountdownTimer(float durationSeconds) const;
};

struct SelfOverlay
{
    char m_Pad00[0x04];
    NUISystem::CObservable* m_pObservable;
};

void CGameScriptInterface::StartCountdownTimer(float durationSeconds) const
{
    const SelfOverlay* const pSelf = reinterpret_cast<const SelfOverlay*>(this);
    const float product = static_cast<float>(DAT_01375550) * durationSeconds;
    const int observerValue = ROUND(product);
    pSelf->m_pObservable->AddExclusiveObserver(reinterpret_cast<NUISystem::CObserver*>(observerValue));
}

struct FakeSelf
{
    void* pad;
    NUISystem::CObservable* pObservable;
};

int main()
{
    NUISystem::CObservable obs;
    obs.m_pLastObserver = 0;
    FakeSelf self;
    self.pad = 0;
    self.pObservable = &obs;

    const CGameScriptInterface* iface = reinterpret_cast<const CGameScriptInterface*>(&self);

    DAT_01375550 = 30;
    iface->StartCountdownTimer(2.0f);
    long v1 = (long)obs.m_pLastObserver;
    if (v1 != 60) { printf("FAIL v1=%ld\n", v1); return 1; }

    DAT_01375550 = 30;
    iface->StartCountdownTimer(0.05f);
    long v2 = (long)obs.m_pLastObserver;
    if (v2 != 2) { printf("FAIL v2=%ld (round 1.5)\n", v2); return 1; }

    DAT_01375550 = 30;
    iface->StartCountdownTimer(0.0833333f);
    long v3 = (long)obs.m_pLastObserver;
    if (v3 != 2) { printf("FAIL v3=%ld\n", v3); return 1; }

    printf("ROUND_OK\n");
    return 0;
}