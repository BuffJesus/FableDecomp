#include <cstdio>
struct CRegion { int field0; };
static int g_arg = 0;
extern "C" int __cdecl RegionExitTextHelper(int v) { g_arg = v; return v + 100; }
void __fastcall CRegion_SetMiniMapRegionExitTextOffsetX(CRegion* self, int edx_dummy, int* out, int unused)
{
    int old = self->field0;
    self->field0 = RegionExitTextHelper(old);
    *out = old;
}
int main()
{
    CRegion r; r.field0 = 42;
    int out = -1;
    CRegion_SetMiniMapRegionExitTextOffsetX(&r, 0, &out, 999);
    if (r.field0 == 142 && out == 42 && g_arg == 42) {
        std::printf("CRegion_0042a0ce_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL field0=%d out=%d g_arg=%d\n", r.field0, out, g_arg);
    return 1;
}