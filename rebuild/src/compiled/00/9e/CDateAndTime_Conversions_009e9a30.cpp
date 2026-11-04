#include "fable_date_and_time.h"
#include "fable_win32_time.h"
#include "fable_file_information.h"

extern "C" __declspec(dllimport) int FABLE_STDCALL SystemTimeToFileTime(
    const _SYSTEMTIME* systemTime, FILETIME* fileTime);

// Retail 009E9A30: copy the SYSTEMTIME fields, skipping the day of week.
CDateAndTime::CDateAndTime(const _SYSTEMTIME& systemTime)
    : Year(systemTime.wYear), Month(systemTime.wMonth), Day(systemTime.wDay),
      Hour(systemTime.wHour), Minute(systemTime.wMinute),
      Second(systemTime.wSecond), Millisecond(systemTime.wMilliseconds)
{
}

// Retail 009E9B80: chronological order, field by field.
bool CDateAndTime::operator<(const CDateAndTime& other) const
{
    if (Year != other.Year) return Year < other.Year;
    if (Month != other.Month) return Month < other.Month;
    if (Day != other.Day) return Day < other.Day;
    if (Hour != other.Hour) return Hour < other.Hour;
    if (Minute != other.Minute) return Minute < other.Minute;
    if (Second != other.Second) return Second < other.Second;
    return Millisecond < other.Millisecond;
}

// Retail 009E9BE0.
bool CDateAndTime::operator>(const CDateAndTime& other) const
{
    if (Year != other.Year) return Year > other.Year;
    if (Month != other.Month) return Month > other.Month;
    if (Day != other.Day) return Day > other.Day;
    if (Hour != other.Hour) return Hour > other.Hour;
    if (Minute != other.Minute) return Minute > other.Minute;
    if (Second != other.Second) return Second > other.Second;
    return Millisecond > other.Millisecond;
}

// Retail 009E9D40.
FILETIME CDateAndTime::ToFileTime() const
{
    _SYSTEMTIME systemTime;
    systemTime.wYear = (unsigned short)Year;
    systemTime.wMonth = (unsigned short)Month;
    systemTime.wDay = (unsigned short)Day;
    systemTime.wHour = (unsigned short)Hour;
    systemTime.wMinute = (unsigned short)Minute;
    systemTime.wSecond = (unsigned short)Second;
    systemTime.wDayOfWeek = 0;
    systemTime.wMilliseconds = (unsigned short)Millisecond;
    FILETIME fileTime;
    SystemTimeToFileTime(&systemTime, &fileTime);
    return fileTime;
}
