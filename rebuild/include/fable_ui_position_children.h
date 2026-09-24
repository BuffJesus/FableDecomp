#pragma once
#include "fable_ui_transform.h"

// Non-owning retail tree node view. Construction, ordering and ownership are
// outside this extraction. Node links are established by retail iteration.
struct FableUiPositionChildNode
{
    unsigned Unrecovered00;
    FableUiPositionChildNode* Parent;
    FableUiPositionChildNode* Left;
    FableUiPositionChildNode* Right;
    FableUiComponentDrawView* Component;
};
FABLE_STATIC_ASSERT(offsetof(FableUiPositionChildNode, Component) == 0x10);

// Both position-only and live-child passes reload their child after callbacks.
// Accessors preserve that behavior when callbacks replace entries or storage.
template<class ChildAccess>
inline void FableUiPropagateChildTransform(FableUiComponentDrawView* parent, ChildAccess access)
{
    FableUiComponentDrawView* child = access.Get();
    child->Vtable->SetParentZoom(child, 0, &parent->RenderZoom);
    child = access.Get();
    child->Vtable->SetRelativeParentZoom(child, 0, &parent->RelativeRenderZoom);
    const bool positionIndependent = parent->Vtable->IsPositionIndependent(parent, 0);
    const bool zoomIndependent = parent->Vtable->IsZoomIndependent(parent, 0);
    FableUiStateVector2 normal = parent->Position;
    if (!zoomIndependent)
    {
        // Retail rounds horizontal scaling before translation, retaining
        // the vertical product until its final addition.
        const volatile float horizontal = normal.x * parent->ParentZoom.x;
        const double vertical = static_cast<double>(normal.y) * parent->ParentZoom.y;
        normal.x = horizontal;
        normal.y = static_cast<float>(positionIndependent ? vertical : vertical + parent->ParentPosition.y);
    }
    else if (!positionIndependent) normal.y += parent->ParentPosition.y;
    if (!positionIndependent) normal.x += parent->ParentPosition.x;
    child = access.Get();
    // Preserve the original direct pointer for the wholly independent case.
    child->Vtable->SetParentPosition(child, 0,
        positionIndependent && zoomIndependent ? &parent->Position : &normal);

    const bool relative = parent->Vtable->UseRelativePosition(parent, 0);
    const bool relativeZoomIndependent = parent->Vtable->IsZoomIndependent(parent, 0);
    FableUiStateVector2 converted = parent->Position;
    if (relative) FableUiConvertCoordinates(&converted, 0, converted);
    FableUiStateVector2 inherited = converted;
    if (!relativeZoomIndependent)
    {
        double horizontal = static_cast<double>(converted.x) * parent->RelativeParentZoom.x;
        const double vertical = static_cast<double>(converted.y) * parent->RelativeParentZoom.y;
        if (!relative)
        {
            const volatile float rounded = static_cast<float>(horizontal);
            horizontal = rounded;
        }
        inherited.x = static_cast<float>(positionIndependent ? horizontal : horizontal + parent->RelativeParentPosition.x);
        inherited.y = static_cast<float>(positionIndependent ? vertical : vertical + parent->RelativeParentPosition.y);
    }
    else if (!positionIndependent)
    {
        inherited.x += parent->RelativeParentPosition.x;
        inherited.y += parent->RelativeParentPosition.y;
    }
    child = access.Get();
    child->Vtable->SetRelativeParentPosition(child, 0,
        positionIndependent && relativeZoomIndependent && !relative ? &parent->Position : &inherited);
}

struct FableUiPositionNodeAccess
{
    FableUiPositionChildNode* Node;
    FableUiComponentDrawView* Get() const { return Node->Component; }
};

// Extracted PositionChildren pass of CComponent::Update, 00531EFC..005321E0.
// This is not the complete base Update and must run after local transform updates.
inline void FableUiUpdatePositionChildren(FableUiComponentDrawView* parent)
{
    FableUiPositionChildNode* node = parent->PositionChildren->Left;
    while (node != parent->PositionChildren)
    {
        FableUiPositionNodeAccess access = {node};
        FableUiPropagateChildTransform(parent, access);

        if (node->Right)
        {
            node = node->Right;
            while (node->Left) node = node->Left;
        }
        else
        {
            FableUiPositionChildNode* ancestor = node->Parent;
            while (node == ancestor->Right) { node = ancestor; ancestor = ancestor->Parent; }
            if (node->Right != ancestor) node = ancestor;
        }
    }
}
