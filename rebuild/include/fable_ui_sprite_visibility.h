#pragma once

#include "rebuild_abi.h"

enum FableUiSpriteDisposition
{
    FableUiSpriteSubmit,
    FableUiSpriteRelease
};

// CSprite::Draw 0041AFA0..0041B065. NoRenderNextFrame is the original
// Lionhead field name. First invisible frame still submits the sprite;
// subsequent invisible frames clear its handle until visibility returns.
// This extracts the decision, not engine handle ownership or the full Draw.
inline FableUiSpriteDisposition FableUiSpriteVisibility(
    fable_u8& noRenderNextFrame, fable_u8 renderAlpha,
    float renderZoomX, float renderZoomY)
{
    if (renderAlpha && !(renderZoomX <= 0.0f && renderZoomY <= 0.0f))
    {
        noRenderNextFrame = 0;
        return FableUiSpriteSubmit;
    }
    if (noRenderNextFrame == 1)
        return FableUiSpriteRelease;
    noRenderNextFrame = 1;
    return FableUiSpriteSubmit;
}
