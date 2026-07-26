#include <new>
#include "candidates/mouse_dx_accessors_compiled.h"

void CMouseDX::SetMouseCursor(const CSurface& surface)
{
    CSurface* copiedSurface = new CSurface(surface);
    MouseCursor.Reset(copiedSurface);

    if (DisableCursorWarp != 0)
    {
        CMousePrimitiveHierarchy* hierarchy = ResolveMousePrimitiveHierarchy();
        CMouseEnginePrimitive* primitive = hierarchy->Owner->Primitive;
        primitive->Dispatch->SetCursorSurface(
            primitive, 0, 0, MouseCursor.Rep->Value);

        hierarchy = ResolveMousePrimitiveHierarchy();
        primitive = hierarchy->Owner->Primitive;
        primitive->Dispatch->SetCursorEnabled(primitive, 1);
    }
}
