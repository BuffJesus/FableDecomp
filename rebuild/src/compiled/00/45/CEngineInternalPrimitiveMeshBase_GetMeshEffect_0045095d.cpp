#pragma optimize("s",on)
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

// __fastcall member modeled as __fastcall: this=ecx, dummy edx, real arg on stack.
void __fastcall CEngineInternalPrimitiveMeshBase_GetMeshEffect(MeshEffectPtr* self, int /*edx*/, Holder* src) {
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