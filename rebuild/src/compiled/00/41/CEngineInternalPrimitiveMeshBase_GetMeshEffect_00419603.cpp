#pragma optimize("s",on)
// CEngineInternalPrimitiveMeshBase::GetMeshEffect @ 0x00419603 (47 bytes)
// Despite the "GetMeshEffect" manifest name, the retail body is a refcounted
// smart-pointer ASSIGNMENT (TCRefPtr::operator= shape): it stores src's held
// object into 'this', releasing the previously-held object (virtual slot 1 =
// vtable[1] when its refcount reaches 0) and incrementing the new one's refcount.
//
// Held object layout:  vtable @ +0x00,  refCount @ +0x04.
// Handle layout:       ptr    @ +0x00.
//
// __fastcall member => 'this' in ecx, one stack arg (src), 'ret 4'.
// VC7.1 forbids __fastcall on a free function (C4234), so it is modeled as a
// struct member function, which the compiler naturally lowers to thiscall.
// The virtual release call is typed __fastcall (single ptr arg in ecx) because
// VC7.1 also rejects __fastcall on a function-pointer type; for a this-only
// virtual this is byte-identical (ecx=this).
//
// The manifest lists the return as 'bool' but the retail bytes NEVER set eax,
// so the value is dead: modeled as 'void' (a bool return would emit an extra
// 'mov al,1', breaking parity).

struct CRefCounted {
    void** vtable;   // +0x00
    int    refCount; // +0x04
};

struct CRefHandle {
    CRefCounted* ptr; // +0x00
    void SetHeld(CRefHandle* src);
};

void CRefHandle::SetHeld(CRefHandle* src)
{
    CRefCounted* cur = this->ptr;
    if (cur != src->ptr) {
        if (cur) {
            if (--cur->refCount == 0)
                ((void(__fastcall*)(CRefCounted*))cur->vtable[1])(cur);
            this->ptr = 0;                 // emits 'and dword ptr [esi],0' under /O1
        }
        CRefCounted* nw = src->ptr;
        this->ptr = nw;
        if (nw)
            ++nw->refCount;
    }
}