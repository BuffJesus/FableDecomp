#include "fable_date_and_time.h"
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

CDateAndTime::CDateAndTime(const FILETIME& fileTime)
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