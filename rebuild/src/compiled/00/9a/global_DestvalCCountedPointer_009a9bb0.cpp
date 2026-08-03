// _Dest_val<std::allocator<CCountedPointer<T>_>,CCountedPointer<T>_> @ 0x009A9BB0
//
// Genuine C++ reconstruction (no inline assembly). STL _Dest_val destroys the
// CCountedPointer<T> in place: decrement the shared refcount block, invoke the
// masked destroy-thunk (thiscall, ecx = the wrapped object pointer) when the
// count hits zero, then ::operator delete the control block and null both of
// this CCountedPointer's fields (obj, blk).
//
// Block layout matches the already-landed CCountedPointer<CProgressDisplay>
// ctor at 0x00403D20: RefBlock{ int count; void(__fastcall*dtor)(void*); void*
// ptr; }. The dtor is called with ptr in ecx (thiscall), matching
// "mov ecx,[eax+8]; call [eax+4]" in the retail bytes.

struct RefBlock
{
    int count;
    void (__fastcall *dtor)(void*);
    void* ptr;
};

struct CCountedPtr
{
    void* obj;
    RefBlock* blk;
};

extern "C" void __fastcall Dest_val_CCountedPointer(CCountedPtr* self, int /*edx*/)
{
    RefBlock* b = self->blk;
    if (b != 0) {
        --b->count;
        b = self->blk;
        if (b->count == 0) {
            b->dtor(b->ptr);
            b = self->blk;
            ::operator delete(b);
        }
    }
    self->obj = 0;
    self->blk = 0;
}