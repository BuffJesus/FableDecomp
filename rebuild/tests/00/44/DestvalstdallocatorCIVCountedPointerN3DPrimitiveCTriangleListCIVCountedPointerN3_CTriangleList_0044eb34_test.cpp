#include <cstdio>

struct CountedObj { void (__fastcall **vtbl)(CountedObj*); long refcount; };
struct CIVCountedPointer { CountedObj* obj; };

static int g_freed = 0;
static void __fastcall freefn(CountedObj* o) { g_freed = 1; }

void __fastcall Dest_val(CIVCountedPointer* self, void* alloc)
{
    CountedObj* o = self->obj;
    if (o) {
        if (--o->refcount == 0) {
            o->vtbl[1](o);
        }
        self->obj = 0;
    }
}

int main()
{
    void (__fastcall *vt[2])(CountedObj*) = {0, freefn};
    CountedObj o;
    o.vtbl = vt;
    o.refcount = 2;
    CIVCountedPointer p; p.obj = &o;

    /* refcount 2 -> 1, not freed, pointer cleared */
    Dest_val(&p, 0);
    if (o.refcount != 1) { std::printf("FAIL refcount\n"); return 1; }
    if (g_freed != 0) { std::printf("FAIL freed early\n"); return 1; }
    if (p.obj != 0) { std::printf("FAIL not cleared\n"); return 1; }

    /* now refcount 1 -> 0, freed */
    o.refcount = 1;
    p.obj = &o;
    Dest_val(&p, 0);
    if (g_freed != 1) { std::printf("FAIL not freed\n"); return 1; }
    if (p.obj != 0) { std::printf("FAIL not cleared2\n"); return 1; }

    /* null obj: no-op */
    CIVCountedPointer n; n.obj = 0;
    Dest_val(&n, 0);

    std::printf("_Dest_val_0044eb34_TEST PASS\n");
    return 0;
}