#include <cstdio>
struct C3DVector { float x, y, z; };
struct CGroundNavigator { float FindPosZ(const C3DVector& pos, int flag, bool b); };
struct CGroundNavigatorExternalInterface { void* pad0; CGroundNavigator* nav; };

static const C3DVector* g_expect_pos;
static bool g_expect_b;
static const float g_ret = 42.5f;

float CGroundNavigator::FindPosZ(const C3DVector& pos, int flag, bool b)
{
    if ((CGroundNavigator*)this != (CGroundNavigator*)0x12345678) { printf("BAD this "); return 0.0f; }
    if (&pos != g_expect_pos) { printf("BAD pos "); return 0.0f; }
    if (flag != 0) { printf("BAD flag "); return 0.0f; }
    if (b != g_expect_b) { printf("BAD b "); return 0.0f; }
    return g_ret;
}

float __fastcall FindPosZAt(CGroundNavigatorExternalInterface* self, void* edx, const C3DVector& pos, bool b);

int main()
{
    CGroundNavigatorExternalInterface self;
    self.pad0 = 0;
    self.nav = (CGroundNavigator*)0x12345678;
    C3DVector pos = { 1.0f, 2.0f, 3.0f };

    g_expect_pos = &pos;
    g_expect_b = true;
    float r = FindPosZAt(&self, 0, pos, true);
    if (r != g_ret) { printf("BAD ret "); return 1; }

    g_expect_b = false;
    r = FindPosZAt(&self, 0, pos, false);
    if (r != g_ret) { printf("BAD ret2 "); return 1; }

    printf("OK_006cd970 ");
    return 0;
}