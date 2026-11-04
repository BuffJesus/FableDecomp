#pragma once
#include "rebuild_abi.h"
#include <stddef.h>

struct HWND__;
typedef long (FABLE_STDCALL* FableWindowProcedure)(HWND__*, unsigned int, unsigned int, long);

// Win32 WNDCLASSEXW layout used by the native platform startup boundary.
struct FableWindowClass
{
    unsigned int cbSize;
    unsigned int style;
    FableWindowProcedure lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    void* hInstance;
    void* hIcon;
    void* hCursor;
    void* hbrBackground;
    const wchar_t* lpszMenuName;
    const wchar_t* lpszClassName;
    void* hIconSm;
};
FABLE_STATIC_ASSERT(sizeof(FableWindowClass) == 48);
FABLE_STATIC_ASSERT(offsetof(FableWindowClass, lpfnWndProc) == 8);
FABLE_STATIC_ASSERT(offsetof(FableWindowClass, lpszClassName) == 40);
