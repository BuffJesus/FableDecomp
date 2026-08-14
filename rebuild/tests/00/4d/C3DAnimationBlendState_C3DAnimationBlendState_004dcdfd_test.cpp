#include <cstdio>

typedef void (*ReleaseFn)(void* self);

struct RefCounted {
    ReleaseFn* vtbl;
    int        refcount;
};

struct State {
    void*       pad0;
    void*       pad4;
    void*       pad8;
    RefCounted* m_c;
};

static int g_released = 0;
static void release_impl(void* self) { (void)self; g_released++; }

static void run_dtor_body(State* self)
{
    RefCounted* c = self->m_c;
    if (c) {
        if (--c->refcount == 0) {
            c->vtbl[1](c);
        }
        self->m_c = 0;
    }
}

int main()
{
    ReleaseFn vt[2] = { 0, release_impl };

    RefCounted rc; rc.vtbl = vt; rc.refcount = 1;
    State s; s.m_c = &rc;
    run_dtor_body(&s);
    if (g_released != 1) { printf("FAIL relA\n"); return 1; }
    if (s.m_c != 0)      { printf("FAIL clrA\n"); return 1; }

    RefCounted rc2; rc2.vtbl = vt; rc2.refcount = 3;
    State s2; s2.m_c = &rc2;
    run_dtor_body(&s2);
    if (g_released != 1)   { printf("FAIL relB\n"); return 1; }
    if (rc2.refcount != 2) { printf("FAIL decB\n"); return 1; }
    if (s2.m_c != 0)       { printf("FAIL clrB\n"); return 1; }

    State s3; s3.m_c = 0;
    run_dtor_body(&s3);
    if (g_released != 1) { printf("FAIL nullC\n"); return 1; }

    printf("DTOR_BLEND_OK\n");
    return 0;
}