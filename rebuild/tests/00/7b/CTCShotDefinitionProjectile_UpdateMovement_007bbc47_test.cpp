#include <cstdio>

// Behaviour model of UpdateMovement: if predicate true, invoke slot on m4->m60 with arg 1.

static int g_called_arg = -1;
static bool g_pred = true;

struct HasVtbl { void** vtbl; };
struct M4 { char pad[0x60]; HasVtbl* m60; };
struct Self { void* m0; M4* m4; };

static void slotfn(void* self, int a) { g_called_arg = a; }

static bool pred(Self*) { return g_pred; }

static void UpdateMovement_model(Self* self) {
    if (pred(self)) {
        HasVtbl* o = self->m4->m60;
        typedef void (*VFn)(void*, int);
        VFn fn = (VFn)o->vtbl[0x98/4];
        fn(o, 1);
    }
}

int main() {
    void* vt[64] = {0};
    vt[0x98/4] = (void*)&slotfn;
    HasVtbl o; o.vtbl = vt;
    M4 m4; m4.m60 = &o;
    Self s; s.m4 = &m4;

    // predicate true -> slot called with 1
    g_pred = true; g_called_arg = -1;
    UpdateMovement_model(&s);
    bool ok1 = (g_called_arg == 1);

    // predicate false -> slot NOT called
    g_pred = false; g_called_arg = -1;
    UpdateMovement_model(&s);
    bool ok2 = (g_called_arg == -1);

    if (ok1 && ok2) printf("BEHAVIOR_OK\n");
    else printf("BEHAVIOR_FAIL a1=%d a2=%d\n", ok1, ok2);
    return 0;
}