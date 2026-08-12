#include <cstdio>

struct CountedObject {
    virtual void v0() {}
    virtual void v1();   // release
    int refcount;
};

struct CIVCountedPointer {
    CountedObject* p;
};

void __fastcall CIVCountedPointer_Dest_val(CIVCountedPointer* self);

static int g_released = 0;
void CountedObject::v1() { g_released = 1; }

int main()
{
    // Case 1: refcount > 1 -> just decrement, no release, pointer nulled.
    CountedObject a;
    a.refcount = 3;
    CIVCountedPointer pa; pa.p = &a;
    g_released = 0;
    CIVCountedPointer_Dest_val(&pa);
    if (a.refcount != 2) { printf("FAIL refcount %d\n", a.refcount); return 1; }
    if (g_released != 0) { printf("FAIL released early\n"); return 1; }
    if (pa.p != 0) { printf("FAIL not nulled\n"); return 1; }

    // Case 2: refcount == 1 -> decrement to 0, release called.
    CountedObject b;
    b.refcount = 1;
    CIVCountedPointer pb; pb.p = &b;
    g_released = 0;
    CIVCountedPointer_Dest_val(&pb);
    if (b.refcount != 0) { printf("FAIL b refcount %d\n", b.refcount); return 1; }
    if (g_released != 1) { printf("FAIL b not released\n"); return 1; }
    if (pb.p != 0) { printf("FAIL b not nulled\n"); return 1; }

    // Case 3: null pointer -> no crash, stays null.
    CIVCountedPointer pc; pc.p = 0;
    CIVCountedPointer_Dest_val(&pc);
    if (pc.p != 0) { printf("FAIL c not null\n"); return 1; }

    printf("DESTVAL_OK\n");
    return 0;
}