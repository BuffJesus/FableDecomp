#include <cstdio>

struct CCounted {
    void* vtbl;
    int   refcount;
};

struct CCountedPtr {
    CCounted* p;
};

void __fastcall _Dest_val(CCountedPtr* self, void* alloc)
{
    CCounted* obj = self->p;
    if (obj) {
        if (--obj->refcount == 0) {
            (*(void (__fastcall**)(CCounted*))((char*)obj->vtbl + 4))(obj);
        }
        self->p = 0;
    }
}

static int g_destroyed = 0;
static void __fastcall fake_dtor(CCounted* o) { g_destroyed++; }

int main()
{
    // vtable: slot0 unused, slot1 = dtor
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&fake_dtor;

    // Case 1: refcount 2 -> becomes 1, no dtor, pointer nulled
    CCounted obj1; obj1.vtbl = vt; obj1.refcount = 2;
    CCountedPtr ptr1; ptr1.p = &obj1;
    _Dest_val(&ptr1, 0);
    if (obj1.refcount != 1) { std::printf("FAIL refcount case1\n"); return 1; }
    if (ptr1.p != 0) { std::printf("FAIL null case1\n"); return 1; }
    if (g_destroyed != 0) { std::printf("FAIL dtor case1\n"); return 1; }

    // Case 2: refcount 1 -> becomes 0, dtor called, pointer nulled
    CCounted obj2; obj2.vtbl = vt; obj2.refcount = 1;
    CCountedPtr ptr2; ptr2.p = &obj2;
    _Dest_val(&ptr2, 0);
    if (obj2.refcount != 0) { std::printf("FAIL refcount case2\n"); return 1; }
    if (ptr2.p != 0) { std::printf("FAIL null case2\n"); return 1; }
    if (g_destroyed != 1) { std::printf("FAIL dtor case2\n"); return 1; }

    // Case 3: null pointer -> nothing happens
    CCountedPtr ptr3; ptr3.p = 0;
    _Dest_val(&ptr3, 0);
    if (ptr3.p != 0) { std::printf("FAIL null case3\n"); return 1; }
    if (g_destroyed != 1) { std::printf("FAIL dtor case3\n"); return 1; }

    std::printf("_Dest_val_0040f270_TEST PASS\n");
    return 0;
}