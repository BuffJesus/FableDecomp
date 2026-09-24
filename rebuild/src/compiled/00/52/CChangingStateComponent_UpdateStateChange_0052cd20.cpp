#include "fable_ui_state_progress.h"
#include <stdlib.h>

void __fastcall FableUiUpdateStateChange(FableUiStateProgressView* component, void*)
{
    enum { ChangeChildren = 1, ChangeSelf = 2 };
    if (component->StatesToDo->Next != component->StatesToDo &&
        (component->StatesDone & component->StatesBeingDone) == component->StatesBeingDone)
    {
        FableUiStateTaskNode* task = component->StatesToDo->Next;
        component->StatesBeingDone = task->Tasks;
        task->Previous->Next = task->Next;
        task->Next->Previous = task->Previous;
        free(task);
        // Do not clear StatesDone here: retail preserves previously finished bits.
        if (component->StatesBeingDone & ChangeSelf)
            component->Vtable->ProcessChangeState(component, 0);
        if (component->StatesBeingDone & ChangeChildren)
            FableUiChangeChildren(component);
    }
    else
    {
        if ((component->StatesBeingDone & ChangeSelf) &&
            component->Vtable->InternalChanged(component, 0))
            component->StatesDone |= ChangeSelf;
        if ((component->StatesBeingDone & ChangeChildren) &&
            component->Vtable->ChildrenChanged(component, 0))
            component->StatesDone |= ChangeChildren;
    }
    if (component->StatesToDo->Next == component->StatesToDo &&
        (component->StatesDone & component->StatesBeingDone) == component->StatesBeingDone &&
        component->CurrentState != component->TargetState)
    {
        component->StatesDone = component->StatesBeingDone = 0;
        component->PreviousState = component->CurrentState;
        component->CurrentState = component->TargetState;
    }
}
