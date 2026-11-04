// lib_render_manager_2d.cpp, retail 0x009DE2A0 (Ego_r
// ?Draw2DCircle@CRenderManager2D@@QAE_NABVC2DVector@@MABVCRGBColour@@K@Z): an
// outline of 2/3 * pi * r segments in actual pixels, each drawn with
// Draw2DLine back in virtual coordinates. Validated work-only on 2026-10-13
// (native behaviour fixture); landed against the shared header.
#include <math.h>
#include "fable_display_viewport.h"
#include "fable_2d_vector.h"
#include "fable_virtual_pixels.h"

bool CRenderManager2D::Draw2DCircle(const C2DVector& centre, float radius, const CRGBColour& colour, unsigned long flags)
{
    const float x = GFVXToAX(centre.X);
    const float y = GFVYToAY(centre.Y);
    const float actual_radius = GFVXToAX(radius);
    const float pi = (float)(asin(1.0) * 2.0);
    const long segments = (long)(pi * actual_radius * (2.0f / 3.0f));
    for (long i = 0; i < segments; ++i)
    {
        const float from = i * pi * 2.0f / (float)segments;
        const float to = (i + 1) * pi * 2.0f / (float)segments;
        Draw2DLine(C2DVector(GFAXToVX((float)(sin(from) * actual_radius + x)), GFAYToVY((float)(cos(from) * actual_radius + y))),
                   C2DVector(GFAXToVX((float)(sin(to) * actual_radius + x)), GFAYToVY((float)(cos(to) * actual_radius + y))),
                   colour, flags);
    }
    return true;
}
