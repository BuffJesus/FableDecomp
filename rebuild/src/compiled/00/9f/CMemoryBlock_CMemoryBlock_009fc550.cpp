// Byte-exact reconstruction of ~CMemoryBlock @ 0x009fc550 (VC7.1 /O2)
//
// Modeled as a __fastcall free function taking `self` in ecx (the this-call
// destructor).  The two raw vtable-pointer stores reproduce the compiler's
// most-derived / base vtable init that a polymorphic destructor emits.
//
// The conditional cleanup is a call on the +8 member object: retail loads the
// member pointer into ecx (`mov ecx,[esi+8]`), tests it, and if non-null calls
// a method with `self` pushed as the sole stack argument and NO caller-side
// `add esp,4` -> the member's method is a this-call (this = member in ecx,
// self pushed, callee cleans its own stack).  That is `member8->Free(self)`.
//
// Named without '~' so the harness selects this single function (the mangled
// ??1/??_G destructor symbols never contain the '~CMemoryBlock' leaf, so the
// harness would otherwise pick the larger scalar-deleting destructor).

struct CMemoryBlock;

// The +8 member is a pointer to an object whose (this-call) Free(self) performs
// the block release at 0x9fc020.
struct CMemoryBlockOwner {
    void Free(CMemoryBlock* self);
};

struct CMemoryBlock {
    void*             vptr;      // +0
    int               pad4;      // +4
    CMemoryBlockOwner* member8;  // +8
};

// vtable pointers (relocated -> masked by the harness).
static void* const kVtblDerived = (void*)0x012354a4;
static void* const kVtblBase     = (void*)0x012354d0;

void __fastcall CMemoryBlock_dtor_009fc550(CMemoryBlock* self)
{
    self->vptr = kVtblDerived;
    if (self->member8 != 0) {
        self->member8->Free(self);
    }
    self->vptr = kVtblBase;
}