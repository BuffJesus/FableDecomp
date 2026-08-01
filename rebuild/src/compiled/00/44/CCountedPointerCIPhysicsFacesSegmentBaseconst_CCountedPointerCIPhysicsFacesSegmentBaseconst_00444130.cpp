#include <new>

// CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset(
//     CIPhysicsFacesSegmentBase const*) @ 0x00444130.
//
// Genuine C++ reconstruction of the intrusive-refcount smart-pointer Reset
// (real statements only, no inline assembly). Layout matches the sibling ctor
// at 0x00403D20:
//   CCountedPtr { void* obj; RefBlock* blk; }                (this in ecx)
//   RefBlock    { int owners; DestroyFn destroy; void* obj; } (12 bytes)
//
// Retail idiom:
//   1. If the current control block is non-null, drop one owner; when the
//      owner count reaches zero the object destroy-thunk is invoked
//      (__fastcall through blk->destroy with blk->obj in ecx) and the control
//      block itself is freed with ::operator delete.
//   2. The control pointer is cleared and the new object stored. A null new
//      object stops there; otherwise a fresh 12-byte control block is
//      allocated with ::operator new, seeded owners=1 / masked destroy-thunk
//      (0x00443540) / object, and installed. A failed allocation leaves blk
//      cleared. ret 4.
//
// The owner drop is spelled as a decrement followed by a fresh member read
// (`--old->owners; if (self->blk->owners == 0)`) so cl reloads this->blk after
// the dec, matching `dec [eax]; mov eax,[esi+4]; cmp [eax],0`. The two zero
// stores (blk cleared before the store of the new object) match the retail
// `mov [esi+4],0; mov [esi],eax` order, with the entry test of the new object
// hoisted above them. The alloc branch is a nested if/else joining on a shared
// `self->blk = r` store, mirroring the ctor's xor/immediate split; no
// size-optimize pragma so the clear stays the 7-byte immediate store.

typedef void (__fastcall *DestroyFn)(void*);

struct RefBlock { int owners; DestroyFn destroy; void* obj; };
struct CCountedPtr { void* obj; RefBlock* blk; };

extern "C" void __fastcall
CCountedPtr_Reset(CCountedPtr* self, int edxdummy, void* arg)
{
    RefBlock* old = self->blk;
    if (old) {
        --old->owners;
        if (self->blk->owners == 0) {
            RefBlock* b = self->blk;
            b->destroy(b->obj);
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
            b->destroy = (DestroyFn)0x443540;
            b->obj = o;
            r = b;
        } else {
            r = 0;
        }
        self->blk = r;
    }
}