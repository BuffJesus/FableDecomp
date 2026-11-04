// Behaviour fixture for CJoystickDX::EnumAndInit (provider CJoystickDX_EnumAndInit_00ab6b10.cpp), the EnumDevices callback.
// Fake COM objects stand in for IDirectInput8W (CreateDevice, slot 3) and IDirectInputDevice8W (Release 2, SetProperty
// 6, Acquire 7, SetDataFormat 11, SetCooperativeLevel 13). The joystick, its CInputManagerDX and the system manager are
// raw storage at their retail offsets. For every setup step the fixture makes that step fail and checks the callback
// releases the device (except when CreateDevice itself failed), leaves the joystick unbound and keeps enumerating;
// DIPROP_BUFFERSIZE answering 2 (DI_POLLEDDEVICE) still counts as success; a full success binds and acquires the device
// and stops the enumeration. The property payloads (axes, range, dead zone, buffer size) are checked on the way.
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "fable_dinput8.h"
#include "fable_input_manager.h"
#include "fable_joystick_dx.h"
#include "fable_gfuninitialise.h"

// EnumAndInit is protected (Ego_r ?EnumAndInit@CJoystickDX@@KGH...), so it is reached by its mangled name.
extern "C" int __stdcall CallEnumAndInit(const DIDEVICEINSTANCEW*, void*);
#pragma comment(linker, "/alternatename:_CallEnumAndInit@8=?EnumAndInit@CJoystickDX@@KGHPBUDIDEVICEINSTANCEW@@PAX@Z")

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

enum { STEP_CREATE, STEP_FORMAT, STEP_COOP, STEP_RANGE_X, STEP_RANGE_Y, STEP_DEAD_X, STEP_DEAD_Y, STEP_BUFFER, STEP_ACQUIRE, STEP_NONE };
static int g_failStep, g_step, g_releases, g_bufferResult;
static const void* g_format;
static void* g_hwnd;
static unsigned long g_coopFlags;
static const void* g_createGuid;
struct PropCall { unsigned long prop, size, header, obj, how, a, b; };
static PropCall g_props[8];
static int g_propCount;

static long Result(int step) { return g_failStep == step ? (long)0x80004005L : 0; }

static long __stdcall DevRelease(void*) { ++g_releases; return 0; }
static long __stdcall DevSetProperty(void*, const void* prop, const void* header)
{
    const unsigned long* h = static_cast<const unsigned long*>(header);
    PropCall& c = g_props[g_propCount++ & 7];
    c.prop = (unsigned long)prop; c.size = h[0]; c.header = h[1]; c.obj = h[2]; c.how = h[3]; c.a = h[4]; c.b = h[0] > 20 ? h[5] : 0;
    int step = STEP_RANGE_X + (g_propCount - 1);
    if ((unsigned long)prop == 1)   // DIPROP_BUFFERSIZE
        return g_failStep == STEP_BUFFER ? (long)0x80004005L : g_bufferResult;
    return Result(step);
}
static long __stdcall DevAcquire(void*) { return Result(STEP_ACQUIRE); }
static long __stdcall DevSetDataFormat(void*, const void* format) { g_format = format; return Result(STEP_FORMAT); }
static long __stdcall DevSetCooperativeLevel(void*, void* hwnd, unsigned long flags) { g_hwnd = hwnd; g_coopFlags = flags; return Result(STEP_COOP); }
static long __stdcall Unused(void*) { CHECK(!"unexpected device slot"); return 0; }

static void* g_deviceVtable[14];
static void* g_device[2] = { g_deviceVtable, 0 };

static long __stdcall DICreateDevice(void*, const void* guid, void** device, void* outer)
{
    g_createGuid = guid;
    CHECK(outer == 0);
    if (g_failStep == STEP_CREATE) return (long)0x80004005L;
    *device = g_device;
    return 0;
}
static void* g_dinputVtable[4] = { (void*)&Unused, (void*)&Unused, (void*)&Unused, (void*)&DICreateDevice };
static void* g_dinput[1] = { g_dinputVtable };

extern "C" const DIDATAFORMAT c_dfDIJoystick = { 24, 16, 1, 80, 0, 0 };
static unsigned char g_systemManager[0x180];
CSystemManager* FABLE_FASTCALL GFGetSystemManager() { return reinterpret_cast<CSystemManager*>(g_systemManager); }

