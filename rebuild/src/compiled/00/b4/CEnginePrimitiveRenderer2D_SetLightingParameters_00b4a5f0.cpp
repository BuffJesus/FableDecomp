#include "engine/CEnginePrimitiveRenderer2D.h"
#include "fable_2d_vector.h"
#include "fable_rgb_colour.h"
void CEnginePrimitiveRenderer2D::SetLightingParameters(const C3DVector& direction,const CRGBFloatColour& ambient,const CRGBFloatColour& diffuse,const CRGBFloatColour& backlight)
{
    *reinterpret_cast<CRGBFloatColour*>(&AmbientLightColour_R) = ambient;
    *reinterpret_cast<CRGBFloatColour*>(&DiffuseLightColour_R) = diffuse;
    *reinterpret_cast<CRGBFloatColour*>(&BackLightColour_R) = backlight;
    C3DVector& vector = *reinterpret_cast<C3DVector*>(&LightVector_X);
    vector = direction;
    vector.AccurateNormalise();
}
