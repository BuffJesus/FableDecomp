// Behaviour fixture for CDateAndTime::Initialise(const FILETIME&) (0x009E9CB0).
// Links the compiled source object; the conversion is the real kernel32
// FileTimeToSystemTime.
#include "fable_file_information.h"

#include <stdio.h>
#include <string.h>

int main()
{
    int fail = 0;
    unsigned char raw[0x1C + 8];
    memset(raw, 0xEE, sizeof(raw));
    CDateAndTime* t = reinterpret_cast<CDateAndTime*>(raw + 4);

    // 2004-09-14 12:34:56.789 UTC (Fable's release month) as a FILETIME:
    // 100 ns ticks since 1601-01-01.
    unsigned __int64 ticks = 127396388967890000ULL;
    FILETIME ft;
    ft.dwLowDateTime = (unsigned long)ticks;
    ft.dwHighDateTime = (unsigned long)(ticks >> 32);
    t->Initialise(ft);
    if (t->Year != 2004 || t->Month != 9 || t->Day != 14) fail |= 1;
    if (t->Hour != 12 || t->Minute != 34 || t->Second != 56 || t->Millisecond != 789) fail |= 2;
    if (raw[3] != 0xEE || raw[4 + 0x1C] != 0xEE) fail |= 4;

    // The epoch itself.
    ft.dwLowDateTime = 0;
    ft.dwHighDateTime = 0;
    t->Initialise(ft);
    if (t->Year != 1601 || t->Month != 1 || t->Day != 1 || t->Hour != 0 || t->Millisecond != 0) fail |= 8;

    if (fail == 0)
        printf("CDATEANDTIME_INITIALISE_FILETIME_PASS\n");
    else
        printf("CDATEANDTIME_INITIALISE_FILETIME_FAIL %d y=%ld m=%lu d=%lu %lu:%lu:%lu.%lu\n", fail,
            t->Year, t->Month, t->Day, t->Hour, t->Minute, t->Second, t->Millisecond);
    return 0;
}