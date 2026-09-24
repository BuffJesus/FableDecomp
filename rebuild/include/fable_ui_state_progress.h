#pragma once

#include "fable_ui_component_draw.h"

struct FableUiStateTaskNode
{
    FableUiStateTaskNode* Next;
    FableUiStateTaskNode* Previous;
    unsigned Tasks;
};

// Retail view of CChangingStateComponent. Names come from the Ego_r PDB;
// retail uses a one-pointer list at +13C, not the donor's 12-byte list.
struct FableUiStateProgressView : FableUiComponentDrawView
{
    unsigned char Unrecovered130[4];
    unsigned StatesBeingDone;
    unsigned StatesDone;
    FableUiStateTaskNode* StatesToDo;
    float ParentUpdateTime;
    unsigned CurrentState;
    unsigned TargetState;
    unsigned RequestedState;
    float UpdateTime;
    unsigned char PreviousUpdateChanged;
    unsigned char Unrecovered155[3];
    unsigned PreviousState;
};

FABLE_STATIC_ASSERT(offsetof(FableUiStateProgressView, StatesBeingDone) == 0x134);
FABLE_STATIC_ASSERT(offsetof(FableUiStateProgressView, StatesDone) == 0x138);
FABLE_STATIC_ASSERT(offsetof(FableUiStateProgressView, StatesToDo) == 0x13C);
FABLE_STATIC_ASSERT(offsetof(FableUiStateProgressView, PreviousUpdateChanged) == 0x154);
FABLE_STATIC_ASSERT(sizeof(FableUiStateProgressView) == 0x15C);

// Semantic name: the original name of the +C4 query is not established.
bool __fastcall FableUiHasCompletedStateChange(FableUiComponentDrawView*, void*);
bool __fastcall FableUiChangedStateLastUpdate(FableUiStateProgressView*, void*);
void __fastcall FableUiUpdateStateChange(FableUiStateProgressView*, void*);
void __fastcall FableUiChangeState(FableUiStateProgressView*, void*, unsigned requested);
void __fastcall FableUiProcessChangeState(FableUiStateProgressView*, void*);
bool __fastcall FableUiInternalChanged(FableUiComponentDrawView*, void*);
bool __fastcall FableUiChildrenChanged(FableUiComponentDrawView*, void*);
bool __fastcall FableUiBasePositionIndependent(FableUiComponentDrawView*, void*);
bool __fastcall FableUiBaseZoomIndependent(FableUiComponentDrawView*, void*);
bool __fastcall FableUiStatePositionIndependent(FableUiComponentDrawView*, void*);
bool __fastcall FableUiStateZoomIndependent(FableUiComponentDrawView*, void*);
void __fastcall FableUiChangingStateUpdate(FableUiStateProgressView*, void*, float delta);
// Recovered base Update; concrete child construction/services remain separate.
void __fastcall FableUiBaseComponentUpdate(FableUiComponentDrawView*, void*, float delta);

// Both ChangeState and UpdateStateChange use this exact dispatch policy.
inline void FableUiChangeChildren(FableUiStateProgressView* component)
{
    for (unsigned i = 0; i < component->Children.Size(); ++i)
    {
        FableUiComponentDrawView* child = component->Children.Begin[i].Data;
        if (child->Vtable->GetParent(child, 0) != component) continue;
        child = component->Children.Begin[i].Data;
        if (child->Vtable->GetType(child, 0) == 8 &&
            (component->RequestedState == 1 || component->RequestedState == 3 ||
             component->RequestedState == 4)) continue;
        child = component->Children.Begin[i].Data;
        child->Vtable->ChangeState(child, 0, component->RequestedState, component->UpdateTime);
    }
}
