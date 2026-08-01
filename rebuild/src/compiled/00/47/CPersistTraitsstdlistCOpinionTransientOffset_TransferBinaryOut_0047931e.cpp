#pragma optimize("s",on)
#include "rebuild_abi.h"

// CPersistTraits<std::list<COpinionTransientOffset> >::TransferBinaryOut @ 0x0047931e
//
// Retail is __fastcall / void / 3 params, but `this` (ecx) is never used by the
// body, so it is modelled as __fastcall with an explicit (unused) edx slot; the
// two real args arrive on the stack: [esp+0x10]=stream, [esp+0x14]=list.
//
// Semantics (genuine C++, no asm):
//   count = list->Size();          // __fastcall helper, this=list, no stack arg
//   stream->PutCount(count);       // __fastcall helper, this=stream, count pushed
//   for (Node* p = list->head->next; p != list->head; p = p->next)
//       stream->TransferBytes(&p->value, 0x18);   // virtual slot 4 (vtbl+0x10)
//
// Node layout: next @ +0x0, value @ +0x8 (0x18 bytes copied per node).  The list
// object holds a pointer to its sentinel node at +0x0 (`head`).  The circular
// walk starts at head->next and stops when it cycles back to the sentinel.
//
// Both non-virtual helpers (Size, PutCount) are reloc-masked rel32 calls, so
// Size is declared extern and PutCount is an out-of-line member; the per-element
// sink is a genuine virtual dispatch through vtable slot 4.

struct OpinionNode
{
    OpinionNode* next;          // +0x00
    OpinionNode* prev;          // +0x04
    unsigned char value[0x18];  // +0x08
};

struct OpinionList
{
    OpinionNode* head;          // +0x00  pointer to sentinel node
};

// Stream: vptr @ +0x0.  Non-virtual PutCount (direct rel32 call) plus a virtual
// TransferBytes at vtable index 4 (byte offset 0x10).
struct OpinionStream
{
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void TransferBytes(const void* src, int len);  // vtbl+0x10
    void PutCount(fable_u32 count);                        // non-virtual, __fastcall
};

// Reloc-masked helper.
extern "C" fable_u32 FABLE_FASTCALL OpinionList_Size_0047931e(OpinionList* list);

void __fastcall
CPersistTraits_OpinionList_TransferBinaryOut_0047931e(
    void* /*self ecx*/, void* /*edx*/, OpinionStream* stream, OpinionList* list)
{
    fable_u32 count = OpinionList_Size_0047931e(list);
    stream->PutCount(count);
    for (OpinionNode* p = list->head->next; p != list->head; p = p->next)
    {
        stream->TransferBytes(&p->value, 0x18);
    }
}