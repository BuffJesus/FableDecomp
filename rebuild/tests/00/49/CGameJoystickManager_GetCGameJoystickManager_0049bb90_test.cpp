// Fixture: CGameJoystickManager::Get (0x0049BB90): creates the manager once
// (base ctor, then MainPlayerJoystickDeviceNumber = 0 / Set = false), hands it
// to PClass.Reset, and returns *PClass on every call.
#include <stdio.h>
#include <string.h>
#include "fable_input_event.h"
#include "test_support/base_class_stubs.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

static int g_seq, g_baseCtorAt, g_resetAt, g_resets;
static void* g_baseThis; static void* g_resetThis; static CGameJoystickManager* g_resetArg;
CBaseClassNonCopyable::CBaseClassNonCopyable() { g_baseCtorAt = ++g_seq; g_baseThis = this; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CGameJoystickManager::~CGameJoystickManager() {}
template <> void CCountedPointer<CGameJoystickManager>::Reset(CGameJoystickManager* data)
{
    g_resetAt = ++g_seq; ++g_resets; g_resetThis = this; g_resetArg = data; Data = data;
}
CCountedPointer<CGameJoystickManager> CGameJoystickManager::PClass;

int main()
{
    CGameJoystickManager& a = CGameJoystickManager::Get();
    EXPECT(g_resets == 1 && g_baseCtorAt == 1 && g_resetAt == 2);
    EXPECT(g_resetArg == &a && g_baseThis == (void*)&a);
    EXPECT(a.GetMainPlayerJoystickDeviceNumber() == 0 && a.HasExplicitMainPlayerJoystickDeviceNumber() == false);
    const unsigned char* raw = reinterpret_cast<const unsigned char*>(&a);
    long dev; memcpy(&dev, raw + 4, 4);
    EXPECT(dev == 0 && raw[8] == 0);
    CGameJoystickManager& b = CGameJoystickManager::Get();
    EXPECT(&b == &a && g_resets == 1 && g_seq == 2);   // no second allocation
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("FABLETLC_GAME_JOYSTICK_MANAGER_GET PASS\n");
    return 0;
}
// Getters used above (their own rows are 0x0049BB40 / 0x0049BB50).
long CGameJoystickManager::GetMainPlayerJoystickDeviceNumber() const { long v; memcpy(&v, reinterpret_cast<const unsigned char*>(this) + 4, 4); return v; }
bool CGameJoystickManager::HasExplicitMainPlayerJoystickDeviceNumber() const { return reinterpret_cast<const unsigned char*>(this)[8] != 0; }