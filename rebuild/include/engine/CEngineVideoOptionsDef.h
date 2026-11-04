#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
// Partial RETAIL view. Donor CEngineVideoOptionsDef names these fields at
// +68/+6c; retail B24500 accesses +64/+68 after the smaller def base.
// This view is not an allocation size and does not model the unrecovered tail.
class CEngineVideoOptionsDef {
public:
    unsigned char UnrecoveredPrefix[0x3c];
    long HiresTextureMemory;
    float LODErrorTolerance;
    float CharacterLODErrorTolerance;
    float DrawDistanceMultiplier;
    float DrawDistanceMinimum;
    float DrawDistanceMaximum;
    float RepeatedMeshDrawDistanceFactor;
    float MinimumZSpriteAsMeshDistance;
    float MaximumZSpriteAsMeshDistance;
    float ZSpriteDrawDistanceMultiplier;
    long ShadowBufferSize;
    float ShadowDistanceScale;
    bool Enable2DDisplacement;
    bool Enable3DDisplacement;
    bool EnableGlow;
    bool EnableRadialBlur;
    bool EnableWaterReflection;
    bool EnableWeatherEffects;
    bool EnableColourFilter;
    bool EnableRepeatedMeshes;
    float WeatherDensity;
};
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,ShadowBufferSize)==0x64);
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,HiresTextureMemory)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,ShadowDistanceScale)==0x68);
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,Enable2DDisplacement)==0x6c);
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,Enable3DDisplacement)==0x6d);
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,EnableGlow)==0x6e);
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,EnableRadialBlur)==0x6f);
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,EnableWeatherEffects)==0x71);
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,EnableColourFilter)==0x72);
FABLE_STATIC_ASSERT(offsetof(CEngineVideoOptionsDef,WeatherDensity)==0x74);
