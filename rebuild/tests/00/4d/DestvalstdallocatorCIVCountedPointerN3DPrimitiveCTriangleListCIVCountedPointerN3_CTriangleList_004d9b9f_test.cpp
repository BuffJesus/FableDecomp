#include <cstdio>

static int g_destroyed = 0;

struct CountedBody {
    virtual void slot0() {}
    virtual void slot1_destroy() {}
    int refcount;
};

struct CIVCountedPointer {
    CountedBody* ptr;
};

void __fastcall Dest_val_release(CIVCountedPointer* self);

struct RealBody : CountedBody {
    void slot0() {}
    void slot1_destroy() { g_destroyed = 1; }
};

int main()
{
    // Case 1: refcount > 1 -> just decrement, null out, no destroy.
    RealBody b1; b1.refcount = 3;
    CIVCountedPointer p1; p1.ptr = &b1;
    g_destroyed = 0;
    Dest_val_release(&p1);
    bool ok1 = (b1.refcount == 2) && (p1.ptr == 0) && (g_destroyed == 0);

    // Case 2: refcount == 1 -> decrement to 0, destroy, null out.
    RealBody b2; b2.refcount = 1;
    CIVCountedPointer p2; p2.ptr = &b2;
    g_destroyed = 0;
    Dest_val_release(&p2);
    bool ok2 = (b2.refcount == 0) && (p2.ptr == 0) && (g_destroyed == 1);

    // Case 3: null ptr -> nothing happens.
    CIVCountedPointer p3; p3.ptr = 0;
    g_destroyed = 0;
    Dest_val_release(&p3);
    bool ok3 = (p3.ptr == 0) && (g_destroyed == 0);

    if (ok1 && ok2 && ok3) {
        printf("DESTVAL_OK\n");
        return 0;
    }
    printf("DESTVAL_FAIL %d %d %d\n", (int)ok1, (int)ok2, (int)ok3);
    return 1;
}