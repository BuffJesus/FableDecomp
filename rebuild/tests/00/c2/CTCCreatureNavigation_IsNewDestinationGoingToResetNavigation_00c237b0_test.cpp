#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
static int g_calls = 0;
static void* g_seenNav = 0;
static C3DVector* g_seenDest = 0;
static bool g_ret = false;
struct Helper {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void v5() {}
    virtual bool slot6(CTCCreatureNavigation* nav, C3DVector* dest) {
        g_calls++; g_seenNav = (void*)nav; g_seenDest = dest; return g_ret;
    }
};
struct CTCCreatureNavigation { void* pad0; Helper* helper; };
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest);
int main() {
    Helper h;
    CTCCreatureNavigation nav; nav.pad0 = 0; nav.helper = &h;
    C3DVector v; v.x = 1; v.y = 2; v.z = 3;
    g_ret = true;
    bool r1 = IsNewDestinationGoingToResetNavigation(&nav, 0, &v);
    g_ret = false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &v);
    if (g_calls != 2) { printf("FAIL calls\n"); return 1; }
    if (g_seenNav != (void*)&nav) { printf("FAIL nav\n"); return 1; }
    if (g_seenDest != &v) { printf("FAIL dest\n"); return 1; }
    if (r1 != true || r2 != false) { printf("FAIL ret\n"); return 1; }
    printf("OK_00c237b0\n");
    return 0;
}