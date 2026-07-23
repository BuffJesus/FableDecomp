#include <cstdio>

struct IRefCounted {
    void** vtbl;
    int    refcnt;
};

struct MeshEffectHolder {
    IRefCounted* ptr;
};

struct CEngineInternalPrimitiveMeshBase {
    IRefCounted* effect;
};

typedef void (__fastcall *ReleaseFn)(IRefCounted*);

static int g_destroyed = 0;

void __fastcall CEngineInternalPrimitiveMeshBase_GetMeshEffect(
    CEngineInternalPrimitiveMeshBase* self,
    void* edx_unused,
    MeshEffectHolder* other)
{
    IRefCounted* cur = self->effect;
    if (cur != other->ptr) {
        if (cur != 0) {
            if (--cur->refcnt == 0) {
                (*(ReleaseFn*)((char*)cur->vtbl + 4))(cur);
            }
            self->effect = 0;
        }
        IRefCounted* nw = other->ptr;
        self->effect = nw;
        if (nw) {
            ++nw->refcnt;
        }
    }
}

static void __fastcall destroy_stub(IRefCounted* p) {
    g_destroyed = 1;
    (void)p;
}

int main() {
    void* vt[2];
    vt[1] = (void*)(ReleaseFn)&destroy_stub;

    IRefCounted a; a.vtbl = vt; a.refcnt = 2;
    IRefCounted b; b.vtbl = vt; b.refcnt = 1;

    CEngineInternalPrimitiveMeshBase self;
    self.effect = &a;
    MeshEffectHolder h; h.ptr = &b;

    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&self, 0, &h);
    if (self.effect != &b) { std::printf("FAIL ptr\n"); return 1; }
    if (a.refcnt != 1) { std::printf("FAIL a refcnt\n"); return 1; }
    if (b.refcnt != 2) { std::printf("FAIL b refcnt\n"); return 1; }
    if (g_destroyed != 0) { std::printf("FAIL destroyed early\n"); return 1; }

    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&self, 0, &h);
    if (b.refcnt != 2) { std::printf("FAIL noop\n"); return 1; }

    a.refcnt = 1;
    MeshEffectHolder h2; h2.ptr = &a;
    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&self, 0, &h2);
    if (self.effect != &a) { std::printf("FAIL ptr2\n"); return 1; }

    IRefCounted* nullp = 0;
    MeshEffectHolder hn; hn.ptr = nullp;
    a.refcnt = 1;
    CEngineInternalPrimitiveMeshBase_GetMeshEffect(&self, 0, &hn);
    if (g_destroyed != 1) { std::printf("FAIL not destroyed\n"); return 1; }
    if (self.effect != 0) { std::printf("FAIL not null\n"); return 1; }

    std::printf("CEngineInternalPrimitiveMeshBase_0042a5f3_TEST PASS\n");
    return 0;
}