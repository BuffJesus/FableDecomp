#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct INavHelper {
    bool (__stdcall **vtbl)(CTCCreatureNavigation*, C3DVector*);
};
struct CTCCreatureNavigation {
    void* f0;
    INavHelper* f4;
};
static CTCCreatureNavigation* g_expect_nav;
static C3DVector* g_expect_dest;
static bool g_ret;
static bool __stdcall slot6(CTCCreatureNavigation* nav, C3DVector* dest) {
    if (nav != g_expect_nav) { printf("FAIL nav\n"); return false; }
    if (dest != g_expect_dest) { printf("FAIL dest\n"); return false; }
    return g_ret;
}
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest, float)
{
    INavHelper* h = self->f4;
    return h->vtbl[6](self, dest);
}
int main() {
    bool (__stdcall *vt[8])(CTCCreatureNavigation*, C3DVector*);
    for (int i=0;i<8;i++) vt[i]=0;
    vt[6]=slot6;
    INavHelper helper; helper.vtbl = vt;
    CTCCreatureNavigation nav; nav.f0=0; nav.f4=&helper;
    C3DVector dv; dv.x=1; dv.y=2; dv.z=3;
    g_expect_nav=&nav; g_expect_dest=&dv;
    g_ret=true;
    bool r1 = IsNewDestinationGoingToResetNavigation(&nav,0,&dv,0.5f);
    g_ret=false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav,0,&dv,0.5f);
    if (r1==true && r2==false) { printf("OK_00c0f0d0\n"); return 0; }
    printf("FAIL ret %d %d\n", r1, r2); return 1;
}