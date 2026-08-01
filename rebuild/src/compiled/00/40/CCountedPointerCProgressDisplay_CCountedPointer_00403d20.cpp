#include <new>

// CCountedPointer<CProgressDisplay>::CCountedPointer(CProgressDisplay*) @ 0x00403D20.
//
// Genuine C++ reconstruction of the intrusive-refcount smart-pointer ctor
// (real statements only, no inline assembly). The single object argument is
// stored into this->obj;
// when non-null a 12-byte control block is allocated with ::operator new. On
// success it is seeded with owners=1, a masked destroy-thunk address
// (0x00403600), and the object pointer, then installed as this->blk. A null
// object OR a failed allocation leaves this->blk cleared. The ctor returns
// this (eax) and pops its single stack argument (ret 4).
//
// The two zero-stores differ in the retail bytes and must not be tail-merged:
// the alloc branch shares "self->blk = r" (xor eax,eax + mov [esi+4],eax when
// r==0), while the null branch is a distinct literal store (mov [esi+4],0).
// Expressing them as the nested if/else + separate else preserves that split;
// no size-optimize pragma (retail uses the 7-byte immediate store, not and,0).

struct RefBlock { int count; int vtbl; void* ptr; };
struct CCountedPtr { void* obj; RefBlock* blk; };

extern "C" CCountedPtr* __fastcall
CCountedPtr_ctor(CCountedPtr* self, int edxdummy, void* arg)
{
    void* a = arg;
    self->obj = a;
    if (a) {
        RefBlock* b = (RefBlock*)::operator new(0xc);
        RefBlock* r;
        if (b) {
            void* o = self->obj;
            b->count = 1;
            b->vtbl = 0x403600;
            b->ptr = o;
            r = b;
        } else {
            r = 0;
        }
        self->blk = r;
    } else {
        self->blk = 0;
    }
    return self;
}