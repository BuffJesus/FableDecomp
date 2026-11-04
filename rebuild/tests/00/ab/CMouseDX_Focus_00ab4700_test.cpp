// Behaviour fixture for CMouseDX's window and focus members (provider CMouseDX_Focus_00ab4700.cpp; it defines none of
// them). The mouse and the system manager are raw storage at their retail offsets; the system-manager getters, the
// DirectInput device and the user32 imports are test doubles (the imports through /alternatename onto pointer
// variables, since the bootstrap toolchain has no user32.lib).
#include "fable_dinput8.h"
#include "fable_mouse_dx.h"
#include "fable_gfuninitialise.h"
#include "fable_win32_cursor.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

// System manager: retail ApplicationWinHandle at +0x94.
static unsigned char g_systemStorage[0x200];
static bool g_active = true, g_focus = true;
CSystemManager* FABLE_FASTCALL GFGetSystemManager() { return reinterpret_cast<CSystemManager*>(g_systemStorage); }
bool CSystemManager::IsApplicationActive() { return g_active; }
bool CSystemManager::IsApplicationInputFocus() { return g_focus; }

// user32 doubles: the window's client area sits at screen (100, 50).
static void* g_window = reinterpret_cast<void*>(0x1234);
static int g_cursorX = -1, g_cursorY = -1, g_cursorCalls, g_badWindow;
static int __stdcall TestClientToScreen(void* w, FableWin32Point* p) { g_badWindow += w != g_window; p->x += 100; p->y += 50; return 1; }
static int __stdcall TestGetClientRect(void* w, FableWin32Rect* r) { g_badWindow += w != g_window; r->left = 0; r->top = 0; r->right = 640; r->bottom = 480; return 1; }
static int __stdcall TestOffsetRect(FableWin32Rect* r, int dx, int dy) { r->left += dx; r->right += dx; r->top += dy; r->bottom += dy; return 1; }
static int __stdcall TestSetCursorPos(int x, int y) { g_cursorX = x; g_cursorY = y; ++g_cursorCalls; return 1; }
extern "C" void* ImpClientToScreen = (void*)&TestClientToScreen;
extern "C" void* ImpGetClientRect = (void*)&TestGetClientRect;
extern "C" void* ImpOffsetRect = (void*)&TestOffsetRect;
extern "C" void* ImpSetCursorPos = (void*)&TestSetCursorPos;
#pragma comment(linker, "/alternatename:__imp__ClientToScreen@8=_ImpClientToScreen")
#pragma comment(linker, "/alternatename:__imp__GetClientRect@8=_ImpGetClientRect")
#pragma comment(linker, "/alternatename:__imp__OffsetRect@12=_ImpOffsetRect")
#pragma comment(linker, "/alternatename:__imp__SetCursorPos@8=_ImpSetCursorPos")

