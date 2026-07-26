#include <cstdio>

struct RefCounted { void (**vtbl)(void); int refcount; };
struct CountedPtr { RefCounted* p; };

static int g_destroyed = 0;

void __fastcall Dest_val(CountedPtr* self)
{
    RefCounted* obj = self->p;
    if (obj) {
        if (--obj->refcount == 0) {
            void (**vt)(void) = obj->vtbl;
            ((void (__fastcall*)(RefCounted*))vt[1])(obj);
        }
        self->p = 0;
    }
}

void __fastcall fake_dtor(RefCounted* o) { g_destroyed = 1; }

int main()
{
    /* case 1: null pointer -> no crash, stays null */
    CountedPtr a; a.p = 0;
    Dest_val(&a);
    if (a.p != 0) { std::printf("FAIL null\n"); return 1; }

    /* build a vtable: slot0 unused, slot1 = dtor */
    void (*vt[2])(void);
    vt[0] = 0;
    vt[1] = (void(*)(void))fake_dtor;

    /* case 2: refcount 2 -> becomes 1, no destroy, ptr nulled */
    RefCounted obj2; obj2.vtbl = vt; obj2.refcount = 2;
    CountedPtr b; b.p = &obj2;
    g_destroyed = 0;
    Dest_val(&b);
    if (b.p != 0 || obj2.refcount != 1 || g_destroyed != 0) { std::printf("FAIL refcount2\n"); return 1; }

    /* case 3: refcount 1 -> becomes 0, destroy called, ptr nulled */
    RefCounted obj1; obj1.vtbl = vt; obj1.refcount = 1;
    CountedPtr c; c.p = &obj1;
    g_destroyed = 0;
    Dest_val(&c);
    if (c.p != 0 || obj1.refcount != 0 || g_destroyed != 1) { std::printf("FAIL refcount1\n"); return 1; }

    std::printf("_Dest_val_0044c1a0_TEST PASS\n");
    return 0;
}