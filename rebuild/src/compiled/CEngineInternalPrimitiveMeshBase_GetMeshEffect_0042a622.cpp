struct RCObj {
    void* vtbl;
    long refs;
};

struct Holder {
    RCObj* ptr;
};

struct CEngineInternalPrimitiveMeshBase {
    RCObj* ptr;
    void GetMeshEffect(Holder* other);
};

#pragma optimize("s", on)
void CEngineInternalPrimitiveMeshBase::GetMeshEffect(Holder* other)
{
    RCObj* cur = this->ptr;
    if (cur == other->ptr)
        return;
    if (cur) {
        if (--cur->refs == 0) {
            typedef void (__fastcall *Fn)(RCObj*);
            (*(Fn*)((char*)cur->vtbl + 4))(cur);
        }
        this->ptr = 0;
    }
    RCObj* nw = other->ptr;
    this->ptr = nw;
    if (nw)
        ++nw->refs;
}
#pragma optimize("", on)