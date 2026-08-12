// CTexture::GetRefCount  retail 0x009f9d60
// member 0 = pointer to a COM-like interface object whose [0] is a vtable of
// stdcall function pointers taking the object pointer explicitly.
// slot1 ([vtbl+4]) called for side effect; slot2 ([vtbl+8]) returns the count.

struct IRefObj;
struct IRefVtbl {
    long (__stdcall *slot0)(IRefObj*);
    long (__stdcall *slot1)(IRefObj*);
    long (__stdcall *slot2)(IRefObj*);
};
struct IRefObj {
    IRefVtbl* vtbl;
};

struct CTexture {
    IRefObj* obj;
    long GetRefCount();
};

long CTexture::GetRefCount()
{
    IRefObj* p = this->obj;
    if (p != 0)
    {
        p->vtbl->slot1(p);
        p = this->obj;
        return p->vtbl->slot2(p);
    }
    return 0;
}