#include "fable_system_manager_state.h"
#include "fable_window_class.h"
#include <stdio.h>
#include <string.h>

struct CSystemManagerTestAccess
{
    static bool Run(CSystemManager* manager, long width, long height) { return manager->InitWindow(width, height); }
    static FableWindowProcedure Procedure() { return CSystemManager::WinProcedure; }
};

static CSystemManager* manager;
static long width, height;
static int metrics[7], show, mutate, failedResources, failures, names, icons, metricCalls;
static HWND__* createdWindow;
static void* const instance = reinterpret_cast<void*>(0x13570000);
static HWND__* const changedWindow = reinterpret_cast<HWND__*>(0x24680000);
static const wchar_t* const nameResults[3] = {L"registered class", L"window title", L"created class"};
static char trace[64];
static int traceSize;
static unsigned char expectedStorage[sizeof(CSystemManager)];
#define CHECK(x) do { if (!(x)) { ++failures; printf("FAIL line=%d\n", __LINE__); } } while (0)
static void Record(char c) { trace[traceSize++] = c; trace[traceSize] = 0; }
static void* Resource(unsigned long token) { return reinterpret_cast<void*>(failedResources ? 0 : token); }

long FABLE_STDCALL CSystemManager::WinProcedure(HWND__*, unsigned int, unsigned int, long)
{
    ++failures; return 0; // Registration only; InitWindow must not invoke it.
}
CWideString::operator const wchar_t*() const
{
    CHECK(this == &manager->WinAppName && names < 3);
    Record('N'); return nameResults[names++];
}
static void* FABLE_STDCALL Cursor(void* module, const wchar_t* id)
{
    CHECK(module == 0 && id == reinterpret_cast<const wchar_t*>(0x7f00));
    Record('C'); return Resource(0x11110000);
}
static void* FABLE_STDCALL Brush(int index)
{
    CHECK(index == 4); Record('B'); return Resource(0x22220000);
}
static void* FABLE_STDCALL Icon(void* module, const wchar_t* id)
{
    CHECK(module == instance && id == reinterpret_cast<const wchar_t*>(101) && icons < 2);
    Record('I'); return Resource(0x33330000 + icons++);
}
static unsigned short FABLE_STDCALL Register(const FableWindowClass* windowClass)
{
    CHECK(windowClass == &manager->WinClass);
    CHECK(windowClass->cbSize == 48 && windowClass->style == 0);
    CHECK(windowClass->lpfnWndProc == CSystemManagerTestAccess::Procedure());
    CHECK(windowClass->cbClsExtra == 0 && windowClass->cbWndExtra == 0);
    CHECK(windowClass->hInstance == instance);
    CHECK(windowClass->hCursor == Resource(0x11110000));
    CHECK(windowClass->hbrBackground == Resource(0x22220000));
    CHECK(windowClass->hIconSm == Resource(0x33330000) && windowClass->hIcon == Resource(0x33330001));
    CHECK(windowClass->lpszMenuName == 0 && windowClass->lpszClassName == nameResults[0]);
    reinterpret_cast<CSystemManager*>(expectedStorage)->WinClass = *windowClass;
    Record('R'); return failedResources ? 0 : 42;
}
static int FABLE_STDCALL Metric(int index)
{
    const int order[] = {0, 1, 5, 6, 4};
    CHECK(metricCalls < 5 && index == order[metricCalls++]);
    Record(static_cast<char>('0'+index)); return metrics[index];
}
static HWND__* FABLE_STDCALL Create(unsigned long exStyle, const wchar_t* className,
    const wchar_t* title, unsigned long style, int x, int y, int w, int h,
    HWND__* parent, void* menu, void* module, void* parameter)
{
    CHECK(exStyle == 0 && style == 0x00ca0000);
    CHECK(className == nameResults[2] && title == nameResults[1]);
    CHECK(w == width && h == height);
    CHECK(x == (metrics[0]-2*metrics[5]-width)/2);
    CHECK(y == (metrics[1]-2*metrics[6]-height-metrics[4])/2);
    CHECK(parent == 0 && menu == 0 && module == instance && parameter == manager);
    if (mutate) {
        manager->WinShow = -7;
        reinterpret_cast<CSystemManager*>(expectedStorage)->WinShow = -7;
    }
    Record('W'); return createdWindow;
}
static int FABLE_STDCALL Show(HWND__* window, int command)
{
    CHECK(window == createdWindow && manager->ApplicationWinHandle == createdWindow);
    CHECK(command == (mutate ? -7 : show));
    if (mutate) manager->ApplicationWinHandle = changedWindow;
    Record('S'); return 0;
}
static int FABLE_STDCALL Update(HWND__* window)
{
    CHECK(window == (mutate ? changedWindow : createdWindow));
    Record('U'); return 0;
}
extern "C" {
void* (FABLE_STDCALL *TestCursor)(void*, const wchar_t*) = Cursor;
void* (FABLE_STDCALL *TestBrush)(int) = Brush;
void* (FABLE_STDCALL *TestIcon)(void*, const wchar_t*) = Icon;
unsigned short (FABLE_STDCALL *TestRegister)(const FableWindowClass*) = Register;
int (FABLE_STDCALL *TestMetric)(int) = Metric;
HWND__* (FABLE_STDCALL *TestCreate)(unsigned long, const wchar_t*, const wchar_t*, unsigned long,
    int, int, int, int, HWND__*, void*, void*, void*) = Create;
int (FABLE_STDCALL *TestShow)(HWND__*, int) = Show;
int (FABLE_STDCALL *TestUpdate)(HWND__*) = Update;
}
#pragma comment(linker, "/alternatename:__imp__LoadCursorW@8=_TestCursor")
#pragma comment(linker, "/alternatename:__imp__GetStockObject@4=_TestBrush")
#pragma comment(linker, "/alternatename:__imp__LoadIconW@8=_TestIcon")
#pragma comment(linker, "/alternatename:__imp__RegisterClassExW@4=_TestRegister")
#pragma comment(linker, "/alternatename:__imp__GetSystemMetrics@4=_TestMetric")
#pragma comment(linker, "/alternatename:__imp__CreateWindowExW@48=_TestCreate")
#pragma comment(linker, "/alternatename:__imp__ShowWindow@8=_TestShow")
#pragma comment(linker, "/alternatename:__imp__UpdateWindow@4=_TestUpdate")

