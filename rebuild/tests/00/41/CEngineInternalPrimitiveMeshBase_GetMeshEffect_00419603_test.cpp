#include <stdio.h>

struct CRefCounted {
    void** vtable;   // +0x00
    int    refCount; // +0x04
};

struct CRefHandle {
    CRefCounted* ptr; // +0x00
    void SetHeld(CRefHandle* src);
};

void CRefHandle::SetHeld(CRefHandle* src)
{
    CRefCounted* cur = this->ptr;
    if (cur != src->ptr) {
        if (cur) {
            if (--cur->refCount == 0)
                ((void(__fastcall*)(CRefCounted*))cur->vtable[1])(cur);
            this->ptr = 0;
        }
        CRefCounted* nw = src->ptr;
        this->ptr = nw;
        if (nw)
            ++nw->refCount;
    }
}

// ---- test harness ----
static int g_destroyed = 0;
static CRefCounted* g_lastDestroyed = 0;

void __fastcall obj_release(CRefCounted* self)
{
    g_destroyed++;
    g_lastDestroyed = self;
}

int main()
{
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&obj_release;

    // Case A: assign new object into empty handle -> refcount incremented
    CRefCounted objA; objA.vtable = vt; objA.refCount = 1;
    CRefHandle dst; dst.ptr = 0;
    CRefHandle srcA; srcA.ptr = &objA;
    dst.SetHeld(&srcA);
    if (dst.ptr != &objA)   { printf("FAIL A ptr\n"); return 1; }
    if (objA.refCount != 2) { printf("FAIL A ref=%d\n", objA.refCount); return 1; }
    if (g_destroyed != 0)   { printf("FAIL A destroyed\n"); return 1; }

    // Case B: same pointer -> early return, no change
    CRefHandle srcSame; srcSame.ptr = &objA;
    dst.SetHeld(&srcSame);
    if (dst.ptr != &objA)   { printf("FAIL B ptr\n"); return 1; }
    if (objA.refCount != 2) { printf("FAIL B ref\n"); return 1; }

    // Case C: replace held object; old refcount 2 -> 1, NOT destroyed
    CRefCounted objB; objB.vtable = vt; objB.refCount = 5;
    CRefHandle srcB; srcB.ptr = &objB;
    dst.SetHeld(&srcB);
    if (dst.ptr != &objB)   { printf("FAIL C ptr\n"); return 1; }
    if (objA.refCount != 1) { printf("FAIL C oldref=%d\n", objA.refCount); return 1; }
    if (objB.refCount != 6) { printf("FAIL C newref=%d\n", objB.refCount); return 1; }
    if (g_destroyed != 0)   { printf("FAIL C destroyed early\n"); return 1; }

    // Case D: replace held object whose refcount hits 0 -> destructor slot called
    CRefCounted objC; objC.vtable = vt; objC.refCount = 1;
    CRefHandle h2; h2.ptr = &objC;
    CRefCounted objD; objD.vtable = vt; objD.refCount = 3;
    CRefHandle srcD; srcD.ptr = &objD;
    h2.SetHeld(&srcD);
    if (h2.ptr != &objD)         { printf("FAIL D ptr\n"); return 1; }
    if (g_destroyed != 1)        { printf("FAIL D destroyed=%d\n", g_destroyed); return 1; }
    if (g_lastDestroyed != &objC){ printf("FAIL D which\n"); return 1; }
    if (objD.refCount != 4)      { printf("FAIL D newref=%d\n", objD.refCount); return 1; }

    // Case E: assign null-holding source into a held handle -> release + set null
    CRefCounted objE; objE.vtable = vt; objE.refCount = 1;
    CRefHandle srcNull; srcNull.ptr = 0;
    dst.ptr = &objE;
    g_destroyed = 0;
    dst.SetHeld(&srcNull);
    if (dst.ptr != 0)     { printf("FAIL E ptr\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL E destroyed=%d\n", g_destroyed); return 1; }

    printf("CENGINEINTERNALPRIMITIVEMESHBASE_00419603_TEST PASS\n");
    return 0;
}