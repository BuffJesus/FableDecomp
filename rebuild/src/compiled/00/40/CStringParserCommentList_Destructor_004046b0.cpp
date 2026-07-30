#include "rebuild_abi.h"

struct FableStringParserCommentNode_004046B0
{
    FableStringParserCommentNode_004046B0* next;
    FableStringParserCommentNode_004046B0* previous;
};

struct FableStringParserCommentList_004046B0
{
    FableStringParserCommentNode_004046B0* head;
};

extern "C" void FABLE_CDECL
FableStringParserCommentNodeDelete_004046B0(void* node);

// std::list<CStringParserCommentBoundaries>::~list() @ 0x004046B0.
//
// This COMDAT was mislabeled as CGuiControlTreePane::SortTreeRecursively by
// signature propagation.  GFMain passes CStringParser::Comments at +0x14.
// The retail body releases every value node, restores the circular sentinel,
// and finally releases the sentinel itself.
extern "C" __declspec(naked) void FABLE_FASTCALL
FableStringParserCommentListDestroy_004046B0(
    FableStringParserCommentList_004046B0*)
{
    __asm
    {
        push esi
        push edi
        mov edi, ecx
        mov eax, dword ptr [edi]
        mov esi, dword ptr [eax]
        cmp esi, eax
        je release_sentinel
        _emit 08Dh
        _emit 064h
        _emit 024h
        _emit 000h

    release_node:
        mov eax, esi
        mov esi, dword ptr [esi]
        push eax
        call FableStringParserCommentNodeDelete_004046B0
        mov eax, dword ptr [edi]
        add esp, 4
        cmp esi, eax
        jne release_node

    release_sentinel:
        mov eax, dword ptr [edi]
        mov dword ptr [eax], eax
        mov eax, dword ptr [edi]
        mov dword ptr [eax + 4], eax
        mov edi, dword ptr [edi]
        test edi, edi
        je finished
        push edi
        call FableStringParserCommentNodeDelete_004046B0
        add esp, 4

    finished:
        pop edi
        pop esi
        ret
    }
}
