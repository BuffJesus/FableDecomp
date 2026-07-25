#include "rebuild_abi.h"

// Retail 0x004367c0 : CGuiControlTreePane list-clear (std::list::clear-style)
// __fastcall(this) -> void
//
// Layout inferred from disasm:
//   this+0x00 = _Myhead (list_node*)   [ecx] -> circular sentinel node
//   node+0x00 = _Next (list_node*)
//   node+0x04 = _Prev (list_node*)
//
// Semantics: walk from _Myhead->_Next around the circular list, freeing each
// real node until we return to the sentinel, then relink sentinel to itself
// (empty list): _Myhead->_Next = _Myhead ; _Myhead->_Prev = _Myhead.

struct list_node
{
    list_node* _Next; // +0x00
    list_node* _Prev; // +0x04
};

struct list_hdr
{
    list_node* _Myhead; // +0x00
};

// Node deallocator (relocation-masked call target 0x7c8254).
extern "C" void FABLE_CDECL list_node_free(list_node* _Node);

void FABLE_FASTCALL sub_4367c0(list_hdr* thisptr)
{
    list_node* head = thisptr->_Myhead;      // eax = [edi]
    list_node* node = head->_Next;           // esi = [eax]

    while (node != head)                      // cmp esi,eax ; je/jne
    {
        list_node* cur = node;               // eax = esi
        node = node->_Next;                  // esi = [esi]
        list_node_free(cur);                 // push eax ; call 0x7c8254
        head = thisptr->_Myhead;             // eax = [edi] (reload)
    }

    head = thisptr->_Myhead;                 // eax = [edi]
    head->_Next = head;                      // mov [eax],eax
    head = thisptr->_Myhead;                 // edi = [edi]
    head->_Prev = head;                      // mov [edi+4],edi
}