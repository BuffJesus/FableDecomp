// _Buynode<CCountedPointer<CSpawnedFuncBase> > for _List_val<CCountedPointer<CSpawnedFuncBase>,...>
// Allocates one list node, copies the CCountedPointer value into it (bumping the refcount
// block if non-null), and links the new node in at the tail (just before the sentinel head),
// matching retail's push_back-style _Buynode.

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

struct _Node
{
    _Node* _Next;
    _Node* _Prev;
    CCountedPtr _Myval;
};

struct _List_val
{
    _Node* _Myhead;
};

_Node* __fastcall Buynode_CSpawnedFuncBase(_List_val* self, int /*edx*/, const CCountedPtr* val)
{
    _Node* head = self->_Myhead;

    _Node* node = (_Node*)::operator new(0x10);
    CCountedPtr* dst = (CCountedPtr*)((char*)node + 8);
    if (dst != 0)
    {
        dst->obj = val->obj;
        dst->blk = val->blk;
        if (dst->blk != 0)
        {
            ++dst->blk->count;
        }
    }

    _Node* tail = head->_Prev;
    node->_Next = head;
    node->_Prev = tail;
    tail->_Next = node;
    head->_Prev = node;

    return node;
}
