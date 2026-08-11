#include <cstdio>

struct Counted {
    virtual void v0() {}
    virtual void v1() {}
    int refcount;
};

struct AnimPair {
    int first;
    Counted* ptr;
};

void __fastcall _Dest_val(AnimPair* self, int edx);

static int g_released = 0;

struct RealCounted : Counted {
    virtual void v0() {}
    virtual void v1() { g_released = 1; }
};

int main()
{
    // Case 1: refcount > 1 -> just decrement, no release, ptr nulled.
    RealCounted a; a.refcount = 3;
    AnimPair p1; p1.first = 5; p1.ptr = &a;
    g_released = 0;
    _Dest_val(&p1, 0);
    bool ok1 = (a.refcount == 2) && (g_released == 0) && (p1.ptr == 0);

    // Case 2: refcount == 1 -> decrement to 0, release called, ptr nulled.
    RealCounted b; b.refcount = 1;
    AnimPair p2; p2.first = 7; p2.ptr = &b;
    g_released = 0;
    _Dest_val(&p2, 0);
    bool ok2 = (b.refcount == 0) && (g_released == 1) && (p2.ptr == 0);

    // Case 3: null ptr -> nothing happens.
    AnimPair p3; p3.first = 9; p3.ptr = 0;
    g_released = 0;
    _Dest_val(&p3, 0);
    bool ok3 = (g_released == 0) && (p3.ptr == 0);

    if (ok1 && ok2 && ok3) {
        printf("DESTVAL_OK\n");
        return 0;
    }
    printf("DESTVAL_FAIL %d %d %d\n", ok1, ok2, ok3);
    return 1;
}