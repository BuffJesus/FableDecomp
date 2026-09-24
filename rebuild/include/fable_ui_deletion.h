#pragma once
#include "fable_ui_component_draw.h"

struct FableUiDeletionParentNode
{
    FableUiDeletionParentNode* Next;
    FableUiDeletionParentNode* Previous;
    FableUiComponentDrawView* Parent;
};
FABLE_STATIC_ASSERT(sizeof(FableUiDeletionParentNode) == 12);

// Retail malloc/free boundaries (00BFEA0E / 00BFEA14).
void* __cdecl FableUiListAllocate(unsigned bytes);
void __cdecl FableUiListFree(void* allocation);

inline FableUiDeletionParentNode* FableUiCreateDeletionList()
{
    FableUiDeletionParentNode* head = static_cast<FableUiDeletionParentNode*>(FableUiListAllocate(sizeof(FableUiDeletionParentNode)));
    head->Next = head->Previous = head;
    return head;
}

inline void FableUiAppendDeletionParent(FableUiDeletionParentNode* head, FableUiComponentDrawView* parent)
{
    FableUiDeletionParentNode* node = static_cast<FableUiDeletionParentNode*>(FableUiListAllocate(sizeof(FableUiDeletionParentNode)));
    node->Next = head;
    node->Previous = head->Previous;
    head->Previous->Next = node;
    head->Previous = node;
    node->Parent = parent;
}

FableUiDeletionParentNode** __fastcall FableUiAssignDeletionParents(FableUiDeletionParentNode** destination, void*, FableUiDeletionParentNode* const* source);
void __fastcall FableUiDestroyDeletionParents(FableUiDeletionParentNode** list, void*);
FableUiDeletion* __fastcall FableUiCopyDeletion(FableUiDeletion* destination, void*, const FableUiDeletion* source);
FableUiDeletion* __fastcall FableUiGetDeletion(FableUiComponentDrawView*, void*);
// Takes ownership of the by-value list argument, matching the retail callee.
void __fastcall FableUiSetDeletion(FableUiComponentDrawView*, void*, FableUiDeletion value);

inline void FableUiProcessLiveChildDeletion(FableUiComponentDrawView* parent, unsigned index)
{
    FableUiComponentDrawView* child = parent->Children.Begin[index].Data;
    const FableUiDeletion* source = child->Vtable->GetDeletion(child, 0);
    FableUiDeletion snapshot;
    FableUiCopyDeletion(&snapshot, 0, source);
    if (snapshot.Method == 1)
    {
        FableUiDeletion empty = {0, FableUiCreateDeletionList()};
        child = parent->Children.Begin[index].Data;
        FableUiDeletion argument;
        FableUiCopyDeletion(&argument, 0, &empty);
        child->Vtable->SetDeletion(child, 0, argument);
        parent->Vtable->RemoveChildAt(parent, 0, index);
        FableUiDestroyDeletionParents(&empty.AssociatedParents, 0);
    }
    else if (snapshot.Method == 2)
        parent->Vtable->RemoveChildAt(parent, 0, index);
    else if (snapshot.Method == 3)
    {
        FableUiDeletionParentNode* node = snapshot.AssociatedParents->Next;
        while (node != snapshot.AssociatedParents && node->Parent != parent) node = node->Next;
        if (node != snapshot.AssociatedParents)
        {
            node->Previous->Next = node->Next;
            node->Next->Previous = node->Previous;
            FableUiListFree(node);
            if (snapshot.AssociatedParents->Next == snapshot.AssociatedParents)
            {
                FableUiDeletion empty = {0, FableUiCreateDeletionList()};
                child = parent->Children.Begin[index].Data;
                FableUiDeletion argument;
                FableUiCopyDeletion(&argument, 0, &empty);
                child->Vtable->SetDeletion(child, 0, argument);
                FableUiDestroyDeletionParents(&empty.AssociatedParents, 0);
            }
            parent->Vtable->RemoveChildAt(parent, 0, index);
        }
    }
    FableUiDestroyDeletionParents(&snapshot.AssociatedParents, 0);
}
