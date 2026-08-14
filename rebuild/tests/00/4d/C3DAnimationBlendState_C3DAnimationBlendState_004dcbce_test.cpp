#include <cstdio>

struct RefCounted {
    void** vtbl;
    long   refCnt;
};

static int g_released = 0;

void __fastcall release_impl(RefCounted* self) {
    (void)self;
    g_released++;
}

static int g_baseRan = 0;

struct Model {
    void*       base_vtbl;
    char        pad[0x0C];
    RefCounted* rc;
};

void model_dtor(Model* self)
{
    RefCounted* rc = self->rc;
    if (rc)
    {
        if (--rc->refCnt == 0)
        {
            typedef void (__fastcall *Fn)(RefCounted*);
            ((Fn)(rc->vtbl[1]))(rc);
        }
        self->rc = 0;
    }
    g_baseRan++;   // stand-in for base dtor
}

int main()
{
    // vtbl with slot[1] = release_impl
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&release_impl;

    // Case 1: refCnt 2 -> decremented to 1, no release, rc cleared, base runs
    RefCounted rc1; rc1.vtbl = (void**)vt; rc1.refCnt = 2;
    Model m1; m1.rc = &rc1;
    model_dtor(&m1);
    bool ok1 = (rc1.refCnt == 1) && (g_released == 0) && (m1.rc == 0) && (g_baseRan == 1);

    // Case 2: refCnt 1 -> decremented to 0 -> release called, rc cleared
    RefCounted rc2; rc2.vtbl = (void**)vt; rc2.refCnt = 1;
    Model m2; m2.rc = &rc2;
    model_dtor(&m2);
    bool ok2 = (rc2.refCnt == 0) && (g_released == 1) && (m2.rc == 0) && (g_baseRan == 2);

    // Case 3: null rc -> just base runs
    Model m3; m3.rc = 0;
    model_dtor(&m3);
    bool ok3 = (m3.rc == 0) && (g_baseRan == 3);

    if (ok1 && ok2 && ok3)
        printf("PARITY_OK_004dcbce\n");
    else
        printf("FAIL o1=%d o2=%d o3=%d\n", ok1, ok2, ok3);
    return 0;
}