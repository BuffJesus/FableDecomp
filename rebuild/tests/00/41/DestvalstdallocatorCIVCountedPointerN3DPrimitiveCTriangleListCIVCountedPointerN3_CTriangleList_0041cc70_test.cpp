#include <cstdio>

struct CountedObj {
    void** vtbl;
    long   refcount;
};
struct CIVCountedPointer {
    CountedObj* p;
};

static int g_released = 0;

void __fastcall Dest_val(CIVCountedPointer* self, void* al)
{
    CountedObj* p = self->p;
    if (p != 0) {
        if (--p->refcount == 0) {
            (*(void (__fastcall**)(CountedObj*))(*(void***)p + 1))(p);
        }
        self->p = 0;
    }
}

void __fastcall released_fn(CountedObj* o) { (void)o; g_released = 1; }

int main()
{
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&released_fn;

    CountedObj obj;
    obj.vtbl = vt;
    obj.refcount = 2;

    CIVCountedPointer ptr;
    ptr.p = &obj;

    /* refcount 2 -> 1, not released, pointer nulled */
    Dest_val(&ptr, 0);
    if (ptr.p != 0) { std::printf("FAIL not nulled\n"); return 1; }
    if (obj.refcount != 1) { std::printf("FAIL refcount\n"); return 1; }
    if (g_released != 0) { std::printf("FAIL early release\n"); return 1; }

    /* now hit 0 -> release */
    ptr.p = &obj;
    Dest_val(&ptr, 0);
    if (obj.refcount != 0) { std::printf("FAIL refcount2\n"); return 1; }
    if (g_released != 1) { std::printf("FAIL no release\n"); return 1; }
    if (ptr.p != 0) { std::printf("FAIL not nulled2\n"); return 1; }

    /* null pointer: no-op */
    CIVCountedPointer nptr;
    nptr.p = 0;
    Dest_val(&nptr, 0);
    if (nptr.p != 0) { std::printf("FAIL null\n"); return 1; }

    std::printf("_Dest_val_0041cc70_TEST PASS\n");
    return 0;
}