#include "rebuild_abi.h"

// 0x0042899b
// std::list<CVsyncCallbackTimer::CPendingQuery>::_List_const_iterator::operator--(int)  [postfix]
// Retail bytes: 8b118b420489018b4424048910c20800
//   mov edx,[ecx]      ; edx = this->_Ptr  (current node)
//   mov eax,[edx+4]    ; eax = node->_Prev
//   mov [ecx],eax      ; this->_Ptr = _Prev
//   mov eax,[esp+4]    ; eax = return-slot ptr (hidden 1st stack arg)
//   mov [eax],edx      ; *retslot = old node ptr  (return old iterator)
//   ret 8              ; retslot(4) + dummy int(4)

struct _ListNode {
    _ListNode* _Next;   // +0
    _ListNode* _Prev;   // +4
};

struct _ConstIter {
    _ListNode* _Ptr;    // +0
};

// __fastcall member modeled as free __fastcall(thisptr, ...).
// Signature carries: hidden return-value pointer + the postfix dummy int
// => ret 8 (two 4-byte stack args after ecx).
extern "C" _ConstIter* __fastcall op_dec_post_0042899b(
    _ConstIter* thisptr, void* /*edx unused*/,
    _ConstIter* retslot, int /*dummy*/)
{
    _ListNode* old = thisptr->_Ptr;   // mov edx,[ecx]
    thisptr->_Ptr = old->_Prev;       // mov eax,[edx+4] ; mov [ecx],eax
    retslot->_Ptr = old;              // mov [eax],edx  (eax=[esp+4])
    return retslot;
}