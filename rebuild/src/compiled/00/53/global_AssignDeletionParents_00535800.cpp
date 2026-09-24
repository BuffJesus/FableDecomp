#include "fable_ui_deletion.h"

FableUiDeletionParentNode** __fastcall FableUiAssignDeletionParents(FableUiDeletionParentNode** destination, void*, FableUiDeletionParentNode* const* source)
{
    if (destination == source) return destination;
    FableUiDeletionParentNode* output = (*destination)->Next;
    FableUiDeletionParentNode* input = (*source)->Next;
    while (output != *destination && input != *source)
    {
        output->Parent = input->Parent;
        output = output->Next;
        input = input->Next;
    }
    while (output != *destination)
    {
        FableUiDeletionParentNode* next = output->Next;
        output->Previous->Next = next;
        next->Previous = output->Previous;
        FableUiListFree(output);
        output = next;
    }
    while (input != *source)
    {
        FableUiAppendDeletionParent(*destination, input->Parent);
        input = input->Next;
    }
    return destination;
}
