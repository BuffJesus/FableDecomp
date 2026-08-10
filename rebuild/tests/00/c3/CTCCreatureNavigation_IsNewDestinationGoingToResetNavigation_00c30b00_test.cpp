#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

static CTCCreatureNavigation* g_expect_self;
static C3DVector* g_expect_dest;
static bool g_ret;

struct Sub;
struct SubVtbl {
    void* s[7];
    bool (__stdcall *reset)(CTCCreatureNavigation* self, C3DVector* dest);
};
struct Sub { SubVtbl* vtbl; };
struct CTCCreatureNavigation { void* pad0; Sub* sub; };

static bool __stdcall my_reset(CTCCreatureNavigation* self, C3DVector* dest) {
    if (self != g_expect_self) { printf("bad self\n"); return false; }
    if (dest != g_expect_dest) { printf("bad dest\n"); return false; }
    return g_ret;
}

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest) {
    Sub* s = self->sub;
    return s->vtbl->reset(self, dest);
}

int main() {
    SubVtbl vt; for (int i=0;i<7;i++) vt.s[i]=0; vt.reset = my_reset;
    SubVtbl* pvt = & vt;
    Sub sub; sub.vtbl = pvt;
    Sub* psub = & sub;
    CTCCreatureNavigation nav; nav.pad0=0; nav.sub=psub;
    CTCCreatureNavigation* pnav = & nav;
    C3DVector dv; dv.x=1; dv.y=2; dv.z=3;
    C3DVector* pd = & dv;

    g_expect_self=pnav; g_expect_dest=pd;

    g_ret=true;
    bool r1 = IsNewDestinationGoingToResetNavigation(pnav,0,pd);
    g_ret=false;
    bool r2 = IsNewDestinationGoingToResetNavigation(pnav,0,pd);

    if (r1 && !r2) { printf("OK_00c30b00\n"); return 0; }
    printf("FAIL r1=%d r2=%d\n", r1, r2);
    return 1;
}