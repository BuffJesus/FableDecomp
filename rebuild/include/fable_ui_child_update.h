#pragma once
#include "fable_ui_position_children.h"

struct FableUiLiveChildAccess
{
    FableUiComponentDrawView* Parent;
    unsigned Index;
    FableUiComponentDrawView* Get() const { return Parent->Children.Begin[Index].Data; }
};

// Live-child preparation/update portion of 00531EC0, ending before deletion
// processing at 005325AB. A false result follows the skip to 00532789.
// Does not iterate Children or perform the subsequent ownership operations.
template<class ChildAccess>
inline bool FableUiPrepareAndUpdateChild(FableUiComponentDrawView* parent, ChildAccess access, float delta)
{
    FableUiComponentDrawView* child = access.Get();
    if (!child->Vtable->GetParent(child, 0))
    {
        child = access.Get();
        child->Vtable->SetParent(child, 0, parent);
    }
    child = access.Get();
    bool inheritsPosition = child->Parent == parent && !child->Vtable->GetPositionParent(child, 0);
    if (!inheritsPosition)
    {
        child = access.Get();
        inheritsPosition = child->Vtable->GetPositionParent(child, 0) == parent;
    }
    if (!inheritsPosition)
    {
        child = access.Get();
        inheritsPosition = child->Vtable->AcceptForeignParent(child, 0);
    }
    if (inheritsPosition) FableUiPropagateChildTransform(parent, access);
    child = access.Get();
    if (child->Parent != parent && !child->Vtable->AcceptForeignParent(child, 0)) return false;
    child = access.Get();
    child->Vtable->SetParentColour(child, 0, &parent->RenderColour);
    child = access.Get();
    child->Vtable->Update(child, 0, delta);
    return true;
}

inline bool FableUiPrepareAndUpdateLiveChild(FableUiComponentDrawView* parent, unsigned index, float delta)
{
    FableUiLiveChildAccess access = {parent, index};
    return FableUiPrepareAndUpdateChild(parent, access, delta);
}

struct FableUiRetiringChildAccess
{
    FableUiComponentDrawView* Parent;
    unsigned Index;
    FableUiComponentDrawView* Get() const { return Parent->ChildrenToDelete.Begin[Index].Data; }
};

// Matching retiring-child prefix, 005327C0..00532C0D (skip: 00532CD9).
// Completion checks and counted-pointer removal must follow a true result.
inline bool FableUiPrepareAndUpdateRetiringChild(FableUiComponentDrawView* parent, unsigned index, float delta)
{
    FableUiRetiringChildAccess access = {parent, index};
    return FableUiPrepareAndUpdateChild(parent, access, delta);
}
