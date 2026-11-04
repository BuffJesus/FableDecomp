#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineScreenEffectColourFilter.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineLocalDetailGenerator.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"

bool CEngine::SetVideoOptions(const CEngineVideoOptions& options)
{
    using namespace NEngineInternal;
    if(!IsInitialised)return false;
    EnablePostDeviceReset=false;
    VideoOptions=options;
    GPVideoOptions=&VideoOptions;
    bool exact=true;
    if(GPDisplayManager->SetDisplayMode(options.ScreenResolution,options.BitsPerPixel,options.Antialiasing,options.RefreshRate)!=CDisplayManager::CREATE_DEVICE_SUCCESS) {
        exact=false;
        if(GPDisplayManager->SetDisplayMode(options.ScreenResolution,options.BitsPerPixel,SURFACE_MULTISAMPLE_NONE,0)==CDisplayManager::CREATE_DEVICE_FAILED) {
            C2DExtentsI fallback=options.ScreenResolution;
            if(fallback.X>1024)fallback.X=1024;
            if(fallback.Y>768)fallback.Y=768;
            if(GPDisplayManager->SetDisplayMode(fallback,options.BitsPerPixel,SURFACE_MULTISAMPLE_NONE,0)==CDisplayManager::CREATE_DEVICE_FAILED) {
                EnablePostDeviceReset=true;
                return false;
            }
        }
    }
    GPDisplayManager->ChangePresentationInterval(!options.VerticalSync,1,true);
    GPWeatherRenderer->SetEffectQuality(*reinterpret_cast<const CEngineVideoOptionsDef*>(PEngineVideoOptionsDefs[VideoOptions.EffectsQuality].Object));
    GPScreenEffectColourFilter->SetEffectQuality(*reinterpret_cast<const CEngineVideoOptionsDef*>(PEngineVideoOptionsDefs[VideoOptions.EffectsQuality].Object));
    GPWaterRenderer->SetEffectQuality(*reinterpret_cast<const CEngineVideoOptionsDef*>(PEngineVideoOptionsDefs[VideoOptions.EffectsQuality].Object));
    GPResourceManager->ResizeHiresTextureMemory(reinterpret_cast<const CEngineVideoOptionsDef*>(GPEngine->PEngineVideoOptionsDefs[VideoOptions.TextureDetail].Object)->HiresTextureMemory);
    EEngineVideoOptionQuality mesh=VideoOptions.MeshDetail;
    const CEngineVideoOptionsDef* meshDef=reinterpret_cast<const CEngineVideoOptionsDef*>(GPEngine->PEngineVideoOptionsDefs[mesh].Object);
    GPPrimitiveRenderer->SetDrawDistanceQuality(mesh,*meshDef);
    if(GPLocalDetailGenerator)GPLocalDetailGenerator->UpdateLoadingDistanceVideoOptions(*meshDef);
    GPPrimitiveOcclusionManager->StartNewScene();
    if(SmartRestoreComponentVideoMemory()!=SMART_RESTORE_COMPONENT_VIDEO_MEMORY_SUCCESS) {
        EnablePostDeviceReset=true;
        return false;
    }
    EnablePostDeviceReset=true;
    return exact;
}
