#include <cstdio>

struct RefObj {
    void (__fastcall **vtbl)(RefObj*);
    long refcount;
};

struct SmartPtr {
    RefObj* p;
};

static int g_released = 0;

void __fastcall release_fn(RefObj* o);

void __fastcall CEngineInternalPrimitiveMeshBase_GetMeshEffect(SmartPtr* self, void* edx, SmartPtr* other)
{
    RefObj* cur = self->p;
    if (cur != other->p) {
        if (cur != 0) {
            if (--cur->refcount == 0) {
                cur->vtbl[1](cur);
            }
            self->p = 0;
        }
        RefObj* src = other->p;
        self->p = src;
        if (src != 0) {
            ++src->refcount;
        }
    }
}

void __fastcall release_fn(RefObj* o) { g_released++; }

int main() {
    typedef void (__fastcall *RFn)(RefObj*);
    RFn vt[2];
    vt[0] = 0;
    vt[1] = &release_fn;

    RefObj a; a.vtbl = vt; a.refcount = 1;
    RefObj b; b.vtbl = vt; b.refcount = 5;

    SmartPtr self; self.p = &a;
    SmartPtr other; other.p = &b;

    // a had refcount 1 -> decremented to 0 -> released; self now points to b, b refcount 6
    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&self, 0, &other);

    if (self.p != &b) { std::printf("FAIL ptr\n"); return 1; }
    if (b.refcount != 6) { std::printf("FAIL refb %ld\n", b.refcount); return 1; }
    if (g_released != 1) { std::printf("FAIL rel %d\n", g_released); return 1; }
    if (a.refcount != 0) { std::printf("FAIL refa %ld\n", a.refcount); return 1; }

    // same-pointer assignment: no change
    SmartPtr s2; s2.p = &b;
    SmartPtr o2; o2.p = &b;
    long before = b.refcount;
    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&s2, 0, &o2);
    if (b.refcount != before) { std::printf("FAIL same %ld\n", b.refcount); return 1; }

    std::printf("CEngineInternalPrimitiveMeshBase_0042a665_TEST PASS\n");
    return 0;
}