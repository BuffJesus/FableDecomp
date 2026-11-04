#pragma once

// The Win32 time pieces the reconstructions use (no Platform SDK headers in
// the VC7.1 toolset tree). Layout and linkage as <winbase.h>.

#include "rebuild_abi.h"

struct _SYSTEMTIME
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
typedef _SYSTEMTIME SYSTEMTIME;
FABLE_STATIC_ASSERT(sizeof(SYSTEMTIME) == 16);

// kernel32 import (retail IAT slot 0x0143FE18; CDefStringTable 0x009D4DB0).
extern "C" __declspec(dllimport) void FABLE_STDCALL GetSystemTime(_SYSTEMTIME* systemTime);
