#include <stdio.h>
#include <string.h>

#include "fable_game.h"
#include "fable_input_event.h"
#include "fable_system_manager.h"

// Behaviour fixture for CNewFrontendGameComponent::IsControllerDisconnected
// @ 0x0042DC63: only an explicitly chosen main-player joystick is checked,
// against the GLOBAL input manager (not the component's own +0x68 pointer).

static unsigned char g_joyBytes[0x0C];
static unsigned char g_inputBytes[0x100];
CInputManager* g_FableInputManager_013B8388 = reinterpret_cast<CInputManager*>(g_inputBytes);

static int g_seq, g_getCalls, g_hasAt, g_mainAt, g_activeAt;
static const void* g_hasThis;
static const void* g_mainThis;
static const void* g_activeThis;
static long g_activeArg;
static bool g_hasExplicit, g_active;
static long g_device;

CGameJoystickManager& FABLE_FASTCALL CGameJoystickManager::Get()
{
    ++g_getCalls;
    return *reinterpret_cast<CGameJoystickManager*>(g_joyBytes);
}

bool CGameJoystickManager::HasExplicitMainPlayerJoystickDeviceNumber() const
{
    g_hasAt = ++g_seq; g_hasThis = this;
    return g_hasExplicit;
}

long CGameJoystickManager::GetMainPlayerJoystickDeviceNumber() const
{
    g_mainAt = ++g_seq; g_mainThis = this;
    return g_device;
}

bool CInputManager::IsJoystickDeviceNumberActive(long deviceNumber) const
{
    g_activeAt = ++g_seq; g_activeThis = this; g_activeArg = deviceNumber;
    return g_active;
}

static unsigned char g_component[0x148];

static bool RunCase(const char* name, bool hasExplicit, bool active, long device, bool expect)
{
    g_seq = g_getCalls = g_hasAt = g_mainAt = g_activeAt = 0;
    g_hasThis = g_mainThis = g_activeThis = 0;
    g_activeArg = -99;
    g_hasExplicit = hasExplicit; g_active = active; g_device = device;
    memset(g_component, 0x33, sizeof(g_component));
    bool r = reinterpret_cast<CNewFrontendGameComponent*>(g_component)->IsControllerDisconnected();
    bool ok = r == expect;
    ok = ok && g_hasAt == 1 && g_hasThis == g_joyBytes;
    if (hasExplicit)
        ok = ok && g_getCalls == 2 && g_mainAt == 2 && g_mainThis == g_joyBytes
                && g_activeAt == 3 && g_activeThis == g_inputBytes && g_activeArg == device;
    else
        ok = ok && g_getCalls == 1 && g_mainAt == 0 && g_activeAt == 0;
    for (int i = 0; i < 0x148; ++i)
        if (g_component[i] != 0x33) { ok = false; break; }
    if (!ok) printf("FAIL %s r=%d\n", name, (int)r);
    return ok;
}

int main()
{
    bool ok = true;
    ok &= RunCase("no-explicit", false, false, 3, false);
    ok &= RunCase("explicit-active", true, true, 2, false);
    ok &= RunCase("explicit-inactive", true, false, 1, true);
    if (ok) printf("batch8_part3_0042dc63_pass\n");
    return ok ? 0 : 1;
}
