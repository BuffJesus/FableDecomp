// Behaviour fixture for CJoystickDX::SetExclusiveMode (provider CJoystickDX_SetExclusiveMode_00ab6de0.cpp; it does not
// define it). The joystick and its input manager are raw storage at the retail offsets; IDirectInput8W, the device and
// the EnumAndInit callback are doubles (the callback "finds" a device when the test says one is attached).
#include "fable_dinput8.h"
#include "fable_input_manager.h"
#include "fable_joystick_dx.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct CTestDevice : IDirectInputDevice8W
{
    int Unacquires, Releases;
    virtual HRESULT __stdcall QueryInterface(REFIID, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { ++Releases; return 0; }
    virtual HRESULT __stdcall GetCapabilities(void*) { return 0; }
    virtual HRESULT __stdcall EnumObjects(void*, void*, DWORD) { return 0; }
    virtual HRESULT __stdcall GetProperty(REFIID, void*) { return 0; }
    virtual HRESULT __stdcall SetProperty(REFIID, const void*) { return 0; }
    virtual HRESULT __stdcall Acquire() { return 0; }
    virtual HRESULT __stdcall Unacquire() { ++Unacquires; return 0; }
    virtual HRESULT __stdcall GetDeviceState(DWORD, void*) { return 0; }
    virtual HRESULT __stdcall GetDeviceData(DWORD, DIDEVICEOBJECTDATA*, DWORD*, DWORD) { return 0; }
};
static CTestDevice g_found;
static bool g_attached;
static int g_callbacks;
static void* g_callbackRef;

struct CTestDInput : IDirectInput8W
{
    DWORD DevType, Flags;
    LPDIENUMDEVICESCALLBACKW Callback;
    int Enumerations;
    virtual HRESULT __stdcall QueryInterface(REFIID, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual HRESULT __stdcall CreateDevice(REFIID, IDirectInputDevice8W**, void*) { return 0; }
    virtual HRESULT __stdcall EnumDevices(DWORD devType, LPDIENUMDEVICESCALLBACKW callback, void* ref, DWORD flags)
    {
        ++Enumerations; DevType = devType; Flags = flags; Callback = callback;
        if (g_attached) callback(0, ref);
        return 0;
    }
};

// The real callback creates the device; this one plants the test device at +0xD38.
int __stdcall CJoystickDX::EnumAndInit(const DIDEVICEINSTANCEW*, void* joystick)
{
    ++g_callbacks; g_callbackRef = joystick;
    IDirectInputDevice8W* dev = &g_found;
    memcpy(static_cast<unsigned char*>(joystick) + 0xD38, &dev, 4);
    return DIENUM_STOP;
}

static unsigned char storage[sizeof(CJoystickDX)];
static unsigned char managerStorage[sizeof(CInputManagerDX)];

int main()
{
    memset(storage, 0, sizeof(storage)); memset(managerStorage, 0, sizeof(managerStorage));
    CTestDInput dinput; dinput.Enumerations = 0;
    IDirectInput8W* di = &dinput; memcpy(managerStorage + 0x80, &di, 4);
    void* manager = managerStorage; memcpy(storage + 0xD40, &manager, 4);
    CTestDevice old; old.Unacquires = old.Releases = 0;
    IDirectInputDevice8W* oldDev = &old; memcpy(storage + 0xD38, &oldDev, 4);
    g_found.Unacquires = g_found.Releases = 0;
    CJoystickDX* j = reinterpret_cast<CJoystickDX*>(storage);

    // A controller is attached: the old device is released, enumeration finds the new one.
    g_attached = true;
    CHECK(j->CJoystickDX::SetExclusiveMode(false));
    CHECK(old.Unacquires == 1 && old.Releases == 1);
    CHECK(storage[0xD08] == 1);                                       // CJoystick::ExclusiveMode
    CHECK(dinput.Enumerations == 1 && dinput.DevType == DI8DEVCLASS_GAMECTRL && dinput.Flags == DIEDFL_ATTACHEDONLY);
    CHECK(g_callbacks == 1 && g_callbackRef == storage);

    // Nothing attached: the found device is released and the joystick reports no device.
    g_attached = false;
    CHECK(!j->CJoystickDX::SetExclusiveMode(true));
    CHECK(g_found.Unacquires == 1 && g_found.Releases == 1 && dinput.Enumerations == 2);
    CHECK(*reinterpret_cast<void**>(storage + 0xD38) == 0);

    printf(g_fail ? "FAIL\n" : "JOYSTICK_DX_EXCLUSIVE_OK\n");
    return g_fail != 0;
}
