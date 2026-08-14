#include <cstdio>

typedef int EInventoryCategoryIdentifier;

struct CCounted;
struct CCountedVtbl {
    void (__fastcall *dtor)(CCounted*);
    void (__fastcall *release)(CCounted*);
};
struct CCounted {
    CCountedVtbl* vtbl;
    int refcount;
    char pad[0x24];
    int identifier;
};
struct CCategory {
    char pad[0xc];
    CCounted* counted;
};

static int g_released = 0;
static void __fastcall my_release(CCounted* p) { g_released = 1; }

EInventoryCategoryIdentifier __fastcall GetCategoryIdentifier(CCategory* self)
{
    CCounted* p = self->counted;
    if (p) { ++p->refcount; }
    EInventoryCategoryIdentifier result = p->identifier;
    if (--p->refcount == 0) { p->vtbl->release(p); }
    return result;
}

int main() {
    CCountedVtbl vt; vt.dtor = 0; vt.release = my_release;
    CCounted c; c.vtbl = &vt; c.refcount = 5; c.identifier = 42;
    CCategory cat; cat.counted = &c;

    // refcount 5 -> inc 6 -> dec 5, not zero, no release; return 42
    int r = GetCategoryIdentifier(&cat);
    if (r != 42) { printf("FAIL r=%d\n", r); return 1; }
    if (g_released != 0) { printf("FAIL released early\n"); return 1; }
    if (c.refcount != 5) { printf("FAIL rc=%d\n", c.refcount); return 1; }

    // refcount 0 -> inc 1 -> dec 0 -> release; return identifier
    g_released = 0;
    c.refcount = 0; c.identifier = 99;
    r = GetCategoryIdentifier(&cat);
    if (r != 99) { printf("FAIL r2=%d\n", r); return 1; }
    if (g_released != 1) { printf("FAIL not released\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}