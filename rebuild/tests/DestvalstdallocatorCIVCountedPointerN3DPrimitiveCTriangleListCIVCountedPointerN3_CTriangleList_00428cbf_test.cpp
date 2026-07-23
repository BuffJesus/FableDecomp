#include <cstdio>

struct CRefCounted {
    void** vtbl;   // +0x0
    long   refs;   // +0x4
};

struct CIVCountedPointer {
    CRefCounted* p;  // +0x0
};

typedef void (__fastcall *DtorFn)(CRefCounted*);

static int g_dtor_called = 0;
static CRefCounted* g_dtor_arg = 0;

void __fastcall myDtor(CRefCounted* self)
{
    g_dtor_called++;
    g_dtor_arg = self;
}

void __fastcall CIVCountedPointer_Dest_val(CIVCountedPointer* self)
{
    CRefCounted* obj = self->p;
    if (obj) {
        if (--obj->refs == 0) {
            (*(DtorFn*)((char*)obj->vtbl + 4))(obj);
        }
        self->p = 0;
    }
}

int main()
{
    // vtable: slot0 unused, slot1 = dtor
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&myDtor;

    // Case 1: refs=2 -> dec to 1, no dtor, pointer NOT nulled? Actually pointer IS nulled.
    CRefCounted obj1;
    obj1.vtbl = vt;
    obj1.refs = 2;
    CIVCountedPointer cp1;
    cp1.p = &obj1;
    CIVCountedPointer_Dest_val(&cp1);
    if (obj1.refs != 1) { std::printf("FAIL refs1 %ld\n", obj1.refs); return 1; }
    if (g_dtor_called != 0) { std::printf("FAIL dtor called early\n"); return 1; }
    if (cp1.p != 0) { std::printf("FAIL ptr not nulled c1\n"); return 1; }

    // Case 2: refs=1 -> dec to 0, dtor called
    CRefCounted obj2;
    obj2.vtbl = vt;
    obj2.refs = 1;
    CIVCountedPointer cp2;
    cp2.p = &obj2;
    CIVCountedPointer_Dest_val(&cp2);
    if (obj2.refs != 0) { std::printf("FAIL refs2 %ld\n", obj2.refs); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor not called\n"); return 1; }
    if (g_dtor_arg != &obj2) { std::printf("FAIL dtor arg\n"); return 1; }
    if (cp2.p != 0) { std::printf("FAIL ptr not nulled c2\n"); return 1; }

    // Case 3: null pointer -> no-op
    CIVCountedPointer cp3;
    cp3.p = 0;
    CIVCountedPointer_Dest_val(&cp3);
    if (g_dtor_called != 1) { std::printf("FAIL dtor called on null\n"); return 1; }

    std::printf("_Dest_val_00428cbf_TEST PASS\n");
    return 0;
}