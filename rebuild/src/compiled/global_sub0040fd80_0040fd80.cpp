#include "rebuild_abi.h"

// 0x0040fd80 __fastcall — refcounted smart-pointer assignment.
// this (ecx) holds a pointer to a refcounted object at +0x0;
// the object's refcount is at +0x4; vtable slot +0x4 releases it.

struct RefObj {
    void** vtbl;     // +0x00
    int    refCount; // +0x04
};

struct RefPtr {
    RefObj* obj;     // +0x00
};

typedef void (__fastcall *DtorFn)(RefObj*);

void __fastcall sub_0040fd80(RefPtr* thisptr, void* edx, RefObj** src)
{
    RefObj* incoming = *src;          // mov eax,[edi]
    RefObj* current  = thisptr->obj;  // mov ecx,[esi]
    if (current != incoming) {        // cmp ecx,eax / je end
        if (current) {                // test ecx,ecx / je skip
            if (--current->refCount == 0) {          // dec [ecx+4] / jne
                DtorFn fn = ((DtorFn*)current->vtbl)[1]; // mov eax,[ecx]; call [eax+4]
                fn(current);
            }
            thisptr->obj = 0;         // mov dword [esi],0
        }
        incoming = *src;              // mov eax,[edi]
        thisptr->obj = incoming;      // mov [esi],eax
        if (incoming) {               // test eax,eax / je end
            ++incoming->refCount;     // inc [eax+4]
        }
    }
}