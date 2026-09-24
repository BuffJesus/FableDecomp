#include "fable_ui_state_progress.h"

bool __fastcall FableUiChildrenChanged(FableUiComponentDrawView* component, void*)
{
    for (unsigned i = 0; i < component->Children.Size(); ++i)
    {
        FableUiComponentDrawView* child = component->Children.Begin[i].Data;
        if (!child->Vtable->HasCompletedStateChange(child, 0)) return false;
    }
    return true;
}
