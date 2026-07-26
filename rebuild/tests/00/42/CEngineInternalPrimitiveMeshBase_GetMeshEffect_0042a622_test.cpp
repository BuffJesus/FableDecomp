#include <cstdio>

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

static int g_destroyed = 0;
static void __fastcall destroy(RCObj* o) { g_destroyed++; }

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

int main()
{
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&destroy;

    RCObj a; a.vtbl = vt; a.refs = 1;
    RCObj b; b.vtbl = vt; b.refs = 5;

    CEngineInternalPrimitiveMeshBase self; self.ptr = &a;
    Holder other; other.ptr = &b;

    self.GetMeshEffect(&other);
    if (self.ptr != &b) { std::printf("FAIL ptr\n"); return 1; }
    if (b.refs != 6)    { std::printf("FAIL brefs %ld\n", b.refs); return 1; }
    if (g_destroyed != 1){ std::printf("FAIL destroy %d\n", g_destroyed); return 1; }

    CEngineInternalPrimitiveMeshBase s2; s2.ptr = &b;
    Holder o2; o2.ptr = &b;
    long before = b.refs;
    s2.GetMeshEffect(&o2);
    if (b.refs != before) { std::printf("FAIL noop %ld\n", b.refs); return 1; }

    std::printf("CEngineInternalPrimitiveMeshBase_0042a622_TEST PASS\n");
    return 0;
}