// Behaviour fixture for CJoystickDX's focus members (provider CJoystickDX_Focus_00ab6d10.cpp; it defines neither).
// The joystick and the system manager are raw storage at their retail offsets; the system-manager getters and the
// DirectInput device are test doubles, and the raw joystick's vtable slot 16 forwards to the real SetHasInputFocus.
#include "fable_dinput8.h"
#include "fable_joystick_dx.h"
#include "fable_gfuninitialise.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static unsigned char g_systemStorage[0x200];
static bool g_active = true, g_focus = true;
CSystemManager* FABLE_FASTCALL GFGetSystemManager() { return reinterpret_cast<CSystemManager*>(g_systemStorage); }
bool CSystemManager::IsApplicationActive() { return g_active; }
bool CSystemManager::IsApplicationInputFocus() { return g_focus; }

struct CTestDevice : IDirectInputDevice8W
{
    int Acquires, Unacquires;
    virtual HRESULT __stdcall QueryInterface(REFIID, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual HRESULT __stdcall GetCapabilities(void*) { return 0; }
    virtual HRESULT __stdcall EnumObjects(void*, void*, DWORD) { return 0; }
    virtual HRESULT __stdcall GetProperty(REFIID, void*) { return 0; }
    virtual HRESULT __stdcall SetProperty(REFIID, const void*) { return 0; }
    virtual HRESULT __stdcall Acquire() { ++Acquires; return (HRESULT)0x80070005L; }      // failures are ignored
    virtual HRESULT __stdcall Unacquire() { ++Unacquires; return (HRESULT)0x80070005L; }
    virtual HRESULT __stdcall GetDeviceState(DWORD, void*) { return 0; }
    virtual HRESULT __stdcall GetDeviceData(DWORD, DIDEVICEOBJECTDATA*, DWORD* count, DWORD) { *count = 0; return 0; }
};

struct CJoystickDXTest : CJoystickDX
{
    void Focus(bool on) { CJoystickDX::SetHasInputFocus(on); }
};
static void __fastcall Slot16(CJoystickDXTest* self, int, bool on) { self->Focus(on); }
static void* g_vtable[19];
static unsigned char storage[sizeof(CJoystickDX)];

int main()
{
    memset(g_systemStorage, 0, sizeof(g_systemStorage));
    memset(storage, 0, sizeof(storage));
    g_vtable[16] = (void*)&Slot16;
    void** vt = g_vtable; memcpy(storage, &vt, 4);
    CJoystickDXTest* j = reinterpret_cast<CJoystickDXTest*>(storage);

    j->CJoystickDX::SetHasInputFocus(true);                      // no device: nothing happens
    CHECK(storage[0xD3C] == 0);
    CTestDevice device; device.Acquires = device.Unacquires = 0;
    IDirectInputDevice8W* dev = &device; memcpy(storage + 0xD38, &dev, 4);
    j->CJoystickDX::SetHasInputFocus(true);
    CHECK(device.Acquires == 1 && storage[0xD3C] == 1);          // marked acquired whatever DirectInput said
    j->CJoystickDX::SetHasInputFocus(true);
    CHECK(device.Acquires == 1);
    j->CJoystickDX::SetHasInputFocus(false);
    CHECK(device.Unacquires == 1 && storage[0xD3C] == 0);
    j->CJoystickDX::SetHasInputFocus(false);
    CHECK(device.Unacquires == 1);

    g_active = true; g_focus = true;
    j->CJoystickDX::UpdateAcquired();
    CHECK(storage[0xD3C] == 1);
    g_focus = false;
    j->CJoystickDX::UpdateAcquired();
    CHECK(storage[0xD3C] == 0);
    g_active = false; g_focus = true;
    j->CJoystickDX::UpdateAcquired();
    CHECK(storage[0xD3C] == 0 && device.Acquires == 2);

    printf(g_fail ? "FAIL\n" : "JOYSTICK_DX_FOCUS_OK\n");
    return g_fail != 0;
}
