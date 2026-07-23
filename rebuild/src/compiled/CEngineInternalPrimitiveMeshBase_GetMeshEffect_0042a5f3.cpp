#pragma optimize("gsy",on)

struct IRefCounted {
    void** vtbl;   // +0x0
    int    refcnt; // +0x4
};

struct MeshEffectHolder {
    IRefCounted* ptr; // +0x0
};

struct CEngineInternalPrimitiveMeshBase {
    IRefCounted* effect; // +0x0
};

typedef void (__fastcall *ReleaseFn)(IRefCounted*);

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
        if (nw != 0) {
            ++nw->refcnt;
        }
    }
}