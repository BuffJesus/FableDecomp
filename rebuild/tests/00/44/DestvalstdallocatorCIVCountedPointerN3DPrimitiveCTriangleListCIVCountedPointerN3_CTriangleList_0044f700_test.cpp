#include <cstdio>

struct RefCounted {
    void** vtbl;
    int refcount;
};

struct CIVCountedPointer {
    RefCounted* p;
};

void __fastcall Dest_val(CIVCountedPointer* self, void* alloc) {
    RefCounted* p = self->p;
    if (p) {
        if (--p->refcount == 0) {
            (*(void (__fastcall**)(RefCounted*))((char*)p->vtbl + 4))(p);
        }
        self->p = 0;
    }
}

static int g_destroyed = 0;
void __fastcall dtor_stub(RefCounted* self) { g_destroyed = 1; }

int main() {
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&dtor_stub;

    RefCounted obj;
    obj.vtbl = vt;
    obj.refcount = 2;

    CIVCountedPointer cp;
    cp.p = &obj;

    // refcount 2 -> 1, no destroy, pointer NOT cleared? Actually cleared always when non-null.
    Dest_val(&cp, 0);
    if (obj.refcount != 1) { std::printf("FAIL refcount1 %d\n", obj.refcount); return 1; }
    if (g_destroyed != 0) { std::printf("FAIL destroyed early\n"); return 1; }
    if (cp.p != 0) { std::printf("FAIL ptr not cleared\n"); return 1; }

    // second: refcount 1 -> 0, destroy
    obj.refcount = 1;
    cp.p = &obj;
    Dest_val(&cp, 0);
    if (obj.refcount != 0) { std::printf("FAIL refcount0 %d\n", obj.refcount); return 1; }
    if (g_destroyed != 1) { std::printf("FAIL not destroyed\n"); return 1; }
    if (cp.p != 0) { std::printf("FAIL ptr not cleared2\n"); return 1; }

    // null case: nothing happens
    CIVCountedPointer cn;
    cn.p = 0;
    Dest_val(&cn, 0);
    if (cn.p != 0) { std::printf("FAIL null\n"); return 1; }

    std::printf("DestVal_0044f700_TEST PASS\n");
    return 0;
}