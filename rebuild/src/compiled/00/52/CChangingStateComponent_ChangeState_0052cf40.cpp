#include "fable_ui_state_progress.h"
#include "fable_ui_state.h"
#include <stdlib.h>

static void AppendTask(FableUiStateTaskNode* head, unsigned tasks)
{
    FableUiStateTaskNode* node = static_cast<FableUiStateTaskNode*>(malloc(sizeof(*node)));
    // Retail assumes allocation succeeds; recovery preserves that contract.
    node->Tasks = tasks;
    node->Previous = head->Previous;
    node->Next = head;
    node->Previous->Next = node;
    head->Previous = node;
}

void __fastcall FableUiChangeState(FableUiStateProgressView* component, void*, unsigned requested)
{
    if (component->RequestedState == requested) return;
    component->RequestedState = requested;
    component->StatesDone = component->StatesBeingDone = 0;
    FableUiStateTaskNode* head = component->StatesToDo;
    for (FableUiStateTaskNode* task = head->Next; task != component->StatesToDo; )
    {
        FableUiStateTaskNode* next = task->Next;
        free(task);
        task = next;
    }
    component->StatesToDo->Next = component->StatesToDo;
    component->StatesToDo->Previous = component->StatesToDo;
    component->PreviousState = component->CurrentState;
    component->CurrentState = component->TargetState;
    if (requested == 0 || requested == 6)
        component->Vtable->RequestedStateZeroOrSix(component, 0);
    else if (requested == 1 || requested == 5)
        component->Vtable->RequestedStateOneOrFive(component, 0);
    CUIStateRecoveredLayout* state = component->Vtable->FindState(component, 0, requested);
    if (!component->Vtable->HasState(component, 0, requested))
    {
        component->UpdateTime = component->ParentUpdateTime;
        AppendTask(component->StatesToDo, 1);
        FableUiChangeChildren(component);
        return;
    }
    component->TargetState = requested;
    component->UpdateTime = state->updateTime >= 0.0f ? state->updateTime : component->ParentUpdateTime;
    switch (state->stateChangeType)
    {
    case 0: AppendTask(component->StatesToDo, 3); FableUiChangeChildren(component); break;
    case 1: AppendTask(component->StatesToDo, 2); AppendTask(component->StatesToDo, 1); break;
    case 2: AppendTask(component->StatesToDo, 1); AppendTask(component->StatesToDo, 2);
            FableUiChangeChildren(component); break;
    case 3: AppendTask(component->StatesToDo, 2); break;
    case 4: AppendTask(component->StatesToDo, 1); FableUiChangeChildren(component); break;
    }
}
