// CCountedPointer<T>::operator=(const CCountedPointer<T>&) @ 0x009A9BF0
//
// Genuine C++ reconstruction (no inline assembly). Releases this object's
// current reference (decrementing the shared refcount block and, on the
// count reaching zero, invoking the masked destroy-thunk followed by
// ::operator delete on the control block), then copies rhs's object
// pointer + refcount block pointer into this and increments the new
// block's refcount (unless the rhs block is null). If this and rhs
// already share the same block, the whole release/copy/increment
// sequence is skipped (self/same-block assignment).
//
// Block layout matches the already-landed sibling _Dest_val<CCountedPointer<T>>
// at 0x009A9BB0: RefBlock{ int count; void(__fastcall*dtor)(void*); void* ptr; }.

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

extern "C" CCountedPtr* __fastcall CCountedPtr_assign(CCountedPtr* self, int /*edx*/, CCountedPtr* rhs)
{
    void* rhsObj = rhs->obj;
    RefBlock* rhsBlk = rhs->blk;
    RefBlock* curBlk = self->blk;

    if (curBlk != rhsBlk)
    {
        if (curBlk != 0)
        {
            --curBlk->count;
            curBlk = self->blk;
            if (curBlk->count == 0)
            {
                curBlk->dtor(curBlk->ptr);
                curBlk = self->blk;
                ::operator delete(curBlk);
            }
        }

        self->obj = rhsObj;
        self->blk = rhsBlk;

        if (rhsBlk != 0)
        {
            ++rhsBlk->count;
        }
    }

    return self;
}