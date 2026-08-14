#include <stdio.h>

// Behaviour model of ~C3DAnimationBlendState

struct IRefCounted {
    struct VTable { void (*slot0)(void*); void (*slot1)(void*); };
    VTable* vtbl;
    int     refcount;
};

static int g_freed = 0;
static void freefn(void* p) { (void)p; g_freed++; }

static int g_baseDtor = 0;

struct State {
    void* basefield;
    char pad0[8];
    IRefCounted* res;
};

static void baseDtor(State* s) { (void)s; g_baseDtor++; }

static void dtor(State* self)
{
    IRefCounted* r = self->res;
    if (r) {
        if (--r->refcount == 0) {
            r->vtbl->slot1(r);
        }
        self->res = 0;
    }
    baseDtor(self);
}

int main()
{
    IRefCounted::VTable vt = { 0, freefn };

    IRefCounted r1 = { &vt, 1 };
    State s1; s1.res = &r1;
    dtor(&s1);
    if (g_freed != 1) { printf("FAIL freed=%d\n", g_freed); return 1; }
    if (r1.refcount != 0) { printf("FAIL rc=%d\n", r1.refcount); return 1; }
    if (s1.res != 0) { printf("FAIL notnull\n"); return 1; }

    IRefCounted r2 = { &vt, 2 };
    State s2; s2.res = &r2;
    dtor(&s2);
    if (g_freed != 1) { printf("FAIL freed2=%d\n", g_freed); return 1; }
    if (r2.refcount != 1) { printf("FAIL rc2=%d\n", r2.refcount); return 1; }
    if (s2.res != 0) { printf("FAIL notnull2\n"); return 1; }

    State s3; s3.res = 0;
    dtor(&s3);
    if (g_freed != 1) { printf("FAIL freed3=%d\n", g_freed); return 1; }

    if (g_baseDtor != 3) { printf("FAIL base=%d\n", g_baseDtor); return 1; }

    printf("BLENDSTATE_DTOR_OK\n");
    return 0;
}