#include <cstdio>

struct CCounted {
    virtual void f0();
    virtual void destroy();
    int refcount;
};

struct CIVCountedPointer {
    CCounted* p;
    void _Dest_val();
};

static int g_destroyed = 0;

void CCounted::f0() {}
void CCounted::destroy() { g_destroyed++; }

struct Impl : CCounted {
    virtual void f0() {}
    virtual void destroy() { g_destroyed++; }
};

int main()
{
    // Case 1: refcount 2 -> decrements to 1, no destroy, pointer cleared.
    Impl a;
    a.refcount = 2;
    CIVCountedPointer cp1;
    cp1.p = &a;
    cp1._Dest_val();
    bool ok1 = (a.refcount == 1) && (cp1.p == 0) && (g_destroyed == 0);

    // Case 2: refcount 1 -> decrements to 0, destroy called, cleared.
    Impl b;
    b.refcount = 1;
    CIVCountedPointer cp2;
    cp2.p = &b;
    cp2._Dest_val();
    bool ok2 = (b.refcount == 0) && (cp2.p == 0) && (g_destroyed == 1);

    // Case 3: null pointer -> no-op.
    CIVCountedPointer cp3;
    cp3.p = 0;
    cp3._Dest_val();
    bool ok3 = (cp3.p == 0) && (g_destroyed == 1);

    if (ok1 && ok2 && ok3) {
        printf("DESTVAL_OK\n");
        return 0;
    }
    printf("DESTVAL_FAIL\n");
    return 1;
}