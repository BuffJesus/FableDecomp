#include <cstdio>

struct Referenced {
    void** vtbl;
    int    refcount;
    char   pad[0x58];
    float  animSpeed;
};

struct CTCWeapon {
    char        pad[0x70];
    Referenced* target;
};

typedef void (__fastcall *rel_t)(Referenced*);

static int g_released = 0;

static void __fastcall fake_release(Referenced* p) {
    (void)p;
    g_released = 1;
}

float __fastcall CTCWeapon_GetAnimationSpeedValue(CTCWeapon* self)
{
    Referenced* p = self->target;
    if (p != 0)
        p->refcount++;
    float v = p->animSpeed;
    if (--p->refcount == 0)
        ((rel_t)p->vtbl[1])(p);
    return v;
}

int main()
{
    void* vt[2];
    vt[1] = (void*)&fake_release;

    Referenced ref;
    ref.vtbl = (void**)vt;
    ref.refcount = 5;      // held elsewhere -> no release
    ref.animSpeed = 2.5f;

    CTCWeapon w;
    w.target = &ref;

    float r = CTCWeapon_GetAnimationSpeedValue(&w);
    bool ok1 = (r == 2.5f) && (ref.refcount == 5) && (g_released == 0);

    // last-ref case: refcount 1 -> inc to 2 -> dec to 1 (not zero)?
    // scoped hold: inc then dec => net zero. Start at 0 so dec goes to 0 -> release.
    Referenced ref2;
    ref2.vtbl = (void**)vt;
    ref2.refcount = 0;     // inc->1, read, dec->0 -> release
    ref2.animSpeed = 7.0f;
    CTCWeapon w2;
    w2.target = &ref2;
    g_released = 0;
    float r2 = CTCWeapon_GetAnimationSpeedValue(&w2);
    bool ok2 = (r2 == 7.0f) && (g_released == 1);

    if (ok1 && ok2)
        printf("PARITY_OK_006625c0\n");
    else
        printf("FAIL r=%f rc=%d rel=%d r2=%f rel2=%d\n", r, ref.refcount, g_released, r2, g_released);
    return 0;
}