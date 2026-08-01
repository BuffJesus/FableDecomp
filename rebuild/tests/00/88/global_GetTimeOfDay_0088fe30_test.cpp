#include <cstddef>
#include <cstdio>

class TimeOfDay
{
public:
    long hourField;
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

static long g_calledHour = -1;

long TimeOfDay::GetRoundedHour()
{
    long hour = hourField;
    g_calledHour = hour;
    return hour;
}

int main()
{
    long fakeHour = 17;
    TimeOfDay tod;
    tod.hourField = fakeHour;

    TimeManager tm;
    tm.pTimeOfDay = &tod;

    CGameScriptInterface iface;
    iface.pTimeManager = &tm;

    long result = iface.GetTimeOfDay();

    if (result == fakeHour && g_calledHour == fakeHour)
    {
        printf("TEST_PASS_GETTIMEOFDAY_0088FE30\n");
        return 0;
    }
    printf("TEST_FAIL result=%ld expected=%ld\n", result, fakeHour);
    return 1;
}