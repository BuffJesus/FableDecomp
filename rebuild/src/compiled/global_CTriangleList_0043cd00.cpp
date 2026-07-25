#include "rebuild_abi.h"

// _Dest_val<std::allocator<CIVCountedPointer<CTriangleList> >, CIVCountedPointer<CTriangleList> >
// Destroys a CIVCountedPointer: if the held object exists, decrement its refcount
// at +0x4; when it reaches zero, invoke vtable slot 1 ([vtbl+4]); then null the slot.

struct CountedObj {
    void** vtbl;      // +0x00
    int    refcount;  // +0x04
};

struct CIVCountedPointer {
    CountedObj* obj;  // +0x00
};

typedef void (__fastcall *dtor_fn)(CountedObj*);

void __fastcall Dest_val_0043cd00(CIVCountedPointer* thisptr)
{
    CountedObj* p = thisptr->obj;
    if (p != 0) {
        if (--p->refcount == 0) {
            (*(dtor_fn*)((char*)p->vtbl + 4))(p);
        }
        thisptr->obj = 0;
    }
}