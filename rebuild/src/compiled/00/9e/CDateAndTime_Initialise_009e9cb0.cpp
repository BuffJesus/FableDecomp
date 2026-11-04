// CDateAndTime::Initialise(const FILETIME&) @ 0x009E9CB0
// (FableWin ?Initialise@CDateAndTime@@QAEXABU_FILETIME@@@Z).
//
// Converts the FILETIME with FileTimeToSystemTime and copies the SYSTEMTIME
// fields (skipping the day of week). The conversion result is not checked.

#include "fable_file_information.h"

typedef int FableBool;

// winbase.h SYSTEMTIME.
struct FableSystemTime
{
    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDayOfWeek;
    unsigned short wDay;
    unsigned short wHour;
    unsigned short wMinute;
    unsigned short wSecond;
    unsigned short wMilliseconds;
};

extern "C" __declspec(dllimport) FableBool FABLE_STDCALL FileTimeToSystemTime(
    const FILETIME* fileTime,
    FableSystemTime* systemTime);

void CDateAndTime::Initialise(const FILETIME& fileTime)
{
    FableSystemTime systemTime;
    FileTimeToSystemTime(&fileTime, &systemTime);
    Year = systemTime.wYear;
    Month = systemTime.wMonth;
    Day = systemTime.wDay;
    Hour = systemTime.wHour;
    Minute = systemTime.wMinute;
    Second = systemTime.wSecond;
    Millisecond = systemTime.wMilliseconds;
}