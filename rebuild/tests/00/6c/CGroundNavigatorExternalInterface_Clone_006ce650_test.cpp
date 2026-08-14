#include <stdio.h>
#include <stdlib.h>

struct Iface { int a, b, c; };

static int g_ctor_called = 0;

static void* my_new(unsigned int sz) { return malloc(sz); }
static bool g_fail_new = false;

struct Self { int x, y, z; };

static Iface* Construct(Iface* dst, Self* src) {
    g_ctor_called++;
    dst->a = src->x;
    dst->b = src->y;
    dst->c = src->z;
    return dst;
}

static Iface* Clone(Self* self) {
    void* p = g_fail_new ? 0 : my_new(12);
    if (p) {
        return Construct((Iface*)p, self);
    }
    return 0;
}

int main() {
    Self s; s.x = 7; s.y = 8; s.z = 9;

    Iface* r = Clone(&s);
    if (!r) { printf("FAIL: null on success\n"); return 1; }
    if (g_ctor_called != 1) { printf("FAIL: ctor not called once\n"); return 1; }
    if (r->a != 7 || r->b != 8 || r->c != 9) { printf("FAIL: fields\n"); return 1; }
    free(r);

    g_fail_new = true;
    g_ctor_called = 0;
    Iface* r2 = Clone(&s);
    if (r2 != 0) { printf("FAIL: not null on alloc fail\n"); return 1; }
    if (g_ctor_called != 0) { printf("FAIL: ctor called on fail\n"); return 1; }

    printf("CLONE_OK\n");
    return 0;
}