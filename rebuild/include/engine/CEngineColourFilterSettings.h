#pragma once
#include "rebuild_abi.h"
#include "fable_rgb_colour.h"
// Ego_r.pdb: four floats followed by a four-component tint, total32bytes.
class CEngineColourFilterSettings {
public:
    CEngineColourFilterSettings();
    float Alpha,Saturation,Contrast,Brightness;
    CRGBFloatColour Tint;
};
FABLE_STATIC_ASSERT(sizeof(CEngineColourFilterSettings)==0x20);
