#include <new>
#include "fable_display_viewport.h"
#include "fable_2d_vector.h"
#include "fable_virtual_pixels.h"
namespace _STL {
template<> void vector<CQuickDrawTriInfo>::_M_insert_overflow(CQuickDrawTriInfo*,
    const CQuickDrawTriInfo&, const __false_type&, size_t, bool);
}
#include "fable_stlport_vector_impl.h"

void CRenderManager2D::AddText(const C3DVector& position, float scale,
    const CWideString& text, const CFontBank& font,
    const CRGBColour& colour, unsigned long flags)
{
    if (DisplayManager->ViewportOffscreen)
        return;
    CQuickDrawTriInfo info;
    info.Window = GetCurrentAWindow();
    info.Line = false;
    info.IsText = true;
    info.StringX = GFVXToAX(position.X);
    info.StringY = GFVYToAY(position.Y);
    info.StringZ = position.Z;
    info.String = text;
    info.Colour = colour;
    info.StringScale = scale;
    info.PFont = &font;
    info.StringFlags = flags;
    QuickDraw2DTriInfo.push_back(info);
}
