#include <cstdio>

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

static void FakeDtor(void* p) { (void)p; }

int main()
{
    // Build a minimal circular list: head->_Next = head, head->_Prev = head (empty list).
    _Node head;
    head._Next = &head;
    head._Prev = &head;
    head._Myval.obj = 0;
    head._Myval.blk = 0;

    _List_val lst;
    lst._Myhead = &head;

    // Case 1: value with a non-null refcount block -> refcount incremented, node linked at tail.
    {
        int dummyObj = 0;
        RefBlock blk;
        blk.count = 1;
        blk.dtor = (void(__fastcall*)(void*))FakeDtor;
        blk.ptr = &dummyObj;

        CCountedPtr val;
        val.obj = &dummyObj;
        val.blk = &blk;

        _Node* n = Buynode_CSpawnedFuncBase(&lst, 0, &val);

        if (n == 0) { std::printf("FAIL case1 null node\n"); return 1; }
        if (n->_Myval.obj != &dummyObj) { std::printf("FAIL case1 obj\n"); return 1; }
        if (n->_Myval.blk != &blk) { std::printf("FAIL case1 blk\n"); return 1; }
        if (blk.count != 2) { std::printf("FAIL case1 count=%d\n", blk.count); return 1; }

        // Linked between head and head (was empty): head->_Next == n, head->_Prev == n, n->_Next==head, n->_Prev==head.
        if (head._Next != n || head._Prev != n) { std::printf("FAIL case1 head links\n"); return 1; }
        if (n->_Next != &head || n->_Prev != &head) { std::printf("FAIL case1 node links\n"); return 1; }

        ::operator delete(n);
        head._Next = &head;
        head._Prev = &head;
    }

    // Case 2: value with a null refcount block -> no increment, node still linked at tail.
    {
        CCountedPtr val;
        val.obj = 0;
        val.blk = 0;

        _Node* n = Buynode_CSpawnedFuncBase(&lst, 0, &val);

        if (n == 0) { std::printf("FAIL case2 null node\n"); return 1; }
        if (n->_Myval.obj != 0 || n->_Myval.blk != 0) { std::printf("FAIL case2 fields\n"); return 1; }
        if (head._Next != n || head._Prev != n) { std::printf("FAIL case2 head links\n"); return 1; }

        ::operator delete(n);
        head._Next = &head;
        head._Prev = &head;
    }

    // Case 3: append second node to a non-empty list -> new node becomes new tail.
    {
        CCountedPtr val1; val1.obj = (void*)1; val1.blk = 0;
        _Node* n1 = Buynode_CSpawnedFuncBase(&lst, 0, &val1);

        CCountedPtr val2; val2.obj = (void*)2; val2.blk = 0;
        _Node* n2 = Buynode_CSpawnedFuncBase(&lst, 0, &val2);

        if (head._Next != n1) { std::printf("FAIL case3 head next\n"); return 1; }
        if (head._Prev != n2) { std::printf("FAIL case3 head prev\n"); return 1; }
        if (n1->_Next != n2) { std::printf("FAIL case3 n1 next\n"); return 1; }
        if (n2->_Prev != n1) { std::printf("FAIL case3 n2 prev\n"); return 1; }
        if (n1->_Prev != &head) { std::printf("FAIL case3 n1 prev\n"); return 1; }
        if (n2->_Next != &head) { std::printf("FAIL case3 n2 next\n"); return 1; }

        ::operator delete(n1);
        ::operator delete(n2);
        head._Next = &head;
        head._Prev = &head;
    }

    std::printf("PASS_009aaaa0\n");
    return 0;
}
