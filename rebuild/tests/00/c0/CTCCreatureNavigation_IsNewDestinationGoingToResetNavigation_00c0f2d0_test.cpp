#include <cstdio>
struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;

typedef bool (__stdcall *Slot7)(CTCCreatureNavigation* nav, C3DVector* dest);
struct HelperVtbl { Slot7 fn[8]; };
struct Helper { HelperVtbl* vtbl; };
struct CTCCreatureNavigation { void* f0; Helper* f4; };

// Behavioural model: forward this->f4 slot-7(this, dest).
static bool ModelIsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, C3DVector* dest) {
    return self->f4->vtbl->fn[7](self, dest);
}

static CTCCreatureNavigation* g_expect_nav;
static C3DVector*             g_expect_dest;
static bool __stdcall slotYes(CTCCreatureNavigation* nav, C3DVector* dest) {
    if (nav != g_expect_nav || dest != g_expect_dest) { printf("BADARGS\n"); }
    return true;
}
static bool __stdcall slotNo(CTCCreatureNavigation* nav, C3DVector* dest) {
    if (nav != g_expect_nav || dest != g_expect_dest) { printf("BADARGS\n"); }
    return false;
}

int main(){
    Slot7 yesTbl[8]; Slot7 noTbl[8];
    for (int i=0;i<8;i++){ yesTbl[i]=0; noTbl[i]=0; }
    yesTbl[7]=slotYes; noTbl[7]=slotNo;
    HelperVtbl vy; for(int i=0;i<8;i++) vy.fn[i]=yesTbl[i];
    HelperVtbl vn; for(int i=0;i<8;i++) vn.fn[i]=noTbl[i];
    Helper hy; hy.vtbl=&vy;
    Helper hn; hn.vtbl=&vn;

    C3DVector dest = {1.0f,2.0f,3.0f};
    CTCCreatureNavigation a; a.f0=0; a.f4=&hy;
    CTCCreatureNavigation b; b.f0=0; b.f4=&hn;

    g_expect_dest=&dest;
    g_expect_nav=&a; bool r1=ModelIsNewDestinationGoingToResetNavigation(&a,&dest);
    g_expect_nav=&b; bool r2=ModelIsNewDestinationGoingToResetNavigation(&b,&dest);

    if (r1 && !r2) { printf("OK_00c0f2d0\n"); return 0; }
    printf("FAIL r1=%d r2=%d\n", (int)r1, (int)r2); return 1;
}