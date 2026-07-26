#include "rebuild_abi.h"
#include <cstdio>

struct CountedObj {
    void** vtbl;
    int    refcount;
};
struct CIVCountedPointer { CountedObj* obj; };
typedef void (__fastcall *dtor_fn)(CountedObj*);

extern void __fastcall Dest_val_0043cd00(CIVCountedPointer* thisptr);

static int g_destroyed = 0;
void __fastcall fake_dtor(CountedObj* p) { (void)p; g_destroyed = 1; }

int main() {
    void* vtbl[2];
    vtbl[0] = 0;
    vtbl[1] = (void*)&fake_dtor;

    // case 1: refcount>1 -> just decrement, no dtor, slot nulled
    CountedObj a; a.vtbl = vtbl; a.refcount = 2;
    CIVCountedPointer pa; pa.obj = &a;
    Dest_val_0043cd00(&pa);
    int ok1 = (a.refcount == 1 && g_destroyed == 0 && pa.obj == 0);

    // case 2: refcount==1 -> dtor called
    g_destroyed = 0;
    CountedObj b; b.vtbl = vtbl; b.refcount = 1;
    CIVCountedPointer pb; pb.obj = &b;
    Dest_val_0043cd00(&pb);
    int ok2 = (g_destroyed == 1 && pb.obj == 0);

    // case 3: null pointer -> no-op
    CIVCountedPointer pc; pc.obj = 0;
    Dest_val_0043cd00(&pc);
    int ok3 = (pc.obj == 0);

    if (ok1 && ok2 && ok3) printf("PASS_0043cd00_OK\n");
    else printf("FAIL_0043cd00\n");
    return 0;
}