#include <cstdio>

struct CAnimComponentBase {
    virtual void v0() = 0;
    virtual void v1() = 0;
    int refcount;
};

struct DestPair {
    char pad[0x18];
    CAnimComponentBase* p;
    void Dest_val();
};

static int g_destroyed = 0;

struct Impl : CAnimComponentBase {
    void v0() {}
    void v1() { g_destroyed++; }
};

int main()
{
    // Case 1: refcount 2 -> becomes 1, no destroy, ptr cleared
    Impl a; a.refcount = 2;
    DestPair d1; d1.p = &a;
    d1.Dest_val();
    if (a.refcount != 1) { printf("FAIL rc1\n"); return 1; }
    if (d1.p != 0) { printf("FAIL ptr1\n"); return 1; }
    if (g_destroyed != 0) { printf("FAIL destroy1\n"); return 1; }

    // Case 2: refcount 1 -> becomes 0, v1 called
    Impl b; b.refcount = 1;
    DestPair d2; d2.p = &b;
    d2.Dest_val();
    if (b.refcount != 0) { printf("FAIL rc2\n"); return 1; }
    if (d2.p != 0) { printf("FAIL ptr2\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL destroy2\n"); return 1; }

    // Case 3: null ptr -> no-op
    DestPair d3; d3.p = 0;
    d3.Dest_val();
    if (d3.p != 0) { printf("FAIL ptr3\n"); return 1; }

    printf("DESTVAL_OK\n");
    return 0;
}