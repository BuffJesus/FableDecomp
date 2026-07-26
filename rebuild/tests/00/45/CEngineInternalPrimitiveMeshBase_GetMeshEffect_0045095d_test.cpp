#include <cstdio>

struct RefObj {
    void** vtable;   // +0x0
    int refCount;    // +0x4
};

struct Holder {
    RefObj* ptr;     // +0x0
};

struct MeshEffectPtr {
    RefObj* ptr;     // +0x0
};

typedef void (__fastcall *ReleaseFn)(RefObj*);

void __fastcall CEngineInternalPrimitiveMeshBase_GetMeshEffect(MeshEffectPtr* self, int edx, Holder* src) {
    RefObj* cur = self->ptr;
    if (cur != src->ptr) {
        if (cur) {
            if (--cur->refCount == 0) {
                (*(ReleaseFn*)((char*)cur->vtable + 4))(cur);
            }
            self->ptr = 0;
        }
        RefObj* nv = src->ptr;
        self->ptr = nv;
        if (nv) {
            ++nv->refCount;
        }
    }
}

static int g_released = 0;
static void __fastcall dummy0(RefObj*) {}
static void __fastcall relFn(RefObj*) { g_released = 1; }

int main() {
    // vtable: slot0, slot1(=+4) points to relFn
    void* vt[2];
    vt[0] = (void*)&dummy0;
    vt[1] = (void*)&relFn;

    // Case 1: assign new object -> addref
    RefObj a; a.vtable = vt; a.refCount = 1;
    MeshEffectPtr self; self.ptr = 0;
    Holder h; h.ptr = &a;
    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&self, 0, &h);
    if (self.ptr != &a || a.refCount != 2) { std::printf("FAIL case1 rc=%d\n", a.refCount); return 1; }

    // Case 2: same pointer -> no change
    Holder h2; h2.ptr = &a;
    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&self, 0, &h2);
    if (self.ptr != &a || a.refCount != 2) { std::printf("FAIL case2 rc=%d\n", a.refCount); return 1; }

    // Case 3: replace old with different, old refcount hits 0 -> release
    RefObj b; b.vtable = vt; b.refCount = 5;
    // make 'a' about to drop to 0
    a.refCount = 1;
    Holder h3; h3.ptr = &b;
    g_released = 0;
    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&self, 0, &h3);
    if (self.ptr != &b || g_released != 1 || b.refCount != 6) {
        std::printf("FAIL case3 rel=%d bc=%d\n", g_released, b.refCount); return 1;
    }

    // Case 4: replace with null
    Holder h4; h4.ptr = 0;
    b.refCount = 3;
    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&self, 0, &h4);
    if (self.ptr != 0 || b.refCount != 2) { std::printf("FAIL case4 bc=%d\n", b.refCount); return 1; }

    std::printf("CEngineInternalPrimitiveMeshBase_0045095d_TEST PASS\n");
    return 0;
}