#pragma optimize("s", on)
struct RefObj {
    void (__fastcall **vtbl)(RefObj*);
    long refcount;
};

struct SmartPtr {
    RefObj* p;
};

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