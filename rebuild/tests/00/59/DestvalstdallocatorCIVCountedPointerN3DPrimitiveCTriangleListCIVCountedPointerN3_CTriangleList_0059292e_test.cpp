#include <cstdio>

struct CountedObj {
    virtual void slot0() {}
    virtual void slot1() {}
    long refcount;
};
struct CIVCountedPointer {
    CountedObj* ptr;
};
void __fastcall _Dest_val(CIVCountedPointer* self, CIVCountedPointer* val);

static int g_destroyed = 0;
struct Impl : CountedObj {
    void slot0() {}
    void slot1() { g_destroyed = 1; }
};

int main()
{
    // Case 1: null ptr -> no-op
    CIVCountedPointer a; a.ptr = 0;
    _Dest_val(&a, &a);

    // Case 2: refcount 2 -> decrement to 1, no destroy, slot nulled
    Impl obj2; obj2.refcount = 2;
    CIVCountedPointer b; b.ptr = &obj2;
    g_destroyed = 0;
    _Dest_val(&b, &b);
    bool ok2 = (obj2.refcount == 1) && (g_destroyed == 0) && (b.ptr == 0);

    // Case 3: refcount 1 -> decrement to 0, destroy called, slot nulled
    Impl obj3; obj3.refcount = 1;
    CIVCountedPointer c; c.ptr = &obj3;
    g_destroyed = 0;
    _Dest_val(&c, &c);
    bool ok3 = (obj3.refcount == 0) && (g_destroyed == 1) && (c.ptr == 0);

    if (ok2 && ok3) {
        printf("DESTVAL_OK\n");
        return 0;
    }
    printf("DESTVAL_FAIL o2=%d o3=%d\n", (int)ok2, (int)ok3);
    return 1;
}