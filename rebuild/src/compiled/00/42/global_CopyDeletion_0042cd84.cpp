#include "fable_ui_deletion.h"

FableUiDeletion* __fastcall FableUiCopyDeletion(FableUiDeletion* destination, void*, const FableUiDeletion* source)
{
    destination->Method = source->Method;
    destination->AssociatedParents = FableUiCreateDeletionList();
    FableUiDeletionParentNode* head = source->AssociatedParents;
    for (FableUiDeletionParentNode* node = head->Next; node != head; node = node->Next)
        FableUiAppendDeletionParent(destination->AssociatedParents, node->Parent);
    return destination;
}
