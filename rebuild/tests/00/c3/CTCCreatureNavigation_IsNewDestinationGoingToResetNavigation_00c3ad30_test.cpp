#include <cstdio>

struct C3DVector { float x,y,z; };

struct InnerObj;
struct InnerVtbl {
    void* s0; void* s1; void* s2; void* s3; void* s4; void* s5;
    bool (__stdcall* slot6)(void* outer, C3DVector* dest);
};
struct InnerObj { InnerVtbl* vtbl; };

struct CTCCreatureNavigation {
    void* pad0;
    InnerObj* inner;
};

static C3DVector* g_seen_dest;
static void* g_seen_outer;
static bool g_ret;

static bool __stdcall model_slot6(void* outer, C3DVector* dest)
{
    g_seen_outer = outer;
    g_seen_dest = dest;
    return g_ret;
}

// Behavioural model mirroring the retail thunk: load inner at this+4,
// virtual-call slot 0x18 passing (this, dest); float arg ignored (ret 4).
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest, float tolerance)
{
    InnerObj* obj = self->inner;
    return obj->vtbl->slot6(self, dest);
}

int main()
{
    InnerVtbl vt;
    vt.slot6 = model_slot6;
    InnerObj obj;
    obj.vtbl = &vt;
    CTCCreatureNavigation nav;
    nav.pad0 = 0;
    nav.inner = &obj;

    C3DVector d = {1.0f, 2.0f, 3.0f};

    g_ret = true;
    bool r = IsNewDestinationGoingToResetNavigation(&nav, 0, &d, 5.0f);
    if (!r) { printf("FAIL ret true\n"); return 1; }
    if (g_seen_outer != &nav) { printf("FAIL outer\n"); return 1; }
    if (g_seen_dest != &d) { printf("FAIL dest\n"); return 1; }

    g_ret = false;
    r = IsNewDestinationGoingToResetNavigation(&nav, 0, &d, 5.0f);
    if (r) { printf("FAIL ret false\n"); return 1; }

    printf("OK_00c3ad30\n");
    return 0;
}