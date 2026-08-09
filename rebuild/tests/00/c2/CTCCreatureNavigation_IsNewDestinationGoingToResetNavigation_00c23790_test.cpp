#include <stdio.h>

struct C3DVector { float x,y,z; };

struct Nav;
typedef bool (__fastcall *Slot)(void* self, int edx, void* a, C3DVector* d);
struct VT { Slot s[8]; };
struct Nav { VT* vt; };
struct Obj { void* pad0; Nav* field4; };

static void* g_calledSelf; static void* g_calledA; static C3DVector* g_calledD; static bool g_ret;

static bool __fastcall slot7_impl(void* self, int, void* a, C3DVector* d) {
    g_calledSelf = self; g_calledA = a; g_calledD = d;
    return g_ret;
}

// model matching the naked forwarder
static bool model(Obj* self, C3DVector* dest, float /*unused*/) {
    Nav* n = self->field4;
    return n->vt->s[7](n, 0, self, dest);
}

int main() {
    VT vt; for (int i=0;i<8;i++) vt.s[i]=0; vt.s[7]=slot7_impl;
    Nav nav; nav.vt=&vt;
    Obj o; o.field4=&nav;
    C3DVector dv={1,2,3};

    g_ret=true;
    bool r1 = model(&o,&dv,9.0f);
    if (!r1) { printf("FAIL ret true\n"); return 1; }
    if (g_calledSelf != &nav) { printf("FAIL self\n"); return 1; }
    if (g_calledA != &o) { printf("FAIL a\n"); return 1; }
    if (g_calledD != &dv) { printf("FAIL d\n"); return 1; }

    g_ret=false;
    bool r2 = model(&o,&dv,-3.0f);
    if (r2) { printf("FAIL ret false\n"); return 1; }

    printf("OK_00c23790\n");
    return 0;
}