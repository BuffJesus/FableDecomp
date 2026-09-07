// CTexture_Methods::GetRefCount  retail 0x009f9d60
// member 0 = pointer to a COM-like interface object whose [0] is a vtable of
// stdcall function pointers taking the object pointer explicitly.
// slot1 ([vtbl+4]) called for side effect; slot2 ([vtbl+8]) returns the count.

#include "engine/CTexture.h"  // retyped onto the PDB layout; byte parity re-verified
struct IDirect3DTexture9;
struct IRefVtbl {
    long (__stdcall *slot0)(IDirect3DTexture9*);
    long (__stdcall *slot1)(IDirect3DTexture9*);
    long (__stdcall *slot2)(IDirect3DTexture9*);
};
struct IDirect3DTexture9 {
    IRefVtbl* vtbl;
};

struct CTexture_Methods : CTexture {
    long GetRefCount();
};

long CTexture_Methods::GetRefCount()
{
    IDirect3DTexture9* p = this->PD3DTexture;
    if (p != 0)
    {
        p->vtbl->slot1(p);
        p = this->PD3DTexture;
        return p->vtbl->slot2(p);
    }
    return 0;
}