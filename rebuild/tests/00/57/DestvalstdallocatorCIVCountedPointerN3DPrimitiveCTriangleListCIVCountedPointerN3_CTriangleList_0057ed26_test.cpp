extern "C" int printf(const char*, ...);

struct CountedObj {
    virtual void slot0();
    virtual void slot1();
    int refcount;
};

struct CIVCountedPointer {
    CountedObj* p;
    void Dest_val();
};

static int g_destroyed = 0;

void CountedObj::slot0() {}
void CountedObj::slot1() {}

struct TestObj : CountedObj {
    void slot0() {}
    void slot1() { g_destroyed = 1; }
};

int main()
{
    // Case 1: refcount 2 -> becomes 1, no destroy, pointer cleared
    TestObj a;
    a.refcount = 2;
    CIVCountedPointer cp1;
    cp1.p = &a;
    cp1.Dest_val();
    bool ok1 = (a.refcount == 1) && (cp1.p == 0) && (g_destroyed == 0);

    // Case 2: refcount 1 -> becomes 0, destroy called, pointer cleared
    TestObj b;
    b.refcount = 1;
    g_destroyed = 0;
    CIVCountedPointer cp2;
    cp2.p = &b;
    cp2.Dest_val();
    bool ok2 = (b.refcount == 0) && (cp2.p == 0) && (g_destroyed == 1);

    // Case 3: null pointer -> no-op
    CIVCountedPointer cp3;
    cp3.p = 0;
    g_destroyed = 0;
    cp3.Dest_val();
    bool ok3 = (cp3.p == 0) && (g_destroyed == 0);

    if (ok1 && ok2 && ok3) {
        printf("DESTVAL_OK\n");
        return 0;
    }
    printf("FAIL %d %d %d\n", ok1, ok2, ok3);
    return 1;
}