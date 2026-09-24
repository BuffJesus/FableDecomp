#include "fable_ui_deletion.h"

void __fastcall FableUiDestroyDeletionParents(FableUiDeletionParentNode** list, void*)
{
    FableUiDeletionParentNode* head = *list;
    FableUiDeletionParentNode* node = head->Next;
    while (node != head)
    {
        FableUiDeletionParentNode* next = node->Next;
        FableUiListFree(node);
        node = next;
    }
    head->Next = head->Previous = head;
    if (head) FableUiListFree(head);
}
