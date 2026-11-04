#include "engine/CEngineLightingManager.h"
#include "fable_2d_vector.h"
void CEngineLightingManager::SetGlobalLightingParameters(const C3DVector& direction,const CRGBFloatColour& ambient,const CRGBFloatColour& diffuse,const CRGBFloatColour& backlight)
{
    StateUpdateFlags |= 3;
    ThemeChannelsBegin->AmbientColour = ambient;
    ThemeChannelsBegin->DiffuseColour = diffuse;
    ThemeChannelsBegin->BacklightColour = backlight;
    C3DVector& vector = *reinterpret_cast<C3DVector*>(&DiffuseVector_X);
    vector = direction;
    vector.AccurateNormalise();
}
