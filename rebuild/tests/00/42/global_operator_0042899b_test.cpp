#include "rebuild_abi.h"
#include <cstdio>

struct _ListNode { _ListNode* _Next; _ListNode* _Prev; };
struct _ConstIter { _ListNode* _Ptr; };

extern "C" _ConstIter* __fastcall op_dec_post_0042899b(
    _ConstIter* thisptr, void*, _ConstIter* retslot, int);

int main()
{
    _ListNode a, b, c;
    a._Next = &b; a._Prev = &c;
    b._Next = &c; b._Prev = &a;
    c._Next = &a; c._Prev = &b;

    _ConstIter it; it._Ptr = &b;   // currently at node b
    _ConstIter ret;
    _ConstIter* r = op_dec_post_0042899b(&it, 0, &ret, 0);

    // postfix: returns OLD (b), iterator advances to b->_Prev (a)
    int ok = (r == &ret) && (ret._Ptr == &b) && (it._Ptr == &a);
    if (ok) printf("OK_0042899b_DECPOST\n");
    else    printf("FAIL ret=%p old=%p cur=%p\n",
                   (void*)r, (void*)ret._Ptr, (void*)it._Ptr);
    return 0;
}