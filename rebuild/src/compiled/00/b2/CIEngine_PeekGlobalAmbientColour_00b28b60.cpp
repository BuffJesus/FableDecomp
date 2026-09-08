#include "engine/CIEngine.h"

struct CRGBFloatColour {
    float R;
    float G;
    float B;
    float A;
    CRGBFloatColour(float red, float green, float blue, float alpha)
        : R(red), G(green), B(blue), A(alpha) {}
};

struct CIEngine_AmbientMethods : CIEngine {
    CRGBFloatColour PeekGlobalAmbientColour() const;
};

CRGBFloatColour CIEngine_AmbientMethods::PeekGlobalAmbientColour() const {
    return CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f);
}