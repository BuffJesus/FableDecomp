#include <new>

// CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset(
//     CIPhysicsFacesSegmentBase const*) @ 0x00444050.
//
// Genuine C++ reconstruction of the intrusive-refcount smart-pointer Reset
// (real statements only -- no inline assembly). Reset first RELEASES the
// currently-held control block (the classic --owners; if zero destroy+delete
// idiom, identical byte-for-byte to the shared destructor path), then INSTALLS
// the new object exactly like the family ctor: clear this->blk, store the new
// object into this->obj, and when the new object is non-null allocate a 12-byte
// control block with ::operator new, seed it (owners=1, masked destroy-thunk
// 0x00443500, object), and install it. A failed allocation clears this->blk.
//
// Field / ABI notes:
//  * blk is this+0x04, obj is this+0x00 (matches [esi+4]/[esi]).
//  * The control block is {int owners; destroyfn destroy; void* object;} i.e.
//    owners at +0, destroy at +4, object at +8 (matches [eax]/[eax+4]/[eax+8]).
//  * destroy is a __fastcall(void*) pointer: reference->destroy(reference->
//    object) emits mov ecx,[eax+8]; call [eax+4].
//  * ::operator new / ::operator delete are the masked __cdecl allocator pair
//    (push size / push ptr; call; add esp,4).
//  * The new-object argument is the single stack parameter (this is __fastcall,
//    this in ecx); the function pops it (ret 4).
//  * The release block spells self->blk each time (no local cache) so the
//    compiler reloads [esi+4] after the dec and again before delete, matching
//    retail; the two clear-of-blk stores are kept split (nested if with the
//    alloc branch sharing self->blk=r, and the null-object path a separate
//    literal store) so they are not tail-merged.

typedef void (__fastcall *DestroyFn)(void* object);

struct RefBlock { int owners; DestroyFn destroy; void* object; };
struct CCountedPtr { void* obj; RefBlock* blk; };

extern "C" void __fastcall
CCountedPtr_Reset(CCountedPtr* self, int edxdummy, void* arg)
{
    if (self->blk) {
        --self->blk->owners;
        if (self->blk->owners == 0) {
            self->blk->destroy(self->blk->object);
            ::operator delete(self->blk);
        }
    }

    void* a = arg;
    self->blk = 0;
    self->obj = a;
    if (a) {
        RefBlock* b = (RefBlock*)::operator new(0xc);
        RefBlock* r;
        if (b) {
            void* o = self->obj;
            b->owners = 1;
            b->destroy = (DestroyFn)0x443500;
            b->object = o;
            r = b;
        } else {
            r = 0;
        }
        self->blk = r;
    }
}