#include "fable_ui_component_draw.h"

static void DrawChildren(FableUiComponentDrawView* component,
    FableUiComponentChildrenStorage& children, bool retiring,
    void* engine, void* handle, long layer, long* index)
{
    // Re-read vector entries and size across virtual calls, as retail does.
    // A callback may change them; retaining a child pointer across all queries
    // would silently change which component receives the draw.
    for (unsigned i = 0; i < children.Size(); ++i)
    {
        FableUiComponentDrawView* child = children.Begin[i].Data;
        if (child->Vtable->GetParent(child, 0) != component)
        {
            // Retail 0053036A uses the LIVE vector in this retiring-child
            // branch. Preserve the quirk; do not substitute children here.
            child = retiring ? component->Children.Begin[i].Data : children.Begin[i].Data;
            if (!child->Vtable->AcceptForeignParent(child, 0)) continue;
        }
        child = children.Begin[i].Data;
        if (child->Vtable->IsDrawSuppressed(child, 0)) continue;
        child = children.Begin[i].Data;
        if (child->Vtable->IsDrawSuppressed(child, 0)) continue;
        child = children.Begin[i].Data;
        child->Vtable->Draw(child, 0, engine, handle, layer, index, component);
    }
}

void __fastcall FableUiComponentDraw(FableUiComponentDrawView* component, void*,
    void* engine, void* handle, long parentLayer, long* index, FableUiComponentDrawView*)
{
    // Both queries execute even when the first one returns true.
    const bool independent = component->Vtable->IsIndependent(component, 0);
    const bool layerIndependent = component->Vtable->IsLayerIndependent(component, 0);
    const long layer = component->Layer + (independent || layerIndependent ? 0 : parentLayer);
    DrawChildren(component, component->Children, false, engine, handle, layer, index);
    DrawChildren(component, component->ChildrenToDelete, true, engine, handle, layer, index);
}
