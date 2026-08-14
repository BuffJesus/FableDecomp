#include <stdio.h>
#include <string.h>

struct RefCounted { void* vfp; int refcount; };
struct CCharString { char* buf; };
struct Pair { CCharString first; RefCounted* second; };

// stand-in for CCharString copy-ctor: mark that it ran
static int g_copied = 0;
static void CCharString_copy(CCharString* self, void* /*edx*/, const CCharString* src) {
    self->buf = src->buf; // shallow model
    g_copied++;
}

static void Cons_val(Pair* self, Pair* src) {
    if (self) {
        CCharString_copy(&self->first, 0, &src->first);
        RefCounted* rc = src->second;
        self->second = rc;
        if (rc) rc->refcount++;
    }
}

int main() {
    // Case 1: src with a live counted pointer
    RefCounted rc; rc.vfp = 0; rc.refcount = 5;
    char keybuf[] = "hello";
    Pair src; src.first.buf = keybuf; src.second = &rc;
    Pair dst; dst.first.buf = 0; dst.second = (RefCounted*)0xdead;

    Cons_val(&dst, &src);
    int ok = 1;
    ok &= (g_copied == 1);
    ok &= (dst.second == &rc);
    ok &= (rc.refcount == 6);        // incremented
    ok &= (dst.first.buf == keybuf); // key copied

    // Case 2: src->second is null -> no increment, no crash
    Pair src2; src2.first.buf = keybuf; src2.second = 0;
    Pair dst2; dst2.first.buf = 0; dst2.second = (RefCounted*)0xbeef;
    Cons_val(&dst2, &src2);
    ok &= (g_copied == 2);
    ok &= (dst2.second == 0);

    // Case 3: self null -> no-op, no crash
    Cons_val(0, &src);
    ok &= (g_copied == 2);

    if (ok) printf("CONS_VAL_OK\n");
    else    printf("CONS_VAL_FAIL\n");
    return 0;
}