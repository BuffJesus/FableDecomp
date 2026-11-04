// Behaviour fixture for CJoystickDX's small overrides (provider CJoystickDX_Overrides_00ab6ad0.cpp; it defines none of
// them). The joystick is raw storage at the retail offsets fable_joystick_dx.h pins, driven by qualified calls; the
// base update and the device's event reader are counting doubles.
#include "fable_joystick_dx.h"
#include <stdio.h>
#include <string.h>

static int g_fail, g_order[4], g_calls;
static const void* g_this[4];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CJoystick::Update() { g_this[g_calls] = this; g_order[g_calls++] = 1; }
void CJoystickDX::UpdateEvents() { g_this[g_calls] = this; g_order[g_calls++] = 2; }

struct CJoystickDXTest : CJoystickDX
{
    float X() { return CJoystickDX::GetXAxis(); }
    float Y() { return CJoystickDX::GetYAxis(); }
    float X2() { return CJoystickDX::GetXAxis2(); }
    float Y2() { return CJoystickDX::GetYAxis2(); }
};

static unsigned char storage[sizeof(CJoystickDX)];
static void Put(size_t at, float v) { memcpy(storage + at, &v, 4); }

int main()
{
    memset(storage, 0, sizeof(storage));
    CJoystickDXTest* j = reinterpret_cast<CJoystickDXTest*>(storage);
    Put(0xD18, 0.25f); Put(0xD1C, -0.5f); Put(0xD20, 0.75f); Put(0xD24, -1.0f);
    CHECK(j->X() == 0.25f && j->Y() == -0.5f && j->X2() == 0.75f && j->Y2() == -1.0f);

    const C2DVector axes(0.125f, 0.875f);
    j->CJoystickDX::SetAxes(axes);
    CHECK(j->X() == 0.125f && j->Y() == 0.875f && j->X2() == 0.75f && j->Y2() == -1.0f);

    j->CJoystickDX::Update();
    CHECK(g_calls == 2 && g_order[0] == 1 && g_order[1] == 2 && g_this[0] == storage && g_this[1] == storage);

    CHECK(j->CJoystickDX::GetJoystickDeviceNumber() == 0);
    CHECK(j->CJoystickDX::GetRumbleWithID(3) == 0 && j->CJoystickDX::PeekRumbleWithID(3) == 0);
    unsigned char before[sizeof(storage)];
    memcpy(before, storage, sizeof(storage));
    j->CJoystickDX::AddRumble(1, *reinterpret_cast<const CJoystick::CInitRumble*>(storage), true);
    j->CJoystickDX::ClearAllRumbles();
    CHECK(memcmp(before, storage, sizeof(storage)) == 0);      // no force feedback: nothing changes

    printf(g_fail ? "FAIL\n" : "JOYSTICK_DX_OVERRIDES_OK\n");
    return g_fail != 0;
}
