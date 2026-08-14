#include <cstdio>

struct C3DVector { float x, y, z; };
struct Thing { int tag; };

static void* __fastcall Thing_Helper(Thing* self, void* edx, C3DVector* pos)
{
    (void)edx;
    return (void*)((unsigned)self->tag + (unsigned)(int)pos->x);
}

struct Nav;
typedef bool (__fastcall *VSlot2)(Nav* self, void* edx, void* a);

struct Nav {
    void** vtable;
    int id;
};

static bool __fastcall slot2_impl(Nav* self, void* edx, void* a)
{
    (void)edx;
    return (((unsigned)a + (unsigned)self->id) & 1u) == 0u;
}

static bool __fastcall IsPosNavLineOfSightTo(Nav* self, void* edx, C3DVector* pos, Thing* arg1)
{
    (void)edx;
    void* r = Thing_Helper(arg1, 0, pos);
    VSlot2 fn = (VSlot2)self->vtable[2];
    return fn(self, 0, r);
}

int main()
{
    void* vt[4] = {0,0,(void*)&slot2_impl,0};
    Nav nav; nav.vtable = vt; nav.id = 3;
    Thing th; th.tag = 5;
    C3DVector p; p.x = 2.0f; p.y = 0; p.z = 0;
    bool b1 = IsPosNavLineOfSightTo(&nav, 0, &p, &th);
    C3DVector p2; p2.x = 3.0f; p2.y=0; p2.z=0;
    bool b2 = IsPosNavLineOfSightTo(&nav, 0, &p2, &th);
    if (b1 && !b2) { printf("LOS_PARITY_OK\n"); return 0; }
    printf("FAIL b1=%d b2=%d\n", (int)b1, (int)b2);
    return 1;
}