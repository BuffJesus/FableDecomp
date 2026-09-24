#include "fable_ui_state_progress.h"

bool __fastcall FableUiChangedStateLastUpdate(FableUiStateProgressView* component, void*)
{
    if (component->PreviousUpdateChanged !=
        component->Vtable->HasCompletedStateChange(component, 0))
    {
        // Retail calls the virtual query again; it can change between calls.
        return component->Vtable->HasCompletedStateChange(component, 0);
    }
    return false;
}
