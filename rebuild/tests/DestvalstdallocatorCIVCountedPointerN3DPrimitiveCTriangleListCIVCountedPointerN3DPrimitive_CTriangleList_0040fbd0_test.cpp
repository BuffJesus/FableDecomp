#include <cstdio>

struct RefTarget { void (__fastcall **vtbl)(void*); int refcount; };
struct CountedPtr { RefTarget* p; };

static int g_released = 0;

void __fastcall Dest_val(CountedPtr* self)
{
    RefTarget* obj = self->p;
    if (obj != 0) {
        if (--obj->refcount == 0) {
            obj->vtbl[1](obj);
        }
        self->p = 0;
    }
}

static void __fastcall my_release(void*) { g_released++; }

int main()
{
    void (__fastcall *vt[2])(void*);
    vt[0] = 0;
    vt[1] = my_release;

    RefTarget t;
    t.vtbl = vt;
    t.refcount = 1;
    CountedPtr cp;
    cp.p = &t;

    Dest_val(&cp);
    if (cp.p != 0) { std::printf("FAIL: ptr not cleared\n"); return 1; }
    if (g_released != 1) { std::printf("FAIL: release not called\n"); return 1; }

    /* refcount > 1 path: no release, still cleared */
    RefTarget t2;
    t2.vtbl = vt;
    t2.refcount = 2;
    CountedPtr cp2;
    cp2.p = &t2;
    Dest_val(&cp2);
    if (cp2.p != 0) { std::printf("FAIL: ptr2 not cleared\n"); return 1; }
    if (g_released != 1) { std::printf("FAIL: release wrongly called\n"); return 1; }
    if (t2.refcount != 1) { std::printf("FAIL: refcount not dec\n"); return 1; }

    /* null path */
    CountedPtr cp3;
    cp3.p = 0;
    Dest_val(&cp3);
    if (cp3.p != 0) { std::printf("FAIL: null path\n"); return 1; }

    std::printf("Dest_val_0040fbd0_TEST PASS\n");
    return 0;
}