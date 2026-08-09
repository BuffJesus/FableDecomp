#include <stdio.h>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
typedef bool (__stdcall *Slot6)(CTCCreatureNavigation* nav, C3DVector* vec);
struct Sub { Slot6* vptr; };
struct CTCCreatureNavigation { void* field_0; Sub* field_4; };

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* vec, float f)
{
    Sub* s = self->field_4;
    Slot6 fn = s->vptr[6];
    return fn(self, vec);
}

static CTCCreatureNavigation* g_expNav;
static C3DVector* g_expVec;
static bool g_ret;
static bool __stdcall Impl(CTCCreatureNavigation* nav, C3DVector* vec) {
    if (nav != g_expNav) { printf("bad nav\n"); return false; }
    if (vec != g_expVec) { printf("bad vec\n"); return false; }
    return g_ret;
}

int main() {
    Slot6 vt[16] = {0};
    vt[6] = Impl;
    Slot6* vptr = vt;
    Sub subObj; subObj.vptr = vptr;
    Sub* psub = &subObj;
    CTCCreatureNavigation nav; nav.field_0 = 0; nav.field_4 = psub;
    C3DVector v = {1,2,3};
    g_expNav = &nav; g_expVec = &v;

    g_ret = true;
    if (IsNewDestinationGoingToResetNavigation(&nav, 0, &v, 9.0f) != true) { printf("FAIL true\n"); return 1; }
    g_ret = false;
    if (IsNewDestinationGoingToResetNavigation(&nav, 0, &v, 9.0f) != false) { printf("FAIL false\n"); return 1; }
    printf("OK_00c151e0\n");
    return 0;
}