#include <cstdio>
struct C3DVector { float x,y,z; };
struct NavHelper;
struct NavHelperVtbl {
    char pad0[0x18];
    unsigned char (__fastcall *vf18)(NavHelper* self, int edx, void* nav, C3DVector* v);
};
struct NavHelper { NavHelperVtbl* vtbl; };
struct CTCCreatureNavigation {
    void* field0;
    NavHelper* helper;
};
static void* g_expect_nav;
static C3DVector* g_expect_v;
static unsigned char g_ret;
static NavHelper* g_expect_self;
static unsigned char __fastcall my_vf18(NavHelper* self, int edx, void* nav, C3DVector* v) {
    if (self != g_expect_self) { printf("BAD_SELF\n"); return 0xFF; }
    if (nav != g_expect_nav) { printf("BAD_NAV\n"); return 0xFF; }
    if (v != g_expect_v) { printf("BAD_V\n"); return 0xFF; }
    return g_ret;
}
extern "C" unsigned char __fastcall IsNewDestinationGoingToResetNavigation(void* self, int edx, void* v, float f);
int main() {
    NavHelperVtbl vt; vt.vf18 = my_vf18;
    NavHelper h; h.vtbl = &vt;
    CTCCreatureNavigation nav; nav.field0 = 0; nav.helper = &h;
    C3DVector vec; vec.x=1; vec.y=2; vec.z=3;
    g_expect_self = &h; g_expect_nav = &nav; g_expect_v = &vec;
    g_ret = 1;
    unsigned char r1 = IsNewDestinationGoingToResetNavigation(&nav, 0, &vec, 5.0f);
    g_ret = 0;
    unsigned char r0 = IsNewDestinationGoingToResetNavigation(&nav, 0, &vec, 5.0f);
    if (r1 == 1 && r0 == 0) { printf("OK_00c47660\n"); return 0; }
    printf("FAIL r1=%d r0=%d\n", r1, r0); return 1;
}