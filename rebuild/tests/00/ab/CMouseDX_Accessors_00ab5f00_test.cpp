// Behaviour fixture for CMouseDX's small overrides (provider CMouseDX_Accessors_00ab5f00.cpp; it defines none of
// them). The mouse is raw storage at the retail offsets fable_mouse_dx.h pins, driven by qualified calls.
#include "fable_mouse_dx.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static unsigned char storage[sizeof(CMouseDX)];
static float F(size_t at) { float v; memcpy(&v, storage + at, 4); return v; }

int main()
{
    memset(storage, 0, sizeof(storage));
    CMouseDX* m = reinterpret_cast<CMouseDX*>(storage);
    const float pos[2] = { 12.5f, -3.25f };
    memcpy(storage + 0x3414, pos, sizeof(pos));

    C2DVector out;
    m->CMouseDX::GetPos(&out);
    CHECK(memcmp(&out, pos, sizeof(pos)) == 0);
    CHECK(reinterpret_cast<const void*>(&m->CMouseDX::PeekPos()) == storage + 0x3414);

    m->CMouseDX::SetInputOn(true);
    CHECK(storage[0x343D] == 1);
    m->CMouseDX::SetInputOn(false);
    CHECK(storage[0x343D] == 0);

    m->CMouseDX::SetRecentringOn(true);
    CHECK(storage[0x343E] == 1 && m->CMouseDX::IsRecentringOn());
    m->CMouseDX::SetRecentringOn(false);
    CHECK(!m->CMouseDX::IsRecentringOn());

    m->CMouseDX::SetDrawCursor(true);
    CHECK(storage[0x343F] == 1 && m->CMouseDX::IsDrawingCursor());
    m->CMouseDX::SetDrawCursor(false);
    CHECK(!m->CMouseDX::IsDrawingCursor());

    m->CMouseDX::SetMouseFiltering(true);
    CHECK(storage[0x484B] == 1);

    storage[0x4848] = 1; storage[0x4849] = 0; storage[0x484A] = 1;
    CHECK(m->CMouseDX::IsLMBDown() && !m->CMouseDX::IsMMBDown() && m->CMouseDX::IsRMBDown());
    storage[0x4848] = 0; storage[0x4849] = 1; storage[0x484A] = 0;
    CHECK(!m->CMouseDX::IsLMBDown() && m->CMouseDX::IsMMBDown() && !m->CMouseDX::IsRMBDown());
    CHECK(F(0x3414) == 12.5f);    // untouched by the switches

    printf(g_fail ? "FAIL\n" : "MOUSE_DX_ACCESSORS_OK\n");
    return g_fail != 0;
}
