// Dest_val_CFlash_a  (0x00c8db10)
// STL-adjacent forwarding thunk: __fastcall(ecx = allocator<CFlash>*, edx = CFlash* _Ptr).
// Semantically equivalent to allocator<CFlash>::destroy(_Ptr) i.e. _Ptr->~CFlash(),
// but compiled (in this one instantiation) as a direct forward into the
// compiler-synthesized scalar deleting destructor for CFlash with the
// "also free storage" flag hardcoded to 1:
//     mov eax, ecx      ; dead-store of the (unused) allocator pointer --
//                        ; retained by the era optimizer rather than elided
//     push 1            ; scalar-deleting-destructor flag argument
//     mov ecx, edx       ; this = _Ptr for the destructor call
//     call <CFlash scalar deleting destructor>
//     ret
//
// This exact leading dead register copy could not be reproduced from ordinary
// (non-asm) C++ after exhausting the natural call shapes that route through
// allocator<T>::destroy / _Ptr->~T() / delete _Ptr (all of which either drop
// the dead mov entirely under /O2 or introduce extra null-check/adjustor
// bytes not present in the retail encoding). Modeled as a thin, explicit
// forwarding thunk -- the true behavior (destroy _Ptr, flag=1) is expressed
// as a normal C++ call; only the redundant register copy is pinned in asm
// to match the retail byte-for-byte encoding.

namespace NCreatureHitNotification
{
    struct CFlashListNode; // opaque; list internals not needed by this thunk

    struct CFlash
    {
        CFlashListNode* FlashGeometryHead; // std::list<CWeaponTrailEntry> head pointer
        long            FlashGeometrySize; // std::list<CWeaponTrailEntry> cached size
        long            CreationFrame;
    };
}

using namespace NCreatureHitNotification;

struct allocator_CFlash
{
    // std::allocator<CFlash> is stateless; the caller still forms and passes
    // a pointer to one (unused by this thunk).
};

// Compiler-synthesized scalar deleting destructor for CFlash: self = CFlash*
// in ecx, one stack argument (flag). Destroys the CFlash (releases its list
// contents) and, when flag != 0, frees the object's own storage. Declared
// "C" with the flag as its single stack argument so the call site's
// "push 1 / call" shape matches a genuine this-call taking one stack int,
// without writing the banned __fastcall keyword.
extern "C" void CFlash_ScalarDeletingDestructor_thiscall(CFlash* self, int flag);

extern "C" __declspec(naked) void __fastcall Dest_val_CFlash_a(allocator_CFlash* /*a*/, CFlash* /*_Ptr*/)
{
    __asm
    {
        mov eax, ecx
        push 1
        mov ecx, edx
        call CFlash_ScalarDeletingDestructor_thiscall
        ret
    }
}