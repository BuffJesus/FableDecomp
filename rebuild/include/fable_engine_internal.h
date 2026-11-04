#pragma once
class CEngineSubPrimitiveRenderer;
class CEngineScreenEffectOutlineGlow;
class CEngine;
class CEngineSurfaceCompositionManager;
class CRenderManager;
class CDisplayManager;
class CEnginePrimitiveRendererSharedData;
class CEngineVideoOptions;
class CEngineCamera;
class CEngineEnvironment;
class CShaderRenderManager;
class CEngineLightingManager;
class CEnginePrimitiveOcclusionManager;
class CEnginePrimitiveRenderer2D;
class CEngineWeatherRenderer;
class CEngineResourceManager;
class CEngineDef;
class CEnginePrimitiveRenderer;
class CEnginePrimitiveAttachmentManager;
class CEnginePrimitiveTypeInfo;
class CEngineWorldMap;
class CEngineWaterRenderer;
class CEngineShadowRenderer;
class CEngineScreenEffectColourFilter;
class CEngineScreenEffectDisplacementRenderer;
class CEngineScreenEffectGlowRenderer;
class CEngineScreenEffectRadialBlurRenderer;
class CEngineLandscapeRenderer;
class CEngineLocalDetailGenerator;
// Ego_r PDB namespace identities; retail CEngine::Uninitialise and Initialise.
namespace NEngineInternal {
    extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer; // 01436E74
    extern CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow; // 01436E3C
    extern CEnginePrimitiveAttachmentManager* GPPrimitiveAttachmentManager; // 01436E70
    extern CEngineSurfaceCompositionManager* GPSurfaceCompositionManager; // 01436EAC
    extern CEngine* GPEngine; // 01436EA4
    extern CEngineScreenEffectDisplacementRenderer* GPScreenEffectDisplacementRenderer; // 01436E38
    extern CEngineScreenEffectGlowRenderer* GPScreenEffectGlowRenderer; // 01436E34
    extern CEngineScreenEffectRadialBlurRenderer* GPScreenEffectRadialBlurRenderer; // 01436E30
    extern CEngineShadowRenderer* GPShadowRenderer; // 01436E60
    extern CEngineScreenEffectColourFilter* GPScreenEffectColourFilter; // 01436E40
    extern CEngineLandscapeRenderer* GPLandscapeRenderer; // 01436EA8
    extern CEngineLocalDetailGenerator* GPLocalDetailGenerator; // 01436E88
    extern CEngineWaterRenderer* GPWaterRenderer; // 01436E54
    extern CEngineWorldMap* GPEngineWorldMap; // 01436E8C
    extern const CEngineDef* GPEngineDef; // 01436E20; pointee const in Ego_r DIA.
    extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; // 01436E80
    extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; // 01436E84
    extern CEngineResourceManager* GPResourceManager; // 01436E98
    extern CEngineWeatherRenderer* GPWeatherRenderer; // 01436E44
    extern CEnginePrimitiveRenderer2D* GPPrimitiveRenderer2D; // 01436E7C
    extern CEnginePrimitiveOcclusionManager* GPPrimitiveOcclusionManager; // 01436E58
    extern CEngineLightingManager* GPLightingManager; // 01436E9C
    extern CEngineCamera* GPCamera; // 01436EA0
    extern CEngineEnvironment* GPEngineEnvironment; // 01436E48
    extern CShaderRenderManager* GPShaderRenderManager; // 01436E14
    extern CRenderManager* GPRenderManager; // 01436E18
    extern CDisplayManager* GPDisplayManager; // 01436E1C
    extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; // 01436E78
    extern const CEngineVideoOptions* GPVideoOptions; // 01436EB0
}
