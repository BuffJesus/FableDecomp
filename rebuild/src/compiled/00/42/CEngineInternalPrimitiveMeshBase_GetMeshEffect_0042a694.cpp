#pragma optimize("s",on)
struct RCObj {
    void (__fastcall **vtbl)(RCObj*);
    int refcount;
};

struct CEngineInternalPrimitiveMeshBase {
    RCObj* ptr;
    void GetMeshEffect(RCObj** src);
};

void CEngineInternalPrimitiveMeshBase::GetMeshEffect(RCObj** src)
{
    RCObj* cur = ptr;
    if (cur != *src) {
        if (cur) {
            if (--cur->refcount == 0) {
                cur->vtbl[1](cur);
            }
            ptr = 0;
        }
        RCObj* n = *src;
        ptr = n;
        if (n) {
            ++n->refcount;
        }
    }
}