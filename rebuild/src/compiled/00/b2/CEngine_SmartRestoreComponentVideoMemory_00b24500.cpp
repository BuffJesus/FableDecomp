#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "fable_engine_component.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEngineScreenEffectGlowRenderer.h"
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

namespace {
inline EEngineVideoOptionQuality ReduceQuality(EEngineVideoOptionQuality quality)
{
    switch(quality) {
    case ENGINE_VIDEO_OPTION_QUALITY_NORMAL:return ENGINE_VIDEO_OPTION_QUALITY_LOW;
    case ENGINE_VIDEO_OPTION_QUALITY_HIGHEST:return ENGINE_VIDEO_OPTION_QUALITY_HIGH;
    case ENGINE_VIDEO_OPTION_QUALITY_HIGH:return ENGINE_VIDEO_OPTION_QUALITY_NORMAL;
    default:return ENGINE_VIDEO_OPTION_QUALITY_LOW;
    }
}
}
CEngine::ESmartRestoreComponentVideoMemoryResult CEngine::SmartRestoreComponentVideoMemory()
{
    using namespace NEngineInternal;
    EEngineVideoOptionQuality shadow=VideoOptions.ShadowQuality;
    EEngineVideoOptionQuality effects=VideoOptions.EffectsQuality;
    bool reduced=false;
    for(;;) {
        for(CEngineComponent** component=EngineComponents.begin();component!=EngineComponents.end();++component)
            (*component)->OnReleaseDefaultPoolResources();
        for(CEngineComponent** component=EngineComponents.begin();component!=EngineComponents.end();++component)
            if(!(*component)->OnRestoreDefaultPoolResources())return SMART_RESTORE_COMPONENT_VIDEO_MEMORY_FAILED;
        const CEngineVideoOptionsDef* effectDef=reinterpret_cast<const CEngineVideoOptionsDef*>(PEngineVideoOptionsDefs[effects].Object);
        const CEngineVideoOptionsDef* shadowDef=reinterpret_cast<const CEngineVideoOptionsDef*>(PEngineVideoOptionsDefs[shadow].Object);
        bool failed=GPShadowRenderer && !GPShadowRenderer->ChangeVideoOptionShadowQuality(shadowDef->ShadowBufferSize,shadowDef->ShadowDistanceScale);
        failed|=GPScreenEffectDisplacementRenderer && !GPScreenEffectDisplacementRenderer->SetEffectQuality(*effectDef);
        failed|=GPScreenEffectGlowRenderer && !GPScreenEffectGlowRenderer->SetEffectQuality(*effectDef);
        failed|=GPScreenEffectRadialBlurRenderer && !GPScreenEffectRadialBlurRenderer->SetEffectQuality(*effectDef);
        if(!failed)return reduced?SMART_RESTORE_COMPONENT_VIDEO_MEMORY_REDUCED_DETAIL:SMART_RESTORE_COMPONENT_VIDEO_MEMORY_SUCCESS;
        reduced=true;
        if(shadow>ENGINE_VIDEO_OPTION_QUALITY_NORMAL)shadow=ReduceQuality(shadow);
        else if(effects>ENGINE_VIDEO_OPTION_QUALITY_LOW)effects=ReduceQuality(effects);
        else if(shadow>ENGINE_VIDEO_OPTION_QUALITY_LOW)shadow=ReduceQuality(shadow);
        else return SMART_RESTORE_COMPONENT_VIDEO_MEMORY_FAILED;
    }
}