int main()
{
    const long dimensions[][2] = {{800,600}, {0,0}, {-1,-1}, {4000,3001}, {1279,719}};
    const int screens[][5] = {{1920,1080,1,1,23}, {801,601,2,3,27}, {-9,-11,0,0,0}};
    const int shows[] = {-1,0,5};
    int scenarios = 0;
    for (int d = 0; d != 5; ++d)
    for (int s = 0; s != 3; ++s)
    for (int command = 0; command != 3; ++command)
    for (int createSuccess = 0; createSuccess != 2; ++createSuccess)
    for (mutate = 0; mutate != 2; ++mutate)
    for (int initialValid = 0; initialValid != 2; ++initialValid)
    for (failedResources = 0; failedResources != 2; ++failedResources)
    {
        unsigned long storage[sizeof(CSystemManager)/4];
        memset(storage, 0xa5, sizeof(storage));
        manager = reinterpret_cast<CSystemManager*>(storage);
        manager->WinInstanceHandle = instance;
        show = shows[command]; manager->WinShow = show;
        manager->WindowValid = initialValid != 0;
        memcpy(expectedStorage, storage, sizeof(storage));
        width = dimensions[d][0]; height = dimensions[d][1];
        metrics[0] = screens[s][0]; metrics[1] = screens[s][1];
        metrics[5] = screens[s][2]; metrics[6] = screens[s][3]; metrics[4] = screens[s][4];
        createdWindow = reinterpret_cast<HWND__*>(createSuccess ? 0x12340000 : 0);
        traceSize = names = icons = metricCalls = 0; trace[0] = 0;
        CHECK(CSystemManagerTestAccess::Run(manager, width, height));
        CHECK(strcmp(trace, "CBNIIR01564NNWSU") == 0);
        CHECK(names == 3 && icons == 2 && metricCalls == 5);
        reinterpret_cast<CSystemManager*>(expectedStorage)->ApplicationWinHandle = mutate ? changedWindow : createdWindow;
        reinterpret_cast<CSystemManager*>(expectedStorage)->WindowValid = true;
        CHECK(memcmp(storage, expectedStorage, sizeof(storage)) == 0);
        ++scenarios;
    }
    if (failures) return 1;
    printf("INIT_WINDOW_BEHAVIOR_PASS scenarios=%d\n", scenarios);
    return 0;
}
