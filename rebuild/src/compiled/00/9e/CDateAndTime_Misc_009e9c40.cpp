// lib_date_and_time.cpp, retail 009E9C40-009E9EB0: the CDateAndTime members
// not in the earlier landings. >= and <= are the negated strict comparisons;
// ToString formats "[dd/mm/yy hh:mm:ss]" into a 1K CArray<char> (Ego_r local
// buffer, its zero-filled vector(n) expanded here).
#include <stdio.h>
#include "fable_date_and_time.h"
#include "fable_win32_time.h"
#include "fable_string.h"
#include "fable_carray.h"
#include "fable_stlport_vector_impl.h"

// Retail 009E9C40.
bool CDateAndTime::operator>=(const CDateAndTime& other) const
{
    return !(*this < other);
}

// Retail 009E9C70: as the SYSTEMTIME constructor, skipping the day of week.
void CDateAndTime::Initialise(const _SYSTEMTIME& systemTime)
{
    Year = systemTime.wYear;
    Month = systemTime.wMonth;
    Day = systemTime.wDay;
    Hour = systemTime.wHour;
    Minute = systemTime.wMinute;
    Second = systemTime.wSecond;
    Millisecond = systemTime.wMilliseconds;
}

// Retail 009E9D10.
void CDateAndTime::Initialise(const CDateAndTime& date_and_time)
{
    Year = date_and_time.Year;
    Month = date_and_time.Month;
    Day = date_and_time.Day;
    Hour = date_and_time.Hour;
    Minute = date_and_time.Minute;
    Second = date_and_time.Second;
    Millisecond = date_and_time.Millisecond;
}

// Retail 009E9E10.
bool CDateAndTime::operator<=(const CDateAndTime& other) const
{
    return !(*this > other);
}

// Retail 009E9E30.
CCharString CDateAndTime::ToString() const
{
    CArray<char> buffer(1024);
    sprintf(&buffer[0], "[%02d/%02d/%02d %02d:%02d:%02d]", Day, Month, Year, Hour, Minute, Second);
    CCharString result(&buffer[0]);
    return result;
}

bool CDateAndTime::IsValid() const
{
    return Month != 0;
}
