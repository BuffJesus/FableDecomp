#include "fable_ui_child_update.h"
#include "fable_ui_counted_children.h"
#include "fable_ui_deletion.h"

void __fastcall FableUiBaseComponentUpdate(FableUiComponentDrawView* component, void*, float delta)
{
    component->Time += delta;
    component->Vtable->UpdateZoom(component, 0, delta);
    component->Vtable->UpdatePosition(component, 0, delta);
    component->Vtable->UpdateColour(component, 0, delta);
    FableUiUpdatePositionChildren(component);
    for (unsigned i = 0; i < component->Children.Size(); ++i)
    {
        if (FableUiPrepareAndUpdateLiveChild(component, i, delta))
            FableUiProcessLiveChildDeletion(component, i);
    }
    for (unsigned i = 0; i < component->ChildrenToDelete.Size(); ++i)
    {
        if (FableUiPrepareAndUpdateRetiringChild(component, i, delta))
            FableUiFinishRetiringChild(component, i);
    }
}
