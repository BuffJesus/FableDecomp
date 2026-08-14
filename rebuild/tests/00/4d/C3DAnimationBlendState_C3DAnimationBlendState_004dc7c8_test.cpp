#include <cstdio>

struct RefObj;
struct RefVtbl {
    void (*release_slot1)(RefObj*);
};
struct RefObj {
    RefVtbl* vtbl;
    int      count;
};

static int g_released = 0;
static void do_release(RefObj*) { g_released = 1; }
static RefVtbl g_vt = { do_release };

struct Blend {
    char pad0[0xc];
    RefObj* ptr;
};

static void run_dtor(Blend* self)
{
    RefObj* p = self->ptr;
    if (p) {
        if (--p->count == 0) {
            p->vtbl->release_slot1(p);
        }
        self->ptr = 0;
    }
}

int main()
{
    {
        Blend b; b.ptr = 0;
        run_dtor(&b);
        if (b.ptr != 0) { printf("FAIL null\n"); return 1; }
    }
    {
        RefObj o; o.vtbl = &g_vt; o.count = 3;
        Blend b; b.ptr = &o;
        g_released = 0;
        run_dtor(&b);
        if (o.count != 2) { printf("FAIL dec\n"); return 1; }
        if (g_released != 0) { printf("FAIL earlyrel\n"); return 1; }
        if (b.ptr != 0) { printf("FAIL ptrclear\n"); return 1; }
    }
    {
        RefObj o; o.vtbl = &g_vt; o.count = 1;
        Blend b; b.ptr = &o;
        g_released = 0;
        run_dtor(&b);
        if (o.count != 0) { printf("FAIL zero\n"); return 1; }
        if (g_released != 1) { printf("FAIL norel\n"); return 1; }
        if (b.ptr != 0) { printf("FAIL ptrclear2\n"); return 1; }
    }
    printf("BLENDDTOR_OK\n");
    return 0;
}