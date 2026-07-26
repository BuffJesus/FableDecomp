#include "rebuild_abi.h"

// 0043cfd0 _Dest_val<std::allocator<CIVCountedPointer<CTriangleList> >, CIVCountedPointer<CTriangleList> >
// Releases a CIVCountedPointer: loads the shared counted object; if non-null,
// decrements its refcount at +0x4; when that reaches zero, invokes vtable slot 1
// (virtual dtor) via [*obj + 0x4]; then null-clears the wrapper's stored pointer.

struct CountedObj {
    void** vtbl;   // +0x00
    int    refcnt; // +0x04
};

struct CountedPtr {
    CountedObj* p; // +0x00
};

void __fastcall Dest_val_0043cfd0(CountedPtr* thisptr)
{
    CountedObj* obj = thisptr->p;
    if (obj != 0) {
        if (--obj->refcnt == 0) {
            typedef void (__fastcall *VFn)(CountedObj*);
            ((VFn)(obj->vtbl[1]))(obj);
        }
        thisptr->p = 0;
    }
}