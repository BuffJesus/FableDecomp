#include <cstdio>

struct A;
struct AV { long (__fastcall *m0)(A* self, long v); };
struct A { AV* vt; long base; };

struct P;
struct PV { long* (__fastcall *m0)(P* self); };
struct P { PV* vt; long val; };

struct CMap { void* pad0; A* a; };

long __fastcall GetWorldMapNumber(CMap* self, void* edx, P* p)
{
    long* r = p->vt->m0(p);
    return self->a->vt->m0(self->a, *r);
}

static long __fastcall A_m0(A* self, long v) { return self->base + v; }
static long* __fastcall P_m0(P* self) { return &self->val; }

int main()
{
    AV av; av.m0 = A_m0;
    A a; a.vt = &av; a.base = 100;
    CMap m; m.a = &a;

    PV pv; pv.m0 = P_m0;
    P p; p.vt = &pv; p.val = 23;

    long got = GetWorldMapNumber(&m, 0, &p);
    if (got == 123) printf("PARITY_OK\n");
    else printf("FAIL %ld\n", got);
    return 0;
}