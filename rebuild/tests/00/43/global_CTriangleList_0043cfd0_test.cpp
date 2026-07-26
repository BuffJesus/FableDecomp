#include "rebuild_abi.h"
#include <cstdio>

struct CountedObj {
    void** vtbl;
    int    refcnt;
};
struct CountedPtr {
    CountedObj* p;
};

void __fastcall Dest_val_0043cfd0(CountedPtr* thisptr);

static int g_dtor_called = 0;
static void __fastcall fake_dtor(CountedObj* o) { (void)o; g_dtor_called = 1; }

int main()
{
    // Case 1: refcount > 1, should just decrement, no dtor, ptr nulled.
    void* vt1[2]; vt1[0] = 0; vt1[1] = (void*)&fake_dtor;
    CountedObj o1; o1.vtbl = (void**)vt1; o1.refcnt = 3;
    CountedPtr wp1; wp1.p = &o1;
    Dest_val_0043cfd0(&wp1);
    int ok1 = (o1.refcnt == 2) && (wp1.p == 0) && (g_dtor_called == 0);

    // Case 2: refcount == 1 -> hits zero, dtor invoked, ptr nulled.
    g_dtor_called = 0;
    void* vt2[2]; vt2[0] = 0; vt2[1] = (void*)&fake_dtor;
    CountedObj o2; o2.vtbl = (void**)vt2; o2.refcnt = 1;
    CountedPtr wp2; wp2.p = &o2;
    Dest_val_0043cfd0(&wp2);
    int ok2 = (o2.refcnt == 0) && (wp2.p == 0) && (g_dtor_called == 1);

    // Case 3: null pointer -> no-op, no crash.
    CountedPtr wp3; wp3.p = 0;
    Dest_val_0043cfd0(&wp3);
    int ok3 = (wp3.p == 0);

    if (ok1 && ok2 && ok3)
        printf("PASS_0043cfd0_OK\n");
    else
        printf("FAIL_0043cfd0 %d %d %d\n", ok1, ok2, ok3);
    return 0;
}