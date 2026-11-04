#pragma once
#include "rebuild_abi.h"
#include <stddef.h>
struct HWND__;
// Native Win32 MSG layout, used without the platform SDK headers.
struct tagMSG
{
    HWND__* hwnd;
    unsigned int message;
    unsigned int wParam;
    long lParam;
    unsigned long time;
    struct { long x; long y; } pt;
};
FABLE_STATIC_ASSERT(sizeof(tagMSG) == 28);
FABLE_STATIC_ASSERT(offsetof(tagMSG, wParam) == 8);
