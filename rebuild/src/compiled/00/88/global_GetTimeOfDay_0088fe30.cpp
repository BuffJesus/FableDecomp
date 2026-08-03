#include <cstddef>

class TimeOfDay
{
public:
    long GetRoundedHour();
};

class TimeManager
{
public:
    unsigned char pad_0[0x1C];
    TimeOfDay* pTimeOfDay;
};

class CGameScriptInterface
{
public:
    unsigned char pad_0[0x4];
    TimeManager* pTimeManager;

    long GetTimeOfDay() const;
};

long CGameScriptInterface::GetTimeOfDay() const
{
    return pTimeManager->pTimeOfDay->GetRoundedHour();
}