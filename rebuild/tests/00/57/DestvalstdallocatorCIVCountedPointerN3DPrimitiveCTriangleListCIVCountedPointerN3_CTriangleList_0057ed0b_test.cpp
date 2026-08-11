#include <cstdio>

struct CTriangleList {
    virtual void slot0();
    virtual void destroy();
    int refcount;
};

struct CIVCountedPointer {
    CTriangleList* ptr;
};

void CTriangleList::slot0() {}
void CTriangleList::destroy() {}

// Local standalone definition (mirrors the reconstructed logic).
void __fastcall Dest_val(CIVCountedPointer* self)
{
    CTriangleList* p = self->ptr;
    if (p != 0) {
        if (--p->refcount == 0) {
            p->destroy();
        }
        self->ptr = 0;
    }
}

static int g_destroyed = 0;

struct TL : CTriangleList {
    void slot0() {}
    void destroy() { g_destroyed++; }
};

int main()
{
    // Case 1: refcount 2 -> decremented to 1, not destroyed, ptr cleared.
    TL a; a.refcount = 2;
    CIVCountedPointer h1; h1.ptr = &a;
    Dest_val(&h1);
    bool ok1 = (a.refcount == 1) && (h1.ptr == 0) && (g_destroyed == 0);

    // Case 2: refcount 1 -> decremented to 0, destroy() called, ptr cleared.
    TL b; b.refcount = 1;
    CIVCountedPointer h2; h2.ptr = &b;
    Dest_val(&h2);
    bool ok2 = (b.refcount == 0) && (h2.ptr == 0) && (g_destroyed == 1);

    // Case 3: null ptr -> nothing happens.
    CIVCountedPointer h3; h3.ptr = 0;
    Dest_val(&h3);
    bool ok3 = (h3.ptr == 0) && (g_destroyed == 1);

    if (ok1 && ok2 && ok3) {
        printf("DESTVAL_OK\n");
        return 0;
    }
    printf("DESTVAL_FAIL %d %d %d\n", (int)ok1, (int)ok2, (int)ok3);
    return 1;
}