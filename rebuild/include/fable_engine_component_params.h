#pragma once
#include "fable_game.h"
#include "fable_engine_layer_renderer.h"
// Ego_r CEngineComponentParams, confirmed by retail RenderFrame 00B25950.
class CEngineComponentParams {
public:
    float FrameInterpolation;
    CEngineInterpolationInfo InterpolationInfo;
    unsigned long FrameID;
    unsigned long SceneFilterFlags;
    bool OffWorldTickProcessFlag;
};
FABLE_STATIC_ASSERT(sizeof(CEngineComponentParams)==128);
FABLE_STATIC_ASSERT(offsetof(CEngineComponentParams,InterpolationInfo)==4);
FABLE_STATIC_ASSERT(offsetof(CEngineComponentParams,FrameID)==116);
FABLE_STATIC_ASSERT(offsetof(CEngineComponentParams,OffWorldTickProcessFlag)==124);

class CEngineComponentRenderParams {
public:
    const CEngineComponentParams* Params;
    EEngineRenderLayer RenderLayer;
    ERenderRenderLayerFlags RenderLayerFlags;
};
FABLE_STATIC_ASSERT(sizeof(CEngineComponentRenderParams)==12);
