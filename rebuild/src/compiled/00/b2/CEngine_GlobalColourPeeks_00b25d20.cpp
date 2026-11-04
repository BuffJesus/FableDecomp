#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineLightingManager.h"

const CRGBFloatColour CEngine::PeekGlobalDiffuseColour() const
{
    return NEngineInternal::GPLightingManager->ThemeChannelsBegin->DiffuseColour;
}

const CRGBFloatColour CEngine::PeekGlobalAmbientColour() const
{
    return NEngineInternal::GPLightingManager->ThemeChannelsBegin->AmbientColour;
}
