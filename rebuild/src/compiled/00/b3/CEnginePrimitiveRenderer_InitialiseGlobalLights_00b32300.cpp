#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineLightingManager.h"
#include "fable_engine_internal.h"
#include "fable_display_viewport.h"
#include "fable_rgb_colour.h"
using namespace NEngineInternal;
void CEnginePrimitiveRenderer::InitialiseGlobalLights()
{
    GPLightingManager->UpdateGlobalLightingStates();
    GPRenderManager->SetDiffuseMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL);
    GPRenderManager->SetAmbientMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL);
    GPRenderManager->SetMaterial(CRGBColour(255,255,255,255),CRGBColour(255,255,255,255));
    GPRenderManager->SetNormalizeNormals(true);
}
