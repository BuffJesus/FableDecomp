#include "fable_font_bank.h"
#include "fable_virtual_pixels.h"
float CFontBank::GetVStringWidth(const CWideString& text) const
{
    C2DExtentsF extents = GetStringExtents(text.GetAsCharArray(), false);
    extents.X = GFAXToVX(extents.X);
    extents.Y = GFAYToVY(extents.Y);
    return extents.X;
}
