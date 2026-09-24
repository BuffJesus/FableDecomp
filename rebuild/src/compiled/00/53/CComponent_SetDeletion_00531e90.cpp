#include "fable_ui_deletion.h"

void __fastcall FableUiSetDeletion(FableUiComponentDrawView* component, void*, FableUiDeletion value)
{
    component->Deletion.Method = value.Method;
    FableUiAssignDeletionParents(&component->Deletion.AssociatedParents, 0, &value.AssociatedParents);
    FableUiDestroyDeletionParents(&value.AssociatedParents, 0);
}
