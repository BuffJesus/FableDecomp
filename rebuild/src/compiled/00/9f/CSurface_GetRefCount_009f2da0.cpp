// Byte-exact reconstruction of CSurface::GetRefCount @ 0x009f2da0
// The member at [esi+4] is a COM-style interface pointer whose "vtable"
// entries are __stdcall functions that take the interface as a stack arg.

struct IRefObj;

typedef long (__stdcall *PFN)(IRefObj*);

struct IRefVtbl {
    PFN slot0;   // +0
    PFN slot1;   // +4  (called, result discarded)
    PFN slot2;   // +8  (called, result returned)
};

struct IRefObj {
    IRefVtbl* vtbl;  // +0
};

struct CSurface {
    void* pad0;      // +0
    IRefObj* obj;    // +4
};

long __fastcall GetRefCount(CSurface* self)
{
    IRefObj* p = self->obj;
    if (p) {
        p->vtbl->slot1(p);
        IRefObj* q = self->obj;
        return q->vtbl->slot2(q);
    }
    return 0;
}