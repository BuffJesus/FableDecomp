#include <stdio.h>

static int g_released = 0;

struct CAnimComponentBase {
    virtual void slot0() {}
    virtual void Release() { g_released = 1; }
    int refCount;
};

struct DestPair {
    int                 animComponent;
    CAnimComponentBase* inner;
    void Dest_val();
};

void DestPair::Dest_val()
{
    CAnimComponentBase* p = this->inner;
    if (p != 0) {
        if (--p->refCount == 0) {
            p->Release();
        }
        this->inner = 0;
    }
}

int main()
{
    int ok = 1;

    // Case 1: inner null -> no-op, no release.
    {
        DestPair d; d.animComponent = 3; d.inner = 0;
        g_released = 0;
        d.Dest_val();
        if (d.inner != 0) ok = 0;
        if (g_released != 0) ok = 0;
    }

    // Case 2: refcount > 1 -> decrement only, no release, inner nulled.
    {
        CAnimComponentBase obj; obj.refCount = 2;
        DestPair d; d.animComponent = 7; d.inner = &obj;
        g_released = 0;
        d.Dest_val();
        if (obj.refCount != 1) ok = 0;
        if (g_released != 0) ok = 0;
        if (d.inner != 0) ok = 0;
    }

    // Case 3: refcount == 1 -> decrement to 0, release called, inner nulled.
    {
        CAnimComponentBase obj; obj.refCount = 1;
        DestPair d; d.animComponent = 9; d.inner = &obj;
        g_released = 0;
        d.Dest_val();
        if (obj.refCount != 0) ok = 0;
        if (g_released != 1) ok = 0;
        if (d.inner != 0) ok = 0;
    }

    if (ok) printf("DEST_VAL_OK\n");
    else    printf("DEST_VAL_FAIL\n");
    return ok ? 0 : 1;
}