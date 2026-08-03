namespace NUISystem
{
class CObserver;

class CObservable
{
public:
    void AddExclusiveObserver(CObserver* pObserver);
};
}

extern int DAT_01375550;

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