#include <cstdio>

struct CCountedObj {
    void (__fastcall** vtbl)(CCountedObj*);
    long refcount;
};

struct CIVCountedPointer {
    CCountedObj* p;
};

void __fastcall Dest_val(CIVCountedPointer* self, void* unused)
{
    CCountedObj* c = self->p;
    if (c != 0) {
        if (--c->refcount == 0) {
            c->vtbl[1](c);
        }
        self->p = 0;
    }
}

static int g_freed = 0;
static void __fastcall my_dtor(CCountedObj* c) { g_freed++; }

int main() {
    void (__fastcall* vt[2])(CCountedObj*);
    vt[0] = 0;
    vt[1] = &my_dtor;

    /* Case 1: refcount 2 -> becomes 1, not freed, pointer nulled */
    CCountedObj obj;
    obj.vtbl = vt;
    obj.refcount = 2;
    CIVCountedPointer cp;
    cp.p = &obj;
    Dest_val(&cp, 0);
    if (g_freed != 0) { std::printf("FAIL: freed early\n"); return 1; }
    if (obj.refcount != 1) { std::printf("FAIL: refcount %ld\n", obj.refcount); return 1; }
    if (cp.p != 0) { std::printf("FAIL: not nulled\n"); return 1; }

    /* Case 2: refcount 1 -> becomes 0, freed */
    obj.refcount = 1;
    cp.p = &obj;
    Dest_val(&cp, 0);
    if (g_freed != 1) { std::printf("FAIL: not freed\n"); return 1; }
    if (cp.p != 0) { std::printf("FAIL: not nulled 2\n"); return 1; }

    /* Case 3: null pointer -> no-op, stays null */
    cp.p = 0;
    Dest_val(&cp, 0);
    if (g_freed != 1) { std::printf("FAIL: freed on null\n"); return 1; }
    if (cp.p != 0) { std::printf("FAIL: null case\n"); return 1; }

    std::printf("Dest_val_0040d8b0_TEST PASS\n");
    return 0;
}