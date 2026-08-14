#include <cstdio>

struct IRefVtbl;
struct IRefCounted { IRefVtbl* vtbl; int refcnt; };
struct IRefVtbl { void (*slot0)(IRefCounted*); void (*slot1)(IRefCounted*); };

static int g_released = 0;
static void do_release(IRefCounted* r) { g_released = 1; }

struct State { void* p0; void* p4; void* p8; IRefCounted* ref; };

static int g_base_called = 0;

// model of the derived dtor body (base dtor separate)
static void dtor_body(State* self) {
    IRefCounted* r = self->ref;
    if (r != 0) {
        if (--r->refcnt == 0) {
            r->vtbl->slot1(r);
        }
        self->ref = 0;
    }
    g_base_called = 1; // base dtor tail-call
}

int main() {
    IRefVtbl vt; vt.slot0 = 0; vt.slot1 = do_release;

    // Case 1: null ref -> just base dtor, no release
    { State s; s.ref = 0; g_released = 0; g_base_called = 0;
      dtor_body(&s);
      if (g_released != 0 || g_base_called != 1) { printf("FAIL c1\n"); return 1; } }

    // Case 2: refcnt 2 -> decremented to 1, no release, ref zeroed, base called
    { State s; IRefCounted r; r.vtbl=&vt; r.refcnt=2; s.ref=&r;
      g_released=0; g_base_called=0;
      dtor_body(&s);
      if (r.refcnt != 1 || g_released != 0 || s.ref != 0 || g_base_called != 1) { printf("FAIL c2\n"); return 1; } }

    // Case 3: refcnt 1 -> decremented to 0, release called, ref zeroed, base called
    { State s; IRefCounted r; r.vtbl=&vt; r.refcnt=1; s.ref=&r;
      g_released=0; g_base_called=0;
      dtor_body(&s);
      if (r.refcnt != 0 || g_released != 1 || s.ref != 0 || g_base_called != 1) { printf("FAIL c3\n"); return 1; } }

    printf("BLENDSTATE_DTOR_OK\n");
    return 0;
}