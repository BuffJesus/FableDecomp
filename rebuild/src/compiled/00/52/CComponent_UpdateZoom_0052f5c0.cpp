#include "fable_ui_transform.h"

void __fastcall FableUiUpdateZoom(FableUiComponentDrawView* component, void*, float delta)
{
    FableUiAdvanceVector(component->Zoom, component->InitialZoom, component->TargetZoom,
        component->ZoomTimeElapsed, component->ZoomTime, delta);
    if (!component->Vtable->IsZoomIndependent(component, 0))
    {
        component->RenderZoom.x = component->Zoom.x * component->ParentZoom.x;
        component->RenderZoom.y = component->Zoom.y * component->ParentZoom.y;
        if (component->Vtable->UseRelativeZoom(component, 0))
        {
            // The native dependent branch keeps conversion intermediates in
            // x87 registers until multiplication by the relative parent zoom.
            double x = component->Zoom.x, y = component->Zoom.y;
            if (FableUiCoordinateConversionEnabled)
                x = x / FableUiCoordinateSourceExtent.x * FableUiCoordinateDestinationExtent.x;
            component->RelativeRenderZoom.x = static_cast<float>(x * component->RelativeParentZoom.x);
            if (FableUiCoordinateConversionEnabled)
                y = y / FableUiCoordinateSourceExtent.y * FableUiCoordinateDestinationExtent.y;
            component->RelativeRenderZoom.y = static_cast<float>(y * component->RelativeParentZoom.y);
        }
        else
        {
            component->RelativeRenderZoom.x = component->Zoom.x * component->RelativeParentZoom.x;
            component->RelativeRenderZoom.y = component->Zoom.y * component->RelativeParentZoom.y;
        }
    }
    else
    {
        component->RenderZoom = component->Zoom;
        if (component->Vtable->UseRelativeZoom(component, 0))
            FableUiConvertCoordinates(&component->RelativeRenderZoom, 0, component->Zoom);
        else
            component->RelativeRenderZoom = component->Zoom;
    }
    if (!component->Parent || component->Vtable->IsZoomIndependent(component, 0))
    {
        FableUiStateVector2 scale;
        component->RelativeRenderZoom.x *= FableUiGetManagerScale(FableUiGetManager(), 0, &scale)->x;
        component->RelativeRenderZoom.y *= FableUiGetManagerScale(FableUiGetManager(), 0, &scale)->y;
        component->RenderZoom.x *= FableUiGetManagerScale(FableUiGetManager(), 0, &scale)->x;
        component->RenderZoom.y *= FableUiGetManagerScale(FableUiGetManager(), 0, &scale)->y;
    }
}
