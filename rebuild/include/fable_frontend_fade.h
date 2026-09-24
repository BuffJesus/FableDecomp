#pragma once

#include "fable_ui_colour.h"

// Functional extraction of the colour interpolation in CGuiComponent's
// 0052F900..0052FE38 prefix. This is a scaffold adapter, not a landed full method.
// Retail evaluates start + (target-start) * (2u-u*u), then truncates to a byte.
inline fable_u32 FableFrontendFadeAlpha(bool fadingIn, float elapsed, float duration)
{
    if (elapsed >= duration || duration <= 0.0f)
        return fadingIn ? 255u : 0u;
    const float phase = elapsed / duration;
    const float weight = phase * (2.0f - phase);
    return FableUiColourState::Interpolate(fadingIn ? 0 : 255,
        fadingIn ? 255 : 0, weight);
}