// DirectInput device double.
struct CTestDevice : IDirectInputDevice8W
{
    HRESULT AcquireResult, UnacquireResult;
    int Acquires, Unacquires;
    virtual HRESULT __stdcall QueryInterface(REFIID, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual HRESULT __stdcall GetCapabilities(void*) { return 0; }
    virtual HRESULT __stdcall EnumObjects(void*, void*, DWORD) { return 0; }
    virtual HRESULT __stdcall GetProperty(REFIID, void*) { return 0; }
    virtual HRESULT __stdcall SetProperty(REFIID, const void*) { return 0; }
    virtual HRESULT __stdcall Acquire() { ++Acquires; return AcquireResult; }
    virtual HRESULT __stdcall Unacquire() { ++Unacquires; return UnacquireResult; }
    virtual HRESULT __stdcall GetDeviceState(DWORD, void*) { return 0; }
    virtual HRESULT __stdcall GetDeviceData(DWORD, DIDEVICEOBJECTDATA*, DWORD* count, DWORD) { *count = 0; return 0; }
};

// Every CMouseDX vtable slot the provider's virtual call reaches is SetHasInputFocus (slot 14); give the raw mouse a
// table whose slot 14 forwards to the real member.
struct CMouseDXTest : CMouseDX
{
    void Focus(bool on) { CMouseDX::SetHasInputFocus(on); }
};
static void __fastcall Slot14(CMouseDXTest* self, int, bool on) { self->Focus(on); }
static void* g_vtable[18];

static unsigned char storage[sizeof(CMouseDX)];
static float F(size_t at) { float v; memcpy(&v, storage + at, 4); return v; }
static void PutF(size_t at, float v) { memcpy(storage + at, &v, 4); }

int main()
{
    memset(g_systemStorage, 0, sizeof(g_systemStorage));
    memcpy(g_systemStorage + 0x94, &g_window, 4);
    memset(storage, 0, sizeof(storage));
    g_vtable[14] = (void*)&Slot14;
    void** vt = g_vtable; memcpy(storage, &vt, 4);
    CMouseDXTest* m = reinterpret_cast<CMouseDXTest*>(storage);

    // SetPos, windowed: position and the old-position copies are set, and the cursor goes to client origin + position.
    const C2DVector pos(30.0f, 20.0f);
    m->CMouseDX::SetPos(&pos);
    CHECK(F(0x3414) == 30.0f && F(0x3418) == 20.0f && F(0x3434) == 30.0f && F(0x3438) == 20.0f);
    CHECK(g_cursorCalls == 1 && g_cursorX == 130 && g_cursorY == 70 && g_badWindow == 0);
    // Exclusive mode: the Windows cursor is left alone.
    storage[0x343C] = 1;
    const C2DVector pos2(5.0f, 6.0f);
    m->CMouseDX::SetPos(&pos2);
    CHECK(F(0x3414) == 5.0f && g_cursorCalls == 1);

    // PeekScreenPos: exclusive -> the position itself; windowed -> client origin + position.
    C2DVector s = m->CMouseDX::PeekScreenPos();
    CHECK(s.X == 5.0f && s.Y == 6.0f);
    storage[0x343C] = 0;
    s = m->CMouseDX::PeekScreenPos();
    CHECK(s.X == 105.0f && s.Y == 56.0f && g_badWindow == 0);

    // SetHasInputFocus without a device does nothing.
    m->CMouseDX::SetHasInputFocus(true);
    CHECK(storage[0x3424] == 0);
    CTestDevice device; device.AcquireResult = DI_OK; device.UnacquireResult = DI_NOEFFECT; device.Acquires = device.Unacquires = 0;
    IDirectInputDevice8W* dev = &device; memcpy(storage + 0x341C, &dev, 4);
    m->CMouseDX::SetHasInputFocus(true);
    CHECK(device.Acquires == 1 && storage[0x3424] == 1);
    m->CMouseDX::SetHasInputFocus(true);                     // already acquired
    CHECK(device.Acquires == 1);
    m->CMouseDX::SetHasInputFocus(false);                    // DI_NOEFFECT still clears Acquired
    CHECK(device.Unacquires == 1 && storage[0x3424] == 0);
    m->CMouseDX::SetHasInputFocus(false);                    // not acquired: nothing to release
    CHECK(device.Unacquires == 1);
    device.AcquireResult = (HRESULT)0x80070005L;             // a failed acquire leaves it unacquired
    m->CMouseDX::SetHasInputFocus(true);
    CHECK(device.Acquires == 2 && storage[0x3424] == 0);
    device.AcquireResult = DI_OK;

    // UpdateAcquired: acquire only while active and focused.
    g_active = true; g_focus = true;
    m->CMouseDX::UpdateAcquired();
    CHECK(storage[0x3424] == 1);
    g_focus = false; device.UnacquireResult = DI_OK;
    m->CMouseDX::UpdateAcquired();
    CHECK(storage[0x3424] == 0);
    g_active = false; g_focus = true;
    m->CMouseDX::UpdateAcquired();
    CHECK(storage[0x3424] == 0);

    printf(g_fail ? "FAIL\n" : "MOUSE_DX_FOCUS_OK\n");
    return g_fail != 0;
}
