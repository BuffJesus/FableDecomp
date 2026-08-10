#include <cstdio>

struct Child;
struct ChildVtbl { void (__fastcall *slot0)(Child* self); };
struct Child { ChildVtbl* vtbl; };
struct CEnginePrimitiveHandle;
struct CEnginePrimitiveRenderer {
    char pad[0x10];
    Child* child;
};

void __fastcall ClearAllChildPrimitives(CEnginePrimitiveRenderer* self, CEnginePrimitiveHandle*);

static int g_called = 0;
static Child* g_called_on = 0;
static void __fastcall my_slot0(Child* self) { g_called++; g_called_on = self; }

int main() {
    ChildVtbl vt; vt.slot0 = my_slot0;
    Child c; c.vtbl = &vt;
    CEnginePrimitiveRenderer r;
    r.child = &c;

    ClearAllChildPrimitives(&r, 0);
    bool ok1 = (g_called == 1) && (g_called_on == &c) && (r.child == 0);

    // null path: no call, stays null
    g_called = 0;
    r.child = 0;
    ClearAllChildPrimitives(&r, 0);
    bool ok2 = (g_called == 0) && (r.child == 0);

    if (ok1 && ok2) printf("00a0b6c0_TEST PASS\n");
    else printf("FAIL c=%d r=%p\n", g_called, (void*)r.child);
    return 0;
}