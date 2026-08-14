#include <cstdio>

struct SubObj {
    char pad[0x6c];
    unsigned char flags;
};

struct CTCPhysicsBase;
typedef int (__fastcall *SlotFn)(CTCPhysicsBase* self);

struct CTCPhysicsBaseVtbl {
    char pad[0x174];
    SlotFn slot174;
};

struct CTCPhysicsBase {
    CTCPhysicsBaseVtbl* vtbl;
    SubObj* sub;
};

bool __fastcall IsPhysicsMeshTypeCylinder(CTCPhysicsBase* self)
{
    if (self->vtbl->slot174(self) == 0 && (self->sub->flags & 8))
        return true;
    return false;
}

static int g_ret = 0;
int __fastcall stub(CTCPhysicsBase* self) { return g_ret; }

int main()
{
    CTCPhysicsBaseVtbl vt;
    vt.slot174 = stub;
    SubObj s;
    s.flags = 0;
    CTCPhysicsBase o;
    o.vtbl = &vt;
    o.sub = &s;

    // slot returns nonzero -> false regardless of flags
    g_ret = 1; s.flags = 8;
    if (IsPhysicsMeshTypeCylinder(&o) != false) { printf("BAD1\n"); return 1; }

    // slot returns 0, flag 8 set -> true
    g_ret = 0; s.flags = 8;
    if (IsPhysicsMeshTypeCylinder(&o) != true) { printf("BAD2\n"); return 1; }

    // slot returns 0, flag 8 clear -> false
    g_ret = 0; s.flags = 0;
    if (IsPhysicsMeshTypeCylinder(&o) != false) { printf("BAD3\n"); return 1; }

    // slot returns 0, other bits set but not 8 -> false
    g_ret = 0; s.flags = 0x07;
    if (IsPhysicsMeshTypeCylinder(&o) != false) { printf("BAD4\n"); return 1; }

    printf("PARITY_OK_006b0170\n");
    return 0;
}