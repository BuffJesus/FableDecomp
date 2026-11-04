#include "engine/CEngineLightingManager.h"
#include "fable_engine_internal.h"
#include "fable_display_viewport.h"
void CEngineLightingManager::EnableFog()
{
    NEngineInternal::GPRenderManager->SetFogEnable(true);
}
