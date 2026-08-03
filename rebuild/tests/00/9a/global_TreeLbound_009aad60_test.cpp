#include <cstdio>

struct CCharString
{
    bool Less(CCharString* _Right);
};

struct _Node
{
    void* _Reserved0;
    _Node* _Parent;
    _Node* _Left;
    _Node* _Right;
    CCharString* _Myval;
};

struct _Tree
{
    _Node* _Myhead;
};

// simple integer-backed "CCharString" stand-in: identity is the pointer, ordering by
// the int payload, matching how the retail helper would compare two key blobs.
struct FakeStr { int v; };

bool CCharString::Less(CCharString* _Right)
{
    FakeStr* a = (FakeStr*)this;
    FakeStr* b = (FakeStr*)_Right;
    return a->v < b->v;
}

// std::_Tree<...>::_Lbound(const CCharString* const& _Keyval) const
// thiscall(ecx=this, [esp+4]=&_Keyval) -> eax = node
extern "C" _Node* __fastcall Tree_Lbound(_Tree* self, void* /*edx*/, CCharString* const& _Keyval)
{
    _Node* head = self->_Myhead;
    _Node* wherenode = head->_Parent;
    if (wherenode != 0)
    {
        CCharString* keyval = _Keyval;
        do
        {
            CCharString* nodeval = wherenode->_Myval;
            bool less;
            if (keyval == nodeval)
                less = false;
            else if (nodeval == 0)
                less = true;
            else if (keyval == 0)
                less = false;
            else
                less = nodeval->Less(keyval);

            if (!less)
            {
                head = wherenode;
                wherenode = wherenode->_Left;
            }
            else
            {
                wherenode = wherenode->_Right;
            }
        } while (wherenode != 0);
    }
    return head;
}

int main()
{
    // Build a small BST by hand (values 10,20,30,40,50) shaped as a balanced tree:
    //          30
    //        /    \
    //      20      40
    //     /          \
    //   10            50
    FakeStr k10; k10.v = 10;
    FakeStr k20; k20.v = 20;
    FakeStr k30; k30.v = 30;
    FakeStr k40; k40.v = 40;
    FakeStr k50; k50.v = 50;

    _Node n10, n20, n30, n40, n50, head;
    n10._Myval = (CCharString*)&k10;
    n20._Myval = (CCharString*)&k20;
    n30._Myval = (CCharString*)&k30;
    n40._Myval = (CCharString*)&k40;
    n50._Myval = (CCharString*)&k50;
    head._Myval = 0;

    n30._Left = &n20; n30._Right = &n40;
    n20._Left = &n10; n20._Right = 0;
    n40._Left = 0;    n40._Right = &n50;
    n10._Left = 0;    n10._Right = 0;
    n50._Left = 0;    n50._Right = 0;

    head._Parent = &n30;
    n30._Parent = &head;
    n20._Parent = &n30;
    n40._Parent = &n30;
    n10._Parent = &n20;
    n50._Parent = &n40;

    _Tree t;
    t._Myhead = &head;

    bool ok = true;

    // lower_bound(25) should land on node 30 (first key >= 25)
    FakeStr q25; q25.v = 25;
    CCharString* pq25 = (CCharString*)&q25;
    _Node* r1 = Tree_Lbound(&t, 0, pq25);
    if (r1 != &n30) { ok = false; std::printf("FAIL r1 got %p want %p\n", (void*)r1, (void*)&n30); }

    // lower_bound(30) exact match should land on node 30 itself
    FakeStr q30; q30.v = 30;
    CCharString* pq30 = (CCharString*)&q30;
    _Node* r2 = Tree_Lbound(&t, 0, pq30);
    if (r2 != &n30) { ok = false; std::printf("FAIL r2 got %p want %p\n", (void*)r2, (void*)&n30); }

    // lower_bound(1) should land before everything -> head (no key >= 1... wait 10>=1)
    FakeStr q1; q1.v = 1;
    CCharString* pq1 = (CCharString*)&q1;
    _Node* r3 = Tree_Lbound(&t, 0, pq1);
    if (r3 != &n10) { ok = false; std::printf("FAIL r3 got %p want %p\n", (void*)r3, (void*)&n10); }

    // lower_bound(51) should land past everything -> head sentinel
    FakeStr q51; q51.v = 51;
    CCharString* pq51 = (CCharString*)&q51;
    _Node* r4 = Tree_Lbound(&t, 0, pq51);
    if (r4 != &head) { ok = false; std::printf("FAIL r4 got %p want %p\n", (void*)r4, (void*)&head); }

    // empty-tree case: Myhead->_Parent == 0 => returns Myhead unchanged
    _Node emptyhead;
    emptyhead._Myval = 0;
    emptyhead._Parent = 0;
    _Tree et; et._Myhead = &emptyhead;
    FakeStr q0; q0.v = 0;
    CCharString* pq0 = (CCharString*)&q0;
    _Node* r5 = Tree_Lbound(&et, 0, pq0);
    if (r5 != &emptyhead) { ok = false; std::printf("FAIL r5 got %p want %p\n", (void*)r5, (void*)&emptyhead); }

    if (ok) std::printf("TREE_LBOUND_OK\n");
    return ok ? 0 : 1;
}