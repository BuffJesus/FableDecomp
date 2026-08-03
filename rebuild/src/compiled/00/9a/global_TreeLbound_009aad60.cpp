// _Tree::_Lbound  (retail 0x009aad60)
// Standard MSVC7.1 std::_Tree<Traits> lower-bound walk, specialised for a
// tree keyed by a CCharString-ish key pointer, compared via a helper at 0x00429950.

// Key-compare helper (CCharString::operator< or equivalent): a real C++ member
// function taking 'this' in ecx and the single argument on the stack (retail
// call site pushes exactly one dword and does not touch edx).
struct CCharString
{
    bool Less(CCharString* _Right);
};

// Node layout matches std::_Tree_nod<Traits>::_Node as used across this binary:
//   +0x00 unused-here (reserved / not read by this walk)
//   +0x04 _Parent
//   +0x08 _Left
//   +0x0c _Right
//   +0x10 _Myval (here: a CCharString* key, stored inline at the node)
struct _Node
{
    void* _Reserved0;
    _Node* _Parent;
    _Node* _Left;
    _Node* _Right;
    CCharString* _Myval;
};

// The tree object itself: only field used here is _Myhead at +0x00.
struct _Tree
{
    _Node* _Myhead;
};

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