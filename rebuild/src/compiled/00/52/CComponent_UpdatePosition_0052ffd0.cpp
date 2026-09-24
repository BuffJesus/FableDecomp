#include "fable_ui_transform.h"

void __fastcall FableUiUpdatePosition(FableUiComponentDrawView* component, void*, float delta)
{
    FableUiAdvanceVector(component->Position, component->InitialPosition, component->TargetPosition,
        component->PositionTimeElapsed, component->PositionTime, delta);
    component->RenderPosition = component->Position;
    if (component->Vtable->UseRelativePosition(component, 0))
        FableUiConvertCoordinates(&component->RelativeRenderPosition, 0, component->Position);
    else
        component->RelativeRenderPosition = component->Position;
    if (!component->Vtable->IsPositionIndependent(component, 0))
    {
        component->RenderPosition.x *= component->ParentZoom.x;
        component->RenderPosition.y *= component->ParentZoom.y;
        component->RenderPosition.x += component->ParentPosition.x;
        component->RenderPosition.y += component->ParentPosition.y;
        component->RelativeRenderPosition.x *= component->RelativeParentZoom.x;
        component->RelativeRenderPosition.y *= component->RelativeParentZoom.y;
        component->RelativeRenderPosition.x += component->RelativeParentPosition.x;
        component->RelativeRenderPosition.y += component->RelativeParentPosition.y;
    }
}
