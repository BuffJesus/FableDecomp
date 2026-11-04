// Behaviour fixture for CMainGameComponent::IsControllerDisconnected @ 0x00416296.
// Checks: the joystick-manager singleton is fetched, its main-player device
// number is read from it, GPInputManager is asked whether that device is
// active (in that order), and the result is the negation; the component is
// untouched.
#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_input_event.h"
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

CInputManager* g_FableInputManager_013B8388;

static unsigned char comp[0x161F0];
static unsigned char inputManager[0x84];
static unsigned char joystickManager[12];
static int seq, getAt, deviceAt, activeAt, calls;
static long deviceNumber;
static bool active, ok;

CGameJoystickManager& FABLE_FASTCALL CGameJoystickManager::Get()
{
    getAt = ++seq; ++calls;
    return *reinterpret_cast<CGameJoystickManager*>(joystickManager);
}
long CGameJoystickManager::GetMainPlayerJoystickDeviceNumber() const
{
    ok = ok && static_cast<const void*>(this) == joystickManager;
    deviceAt = ++seq; ++calls;
    return deviceNumber;
}
bool CInputManager::IsJoystickDeviceNumberActive(long device) const
{
    ok = ok && static_cast<const void*>(this) == inputManager && device == deviceNumber;
    activeAt = ++seq; ++calls;
    return active;
}

static bool Run(long device, bool isActive)
{
    g_FableInputManager_013B8388 = reinterpret_cast<CInputManager*>(inputManager);
    memset(comp, 0x19, sizeof(comp));
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    deviceNumber = device;
    active = isActive;
    seq = getAt = deviceAt = activeAt = calls = 0;
    ok = true;
    bool result = reinterpret_cast<CMainGameComponent*>(comp)->IsControllerDisconnected();
    ok = ok && result == !isActive && calls == 3 && getAt == 1 && deviceAt == 2 && activeAt == 3
        && memcmp(before, comp, sizeof(comp)) == 0;
    if (!ok) printf("case device=%ld active=%d failed\n", device, isActive);
    return ok;
}

int main()
{
    bool pass = Run(2, true);
    pass = Run(2, false) && pass;
    pass = Run(-1, false) && pass;
    pass = Run(0, true) && pass;
    printf(pass ? "PASS_00416296\n" : "FAIL_00416296\n");
    return pass ? 0 : 1;
}
