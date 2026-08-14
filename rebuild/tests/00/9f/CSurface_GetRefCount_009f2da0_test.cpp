#include <cstdio>

struct IRefObj;
typedef long (__stdcall *PFN)(IRefObj*);
struct IRefVtbl { PFN slot0; PFN slot1; PFN slot2; };
struct IRefObj { IRefVtbl* vtbl; };
struct CSurface { void* pad0; IRefObj* obj; };

static int g_slot1_calls = 0;
static long __stdcall impl_slot1(IRefObj*) { g_slot1_calls++; return 999; }
static long __stdcall impl_slot2(IRefObj*) { return 42; }

long __fastcall GetRefCount(CSurface* self)
{
    IRefObj* p = self->obj;
    if (p) {
        p->vtbl->slot1(p);
        IRefObj* q = self->obj;
        return q->vtbl->slot2(q);
    }
    return 0;
}

int main()
{
    IRefVtbl vt = { 0, impl_slot1, impl_slot2 };
    IRefObj o = { &vt };
    CSurface s1 = { 0, &o };
    CSurface s0 = { 0, 0 };

    long r1 = GetRefCount(&s1);
    long r0 = GetRefCount(&s0);

    bool ok = (r1 == 42) && (r0 == 0) && (g_slot1_calls == 1);
    if (ok) printf("PARITY_OK\n");
    else printf("FAIL r1=%ld r0=%ld c=%d\n", r1, r0, g_slot1_calls);
    return 0;
}