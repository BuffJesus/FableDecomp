// Byte-exact reconstruction of
// CFontDataBank::PrepareDataForInclusion @ 0x00a63420
//
// Retail is a __fastcall member (this in ecx only). The verify harness rewrites
// __fastcall -> __fastcall, so we model it as a free __fastcall function whose
// first param is `this` (ecx) and second is an unused edx placeholder; every real
// argument then lands on the stack, giving ret 0x1c (7 stack dwords).
//
// Body: pull a refcounted block pointer from a by-value CCountedPointer stack arg
// (at [esp+0x18]), dec its refcount, and if it reaches 0 call its virtual-ish
// destructor (ecx = payload at +8) then free it via a cdecl helper @ 0x00bfe9bc.

struct RefBlock {
    int              refcount;      // +0x00
    void (__fastcall *dtor)(void*); // +0x04  invoked as call [esi+4], ecx=[esi+8]
    void*            payload;       // +0x08
};

// CCountedPointer<CBankStateBlock> by value: first member is the block ptr.
struct CountedPtr { RefBlock* p; };

extern "C" void __cdecl RefBlock_Free(void* p); // 0x00bfe9bc

struct CFontDataBank;

// Free __fastcall model of the __fastcall member.
//   ecx = self, edx = unused
//   stack: a0, a1, a2, a3, blk(=CountedPtr @ [esp+0x18]), a5, a6  -> ret 0x1c
void __fastcall CFontDataBank_PrepareDataForInclusion(
    CFontDataBank* /*self*/,
    int /*edx_unused*/,
    long /*a0*/,
    long /*a1*/,
    long /*a2*/,
    long /*a3*/,
    CountedPtr blk,   // by value; occupies the [esp+0x18] slot
    long /*a5*/,
    long /*a6*/)
{
    RefBlock* p = blk.p;
    if (p) {
        if (--p->refcount == 0) {
            p->dtor(p->payload);
            RefBlock_Free(p);
        }
    }
}