#include <cstdio>

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


static int g_released = 0;
static void __fastcall release(RCObj* o) { g_released++; }
static void (__fastcall *g_vt[2])(RCObj*) = { 0, release };

int main() {
    RCObj a; a.vtbl = g_vt; a.refcount = 2;
    RCObj b; b.vtbl = g_vt; b.refcount = 1;

    CEngineInternalPrimitiveMeshBase h; h.ptr = &a;
    RCObj* srcp = &b;

    h.GetMeshEffect(&srcp);
    if (h.ptr != &b) { std::printf("FAIL ptr\n"); return 1; }
    if (a.refcount != 1) { std::printf("FAIL a rc %d\n", a.refcount); return 1; }
    if (b.refcount != 2) { std::printf("FAIL b rc %d\n", b.refcount); return 1; }
    if (g_released != 0) { std::printf("FAIL released early\n"); return 1; }

    RCObj* srcp2 = &b;
    h.GetMeshEffect(&srcp2);
    if (b.refcount != 2) { std::printf("FAIL b rc2 %d\n", b.refcount); return 1; }

    a.refcount = 1;
    CEngineInternalPrimitiveMeshBase h2; h2.ptr = &a;
    RCObj* nul = 0;
    h2.GetMeshEffect(&nul);
    if (g_released != 1) { std::printf("FAIL not released %d\n", g_released); return 1; }
    if (h2.ptr != 0) { std::printf("FAIL h2 ptr\n"); return 1; }

    std::printf("CEngineInternalPrimitiveMeshBase_0042a694_TEST PASS\n");
    return 0;
}