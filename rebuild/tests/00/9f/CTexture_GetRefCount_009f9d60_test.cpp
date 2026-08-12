#include <cstdio>

struct IRefObj;
struct IRefVtbl {
    long (__stdcall *slot0)(IRefObj*);
    long (__stdcall *slot1)(IRefObj*);
    long (__stdcall *slot2)(IRefObj*);
};
struct IRefObj {
    IRefVtbl* vtbl;
    long count;
};

struct CTexture {
    IRefObj* obj;
    long GetRefCount();
};

long CTexture::GetRefCount()
{
    IRefObj* p = this->obj;
    if (p == 0)
        return 0;
    p->vtbl->slot1(p);
    p = this->obj;
    return p->vtbl->slot2(p);
}

static int g_slot1_calls = 0;
long __stdcall my_slot0(IRefObj*) { return 0; }
long __stdcall my_slot1(IRefObj* o) { g_slot1_calls++; return 999; }
long __stdcall my_slot2(IRefObj* o) { return o->count; }

int main()
{
    IRefVtbl vt = { my_slot0, my_slot1, my_slot2 };
    IRefObj o; o.vtbl = &vt; o.count = 42;

    CTexture a; a.obj = &o;
    CTexture b; b.obj = 0;

    long ra = a.GetRefCount();
    long rb = b.GetRefCount();

    if (ra == 42 && rb == 0 && g_slot1_calls == 1)
        printf("GETREFCOUNT_OK\n");
    else
        printf("FAIL ra=%ld rb=%ld c=%d\n", ra, rb, g_slot1_calls);
    return (ra == 42 && rb == 0 && g_slot1_calls == 1) ? 0 : 1;
}