int main()
{
    for (int i = 0; i < 14; ++i) g_deviceVtable[i] = (void*)&Unused;
    g_deviceVtable[2] = (void*)&DevRelease; g_deviceVtable[6] = (void*)&DevSetProperty; g_deviceVtable[7] = (void*)&DevAcquire;
    g_deviceVtable[11] = (void*)&DevSetDataFormat; g_deviceVtable[13] = (void*)&DevSetCooperativeLevel;
    void* hwnd = (void*)0x5150;
    memcpy(g_systemManager + offsetof(CSystemManager, ApplicationWinHandle), &hwnd, sizeof(hwnd));
    unsigned char inputManager[0x100] = { 0 };
    void* dinput = g_dinput;
    memcpy(inputManager + 0x80, &dinput, sizeof(dinput));
    DIDEVICEINSTANCEW instance;
    memset(&instance, 0, sizeof(instance));

    for (int fail = STEP_CREATE; fail <= STEP_NONE + 1; ++fail)
    {
        static unsigned char joystick[0xD44];
        memset(joystick, 0xCC, sizeof(joystick));
        void* manager = inputManager;
        memcpy(joystick + 0xD40, &manager, sizeof(manager));
        void* sentinel = (void*)0x1234;
        memcpy(joystick + 0xD38, &sentinel, sizeof(sentinel));
        joystick[0xD3C] = 0x77;
        g_failStep = fail > STEP_NONE ? STEP_NONE : fail;
        g_bufferResult = fail > STEP_NONE ? 2 : 0;    // last pass: DI_POLLEDDEVICE
        g_releases = g_propCount = 0; g_format = 0; g_hwnd = 0; g_coopFlags = 0; g_createGuid = 0;

        int result = CallEnumAndInit(&instance, joystick);
        void* bound; memcpy(&bound, joystick + 0xD38, sizeof(bound));
        CHECK(g_createGuid == &instance.guidInstance);
        if (g_failStep == STEP_NONE)
        {
            CHECK(result == DIENUM_STOP && g_releases == 0);
            CHECK(bound == g_device && joystick[0xD3C] == 1);
        }
        else
        {
            CHECK(result == DIENUM_CONTINUE);
            CHECK(g_releases == (fail == STEP_CREATE ? 0 : 1));
            CHECK(bound == sentinel);
            CHECK(joystick[0xD3C] == (fail == STEP_ACQUIRE ? 0 : 0x77));
        }
        if (fail > STEP_FORMAT) CHECK(g_format == &c_dfDIJoystick);
        if (fail > STEP_COOP) CHECK(g_hwnd == hwnd && g_coopFlags == (DISCL_EXCLUSIVE | DISCL_FOREGROUND));
        if (fail >= STEP_ACQUIRE)
        {
            CHECK(g_propCount == 5);
            CHECK(g_props[0].prop == 4 && g_props[0].obj == DIJOFS_X && g_props[0].a == 0 && g_props[0].b == 0x10000);
            CHECK(g_props[1].prop == 4 && g_props[1].obj == DIJOFS_Y && g_props[1].a == 0 && g_props[1].b == 0x10000);
            CHECK(g_props[0].size == sizeof(DIPROPRANGE) && g_props[0].header == sizeof(DIPROPHEADER) && g_props[0].how == DIPH_BYOFFSET);
            CHECK(g_props[2].prop == 5 && g_props[2].obj == DIJOFS_X && g_props[2].a == 2000 && g_props[2].size == sizeof(DIPROPDWORD));
            CHECK(g_props[3].prop == 5 && g_props[3].obj == DIJOFS_Y && g_props[3].a == 2000);
            CHECK(g_props[4].prop == 1 && g_props[4].obj == 0 && g_props[4].how == DIPH_DEVICE && g_props[4].a == 64);
        }
        else if (fail >= STEP_RANGE_X)
            CHECK(g_propCount == fail - STEP_RANGE_X + 1);   // stops at the failing property
    }

    if (g_fail) { printf("JOYSTICK_ENUM_AND_INIT_FAIL %d\n", g_fail); return 1; }
    printf("JOYSTICK_ENUM_AND_INIT_OK\n");
    return 0;
}
