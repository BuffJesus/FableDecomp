#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEngineScene.h"

void CEnginePrimitiveRenderer::SetDrawDistanceQuality(EEngineVideoOptionQuality,const CEngineVideoOptionsDef& quality)
{
    DrawDistanceFactor=quality.DrawDistanceMultiplier;
    DrawDistanceMinimum=quality.DrawDistanceMinimum;
    DrawDistanceMaximum=quality.DrawDistanceMaximum;
    RepeatedMeshDrawDistanceFactor=quality.RepeatedMeshDrawDistanceFactor;
    ZSpriteDrawDistanceFactor=quality.ZSpriteDrawDistanceMultiplier;
    MinimumZSpriteAsMeshDistance=quality.MinimumZSpriteAsMeshDistance;
    MaximumZSpriteAsMeshDistance=quality.MaximumZSpriteAsMeshDistance;
    DrawDistanceInvalid=true;
    for(CEngineScene** scene=SceneList.begin();scene!=SceneList.end();++scene)
        (*scene)->InvalidateBoundingBoxesAndDrawDistances();
    RepeatedStaticMeshManager_Data->RenderEnabled=quality.EnableRepeatedMeshes;
}
