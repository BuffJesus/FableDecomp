#include "fable_engine.h"
#include "engine/CEngineEnvironment.h"
#include "fable_engine_internal.h"

void CEngine::SetEnvironmentalEffectsAlwaysUpdate(bool always)
{
    NEngineInternal::GPEngineEnvironment->EnvironmentalEffectsAlwaysUpdate = always;
}
