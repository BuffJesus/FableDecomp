#include <cstdio>
struct RefObj;
struct RefVtbl { void (*dtor0)(RefObj*); void (*release)(RefObj*); };
struct RefObj { RefVtbl* vtbl; int refcnt; };
struct BlendState { char pad[0xc]; RefObj* ptr; };
static int g_released = 0;
static void do_release(RefObj* p) { g_released++; }
static RefVtbl g_vtbl = { 0, do_release };
static int g_base = 0;
static void Base_dtor(BlendState* s) { g_base++; }
static void dtor(BlendState* self) {
    RefObj* p = self->ptr;
    if (p) { if (--p->refcnt == 0) p->vtbl->release(p); self->ptr = 0; }
    Base_dtor(self);
}
int main() {
    BlendState a; a.ptr = 0; dtor(&a);
    RefObj o2; o2.vtbl = &g_vtbl; o2.refcnt = 2; BlendState b; b.ptr = &o2; dtor(&b);
    RefObj o3; o3.vtbl = &g_vtbl; o3.refcnt = 1; BlendState c; c.ptr = &o3; dtor(&c);
    if (g_released == 1 && g_base == 3 && b.ptr == 0 && c.ptr == 0 && o2.refcnt == 1) printf("BLENDDTOR_OK\n");
    else printf("FAIL r=%d b=%d\n", g_released, g_base);
    return 0;
}