#include "fable_ui_deletion.h"

FableUiDeletion* __fastcall FableUiGetDeletion(FableUiComponentDrawView* component, void*)
{
    return &component->Deletion;
}
