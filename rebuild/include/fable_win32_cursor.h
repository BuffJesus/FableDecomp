#pragma once
// The user32 window/cursor calls the DirectInput mouse places the Windows cursor with (CMouseDX::SetPos 0x00AB4700,
// CMouseDX::PeekScreenPos 0x00AB4870). Imports are declared individually, as elsewhere in rebuild/include (the
// toolchain has no Windows SDK); the structs are POINT / RECT in the SDK's shape.
#include "rebuild_abi.h"

struct FableWin32Point
{
    long x;
    long y;
};

struct FableWin32Rect
{
    long left;
    long top;
    long right;
    long bottom;
};

extern "C" __declspec(dllimport) int __stdcall ClientToScreen(void* window, FableWin32Point* point);
extern "C" __declspec(dllimport) int __stdcall GetClientRect(void* window, FableWin32Rect* rect);
extern "C" __declspec(dllimport) int __stdcall OffsetRect(FableWin32Rect* rect, int dx, int dy);
extern "C" __declspec(dllimport) int __stdcall SetCursorPos(int x, int y);
