// Retail 009A64B0; FableWin CSystemManager::InitWindow(long, long).
#include "fable_system_manager_state.h"
#include "fable_window_class.h"

extern "C" {
__declspec(dllimport) void* FABLE_STDCALL LoadCursorW(void*, const wchar_t*);
__declspec(dllimport) void* FABLE_STDCALL GetStockObject(int);
__declspec(dllimport) void* FABLE_STDCALL LoadIconW(void*, const wchar_t*);
__declspec(dllimport) unsigned short FABLE_STDCALL RegisterClassExW(const FableWindowClass*);
__declspec(dllimport) int FABLE_STDCALL GetSystemMetrics(int);
__declspec(dllimport) HWND__* FABLE_STDCALL CreateWindowExW(unsigned long,
    const wchar_t*, const wchar_t*, unsigned long, int, int, int, int,
    HWND__*, void*, void*, void*);
__declspec(dllimport) int FABLE_STDCALL ShowWindow(HWND__*, int);
__declspec(dllimport) int FABLE_STDCALL UpdateWindow(HWND__*);
}

bool CSystemManager::InitWindow(long width, long height)
{
    WinClass.cbSize = sizeof(WinClass);
    WinClass.style = 0;
    WinClass.lpfnWndProc = WinProcedure;
    WinClass.cbClsExtra = 0;
    WinClass.cbWndExtra = 0;
    WinClass.hInstance = WinInstanceHandle;
    WinClass.hCursor = LoadCursorW(0, reinterpret_cast<const wchar_t*>(0x7f00));
    WinClass.hbrBackground = GetStockObject(4);
    WinClass.lpszMenuName = 0;
    WinClass.lpszClassName = WinAppName;
    WinClass.hIconSm = LoadIconW(WinInstanceHandle, reinterpret_cast<const wchar_t*>(101));
    WinClass.hIcon = LoadIconW(WinInstanceHandle, reinterpret_cast<const wchar_t*>(101));
    RegisterClassExW(&WinClass);

    const int screenWidth = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    const int borderWidth = 2 * GetSystemMetrics(5);
    y -= 2 * GetSystemMetrics(6);
    y -= height;
    y -= GetSystemMetrics(4);
    ApplicationWinHandle = CreateWindowExW(0, WinAppName, WinAppName, 0x00ca0000,
        (screenWidth - borderWidth - width) / 2, y / 2, width, height,
        0, 0, WinInstanceHandle, this);
    ShowWindow(static_cast<HWND__*>(ApplicationWinHandle), WinShow);
    UpdateWindow(static_cast<HWND__*>(ApplicationWinHandle));
    WindowValid = true;
    return true;
}
